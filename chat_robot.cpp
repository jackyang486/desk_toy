#include <algorithm>
#include <cctype>
#include "chat_robot.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_tls.h"
#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"
#include "mbedtls/base64.h"
#include "SD_MMC.h"
#include "ESP32_board.h"
#include "setting.h"
#include "ai_vox_engine.h"
#include "audio_device/audio_device_es8311.h"
#include "components/espressif/button/button_gpio.h"
#include "components/espressif/button/iot_button.h"
#include "components/espressif/esp_audio_codec/esp_audio_simple_dec.h"
#include "components/espressif/esp_audio_codec/esp_mp3_dec.h"
#include "components/wifi_configurator/wifi_configurator.h"
#include "HTTPClient.h"
#include "cJSON.h"
#include "photo_album.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "src/generated/gui_guider.h"

static bool screen_loaded = false;
static bool has_initial = false;
static std::string g_current_emotion = "";
static TaskHandle_t g_chat_task_handle = nullptr;
static volatile bool g_pending_next_photo = false;   // MCP 请求切下一张照片/图（LVGL 线程执行）
// ---- 识图模式：机器人屏直接显示 SD 卡随机图片（代替表情），AI 可对当前图片识图 ----
// 显示路径复用 Photo_album 单例的 lv_img_path 成员缓冲（取到指针立即用于 lv_*_set_src）
static volatile bool g_vision_mode = false;          // true：识图模式中，表情被图片占用
static volatile bool g_pending_vision_enter = false; // MCP 请求进入识图模式（LVGL 线程执行）
static volatile bool g_pending_vision_exit = false;  // MCP 请求退出识图模式（LVGL 线程执行）

static const std::vector<std::string> kEmotionOrder = {
    "neutral", "happy", "laughing", "funny", "sad", "angry", "crying",
    "loving", "embarrassed", "surprised", "shocked", "thinking", "winking",
    "cool", "relaxed", "delicious", "kissy", "confident", "sleepy", "silly",
    "confused"
};   


namespace {

constexpr gpio_num_t kButtonBoot = GPIO_NUM_0;
constexpr gpio_num_t kLedPin = GPIO_NUM_2;
constexpr smartconfig_type_t kSmartConfigType = SC_TYPE_ESPTOUCH_AIRKISS;  // ESPTouch and AirKiss

auto g_observer = std::make_shared<ai_vox::Observer>();

button_handle_t g_button_boot_handle = nullptr;

void InitMcpTools() {
  auto& engine = ai_vox::Engine::GetInstance();
  engine.AddMcpTool("self.audio_speaker.set_volume",         // tool name
                    "Set the volume of the audio speaker.",  // tool description
                    {
                        {
                            "volume",  // parameter name

                            ai_vox::ParamSchema<int64_t>{
                                // parameter type can be bool, std::string or int64_t
                                .default_value = std::nullopt,  // default value, set to std::nullopt if not specified
                                .min = 0,                       // minimum value, set to std::nullopt if not specified
                                .max = 100,                     // maximum value, set to std::nullopt if not specified
                            },
                        },
                        // add more parameter schema as needed
                    }  // parameter schema
  );

  engine.AddMcpTool("self.audio_speaker.get_volume",         // tool name
                    "Get the volume of the audio speaker.",  // tool description
                    {
                        // empty
                    }  // parameter schema
  );

  engine.AddMcpTool("self.led.set",                                           // tool name
                    "Set the state of the LED, true for on, false for off.",  // tool description
                    {
                        {
                            "state",  // parameter name
                            ai_vox::ParamSchema<bool>{
                                // parameter type can be bool, std::string or int64_t
                                .default_value = std::nullopt,  // default value, set to std::nullopt if not specified
                            },                                  // parameter type
                        },
                        // add more parameter schema as needed
                    }  // parameter schema
  );

  engine.AddMcpTool("self.led.get",                                           // tool name
                    "Get the state of the LED, true for on, false for off.",  // tool description
                    {
                        // empty
                    }  // parameter schema
  );

  engine.AddMcpTool("self.screen.set_brightness",
                    "Set the brightness of the screen backlight, 0-100.",
                    {
                        {
                            "brightness",
                            ai_vox::ParamSchema<int64_t>{
                                .default_value = std::nullopt,
                                .min = 0,
                                .max = 100,
                            },
                        },
                    }
  );

  engine.AddMcpTool("self.screen.get_brightness",
                    "Get the current brightness of the screen backlight.",
                    {}
  );

  engine.AddMcpTool("self.screen.set_timeout",
                    "Set the screen timeout in minutes, 0 means never turn off.",
                    {
                        {
                            "minutes",
                            ai_vox::ParamSchema<int64_t>{
                                .default_value = std::nullopt,
                                .min = 0,
                                .max = 60,
                            },
                        },
                    }
  );

  engine.AddMcpTool("self.screen.get_timeout",
                    "Get the current screen timeout in minutes.",
                    {}
  );

  engine.AddMcpTool("self.photo_album.open",
                    "Enter image recognition (vision) mode: show a random picture from the "
                    "SD card on the robot screen instead of the emotion face. Use this when "
                    "the user asks to open the photo album / browse pictures / recognize an image.",
                    {}
  );

  engine.AddMcpTool("self.photo_album.close",
                    "Exit image recognition (vision) mode and restore the robot emotion face. "
                    "Use this when the user asks to close the picture / exit the photo album.",
                    {}
  );

  engine.AddMcpTool("self.photo_album.next_image",
                    "Switch to the next picture: in vision mode show another random picture on "
                    "the robot screen; in the photo album screen switch to the next album picture. "
                    "Use this when the user asks to see the next photo.",
                    {}
  );

  engine.AddMcpTool("self.photo_album.describe_image",
                    "Describe the picture currently displayed (in vision mode on the robot screen, "
                    "or in the photo album), or answer questions about it. Only usable while a "
                    "local picture is shown.",
                    {
                        {
                            "question",
                            ai_vox::ParamSchema<std::string>{
                                .default_value = std::string("请描述这张图片的内容"),
                            },
                        },
                    }
  );
}
}

void SetEmotion(const std::string& emotion) {
  if (guider_ui.screen_robot == nullptr) {
    return;
  }
  if (g_vision_mode) {
    return;  // 识图模式中：机器人屏背景被图片占用，退出识图模式后统一恢复表情
  }

  static const std::map<std::string, const char*> emotion_map = {
      {"neutral", "S:/image/neutral.jpg"},
      {"happy", "S:/image/happy.jpg"},
      {"laughing", "S:/image/laughing.jpg"},
      {"funny", "S:/image/funny.jpg"},
      {"sad", "S:/image/sad.jpg"},
      {"angry", "S:/image/angry.jpg"},
      {"crying", "S:/image/crying.jpg"},
      {"loving", "S:/image/loving.jpg"},
      {"embarrassed", "S:/image/embarrassed.jpg"},
      {"surprised", "S:/image/surprised.jpg"},
      {"shocked", "S:/image/shocked.jpg"},
      {"thinking", "S:/image/thinking.jpg"},
      {"winking", "S:/image/winking.jpg"},
      {"cool", "S:/image/cool.jpg"},
      {"relaxed", "S:/image/relaxed.jpg"},
      {"delicious", "S:/image/delicious.jpg"},
      {"kissy", "S:/image/kissy.jpg"},
      {"confident", "S:/image/confident.jpg"},
      {"sleepy", "S:/image/sleep.jpg"},
      {"silly", "S:/image/silly.jpg"},
      {"confused", "S:/image/confused.jpg"},
  };

  const std::string normalized = emotion.empty() ? "neutral" : emotion;
  const auto it = emotion_map.find(normalized);
  const char* image_path = (it != emotion_map.end()) ? it->second : "S:/image/neutral.jpg";

  if (g_current_emotion != normalized) {
    lv_obj_set_style_bg_img_src(guider_ui.screen_robot, image_path, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_opa(guider_ui.screen_robot, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    g_current_emotion = normalized;
  }
}

static char g_pending_emotion[24];            // 表情名最长 "embarrassed"(12)，24B 足够
static volatile bool g_pending_emotion_valid = false;
// 状态标签枚举：聊天任务只传枚举，中文文本由 chat_ui_apply_timer_cb（LVGL 线程）统一设置
enum class PendingStatus {
  kNone,
  kIdle,          // 空闲中...
  kLoading,       // 加载中...
  kLoadingFailed, // 加载失败...
  kStandby,       // 待命中...
  kConnecting,    // 连接中...
  kListening,     // 聆听中...
  kSpeaking,      // 说话中...
  kCustom,        // 自定义文本（如激活码，走 g_pending_status 缓冲）
};
static volatile PendingStatus g_pending_status_id = PendingStatus::kNone;
static char g_pending_status[10];             // 仅 kCustom 时使用
static volatile bool g_pending_status_valid = false;
 static QueueHandle_t g_msg_queue = nullptr;
static constexpr size_t kMsgQueueLen = 4;     // 队列深度：满时丢弃新消息并打日志

// UI 待应用数据的统一消费者：表情 / 状态文字 / 聊天消息 / MCP 打开相册
static void chat_ui_apply_timer_cb(lv_timer_t *timer) {
  if (g_pending_status_valid) {
    g_pending_status_valid = false;
    const PendingStatus status = g_pending_status_id;
    g_pending_status_id = PendingStatus::kNone;
    if (screen_loaded && page_head.time_label != nullptr) {
      switch (status) {
        case PendingStatus::kIdle:          lv_label_set_text(page_head.time_label, "空闲中..."); break;
        case PendingStatus::kLoading:       lv_label_set_text(page_head.time_label, "加载中..."); break;
        case PendingStatus::kLoadingFailed: lv_label_set_text(page_head.time_label, "加载失败..."); break;
        case PendingStatus::kStandby:       lv_label_set_text(page_head.time_label, "待命中..."); break;
        case PendingStatus::kConnecting:    lv_label_set_text(page_head.time_label, "连接中..."); break;
        case PendingStatus::kListening:     lv_label_set_text(page_head.time_label, "聆听中..."); break;
        case PendingStatus::kSpeaking:      lv_label_set_text(page_head.time_label, "说话中..."); break;
        case PendingStatus::kCustom:        lv_label_set_text(page_head.time_label, g_pending_status); break;
        case PendingStatus::kNone:          break;
      }
    }
  }
  // 聊天消息队列：一次性排空所有待显示消息（保留最新一条实际生效，
  // 因为 UI 只需要显示最新内容）
  if (g_msg_queue != nullptr) {
    char* msg = nullptr;
    char* latest = nullptr;
    while (xQueueReceive(g_msg_queue, &msg, 0) == pdTRUE) {
      free(latest);  // 前一条被更新覆盖，释放
      latest = msg;
    }
    if (latest != nullptr) {
      if (screen_loaded && guider_ui.screen_robot_label_msg != nullptr) {
        lv_label_set_text(guider_ui.screen_robot_label_msg, latest);
      }
      free(latest);
    }
  }
  if (g_pending_emotion_valid) {
    g_pending_emotion_valid = false;
    SetEmotion(g_pending_emotion);
  }
  if (g_pending_vision_enter) {
    g_pending_vision_enter = false;
    if (lv_scr_act() == guider_ui.screen_robot && guider_ui.screen_robot != nullptr) {
      const char* path = Photo_album::GetInstance().getRandomImagePath();
      if (path != nullptr && path[0] != '\0') {
        g_vision_mode = true;
        // 表情与图片共用机器人屏背景图：识图模式直接换背景
        lv_obj_set_style_bg_img_src(guider_ui.screen_robot, path, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_image_opa(guider_ui.screen_robot, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
      }
    }
  }
  if (g_pending_vision_exit) {
    g_pending_vision_exit = false;
    if (g_vision_mode) {
      g_vision_mode = false;
      g_current_emotion = "";  // 清空去重标记，强制表情重绘
      SetEmotion("happy");
    }
  }
  if (g_pending_next_photo) {
    g_pending_next_photo = false;
    if (g_vision_mode) {
      // 识图模式：换一张随机图片（保持识图模式）
      const char* path = Photo_album::GetInstance().getRandomImagePath();
      if (path != nullptr && path[0] != '\0') {
        lv_obj_set_style_bg_img_src(guider_ui.screen_robot, path, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_image_opa(guider_ui.screen_robot, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
      }
    }
  }
}

// ---- 图片理解（识图）----
// 参考小智官方固件 esp32_camera.cc 的 Explain()：把 JPEG 以 multipart 上传到
// 服务端 MCP initialize 下发的 vision url，服务端调大模型识图后返回文本。
static bool VisionDescribeImage(const std::string& question,
                                const std::string& image_path,
                                std::string& out_answer) {
  const auto vision = ai_vox::Engine::GetInstance().GetVisionConfig();
  if (!vision || vision->url.empty()) {
    out_answer = "识图服务不可用：服务端未下发 vision 配置";
    return false;
  }

  File file = SD_MMC.open(image_path.c_str(), "r");
  if (!file) {
    out_answer = "识图失败：无法打开图片 " + image_path;
    return false;
  }
  const size_t file_size = file.size();
  if (file_size == 0 || file_size > 2 * 1024 * 1024) {
    file.close();
    out_answer = "识图失败：图片大小超出限制(0~2MB)";
    return false;
  }

  uint8_t* jpeg = (uint8_t*)heap_caps_malloc(file_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (jpeg == nullptr) {
    file.close();
    out_answer = "识图失败：内存不足";
    return false;
  }
  const size_t read_len = file.read(jpeg, file_size);
  file.close();
  if (read_len != file_size) {
    free(jpeg);
    out_answer = "识图失败：读取图片不完整";
    return false;
  }

  static const char* kBoundary = "xiaozhi-vision-boundary";
  std::string head;
  head += std::string("--") + kBoundary + "\r\n";
  head += "Content-Disposition: form-data; name=\"question\"\r\n\r\n";
  head += question;
  head += std::string("\r\n--") + kBoundary + "\r\n";
  head += "Content-Disposition: form-data; name=\"file\"; filename=\"image.jpg\"\r\n";
  head += "Content-Type: image/jpeg\r\n\r\n";
  const std::string tail = std::string("\r\n--") + kBoundary + "--\r\n";

  const size_t body_size = head.size() + file_size + tail.size();
  uint8_t* body = (uint8_t*)heap_caps_malloc(body_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (body == nullptr) {
    free(jpeg);
    out_answer = "识图失败：内存不足";
    return false;
  }
  memcpy(body, head.data(), head.size());
  memcpy(body + head.size(), jpeg, file_size);
  memcpy(body + head.size() + file_size, tail.data(), tail.size());
  free(jpeg);

  HTTPClient http;
  http.begin(vision->url.c_str());
  const String device_id = WiFi.macAddress();
  http.addHeader("Device-Id", device_id);
  http.addHeader("Client-Id", device_id);
  if (!vision->token.empty()) {
    http.addHeader("Authorization", String("Bearer ") + vision->token.c_str());
  }
  http.addHeader("Content-Type", String("multipart/form-data; boundary=") + kBoundary);
  http.setTimeout(60000);

  bool ok = false;
  const int http_code = http.POST(body, body_size);
  if (http_code > 0) {
    const String resp = http.getString();
    cJSON* root = cJSON_Parse(resp.c_str());
    if (root != nullptr) {
      const cJSON* success = cJSON_GetObjectItem(root, "success");
      const cJSON* message = cJSON_GetObjectItem(root, "message");
      if (cJSON_IsTrue(success) && message != nullptr && message->valuestring != nullptr) {
        out_answer = message->valuestring;
        ok = true;
      } else if (message != nullptr && message->valuestring != nullptr) {
        out_answer = std::string("识图失败：") + message->valuestring;
      } else {
        out_answer = std::string("识图失败：") + resp.c_str();
      }
      cJSON_Delete(root);
    } else {
      out_answer = std::string("识图失败：响应格式异常 ") + resp.c_str();
    }
  } else {
    out_answer = std::string("识图失败：HTTP ") + http.errorToString(http_code).c_str();
  }
  http.end();
  free(body);
  return ok;
}

// 当前正显示的本地图片 SD 路径（如 /photo/001.jpg）；
// 识图模式返回机器人屏上显示的图片（getRandomImagePath 已把 current_img 指向它）；
// 相册页返回当前图；其余返回空串
static std::string GetShownImagePath() {
  auto& album = Photo_album::GetInstance();
  if (g_vision_mode) {
    const char* name = album.getCurrentImagePath();
    if (name == nullptr || name[0] == '\0') {
      return "";
    }
    return std::string("/photo/") + name;
  }
  if (!album.playLocal) {
    return "";
  }
  const char* name = album.getCurrentImagePath();
  if (name == nullptr || name[0] == '\0') {
    return "";
  }
  return std::string("/photo/") + name;
}

static void RobotWinLongPressCb(lv_event_t *event) {
  if (event == nullptr) {
    return;
  }

  const auto code = lv_event_get_code(event);
  if (code != LV_EVENT_LONG_PRESSED && code != LV_EVENT_LONG_PRESSED_REPEAT) {
    return;
  }

  if (g_current_emotion.empty()) {
    g_current_emotion = "neutral";
  }

  auto it = std::find(kEmotionOrder.begin(), kEmotionOrder.end(), g_current_emotion);
  size_t index = (it == kEmotionOrder.end()) ? 0 : std::distance(kEmotionOrder.begin(), it);
  index = (index + 1) % kEmotionOrder.size();

  SetEmotion(kEmotionOrder[index]);
}

void chat_init() {

  const button_config_t btn_cfg = {
      .long_press_time = 1000,
      .short_press_time = 50,
  };
  const button_gpio_config_t gpio_cfg = {
      .gpio_num = kButtonBoot,
      .active_level = 0,
      .enable_power_save = false,
      .disable_pull = false,
  };
  ESP_ERROR_CHECK(iot_button_new_gpio_device(&btn_cfg, &gpio_cfg, &g_button_boot_handle));
  InitMcpTools();

  auto& ai_vox_engine = ai_vox::Engine::GetInstance();
  ai_vox_engine.SetObserver(g_observer);
  ai_vox_engine.SetOtaUrl("https://api.tenclass.net/xiaozhi/ota/");
  ai_vox_engine.ConfigWebsocket("wss://api.tenclass.net/xiaozhi/v1/",
                                {
                                    {"Authorization", "Bearer test-token"},
                                });

  ai_vox_engine.Start(board.getAudioDeviceEs8311(), board.getAudioDeviceEs8311());

  ESP_ERROR_CHECK(iot_button_register_cb(
      g_button_boot_handle,
      BUTTON_PRESS_DOWN,
      nullptr,
      [](void* button_handle, void* usr_data) {
        printf("boot button pressed\n");
        ai_vox::Engine::GetInstance().Advance();
      },
      nullptr));
   lv_obj_set_style_text_font(page_head.time_label, &font_puhui_16_4, LV_PART_MAIN|LV_STATE_DEFAULT);  //lv_font_source_han_sans_normal_16

   // 长按必须绑定到真正可交互的对象，并允许事件冒泡到父对象，否则容器在某些页面状态下不会触发。
   lv_obj_add_flag(guider_ui.screen_robot, LV_OBJ_FLAG_CLICKABLE);
   lv_obj_add_event_cb(guider_ui.screen_robot, RobotWinLongPressCb, LV_EVENT_LONG_PRESSED, nullptr);
   lv_obj_set_style_text_font(guider_ui.screen_robot_label_msg, &font_puhui_16_4,  LV_PART_MAIN|LV_STATE_DEFAULT);
   SetEmotion("happy");

   // 开机扫描 SD 卡相册（相册页尚未创建，show_image 内部有空指针保护不会显示），
   // 供识图模式随机选图；图片列表常驻（识图模式不随页面进出反复扫描）
   Photo_album::GetInstance().scan_image();

    // 聊天消息队列：先于 UI 定时器与聊天任务创建（生产/消费双方都用到）
    if (g_msg_queue == nullptr) {
      g_msg_queue = xQueueCreate(kMsgQueueLen, sizeof(char*));
    }

    // UI 待应用数据消费者：表情/状态文字/聊天消息/MCP 打开相册（LVGL 线程）
    lv_timer_create(chat_ui_apply_timer_cb, 100, NULL);
    printf("Free INTERNAL heap: %lu\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT));
    printf("Largest INTERNAL free block: %lu\n", heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT));

}

// UI 更新只写待应用缓冲/枚举（由 chat_ui_apply_timer_cb 在 LVGL 线程真正应用），
// 聊天任务内禁止直接调用任何 lv_* 函数
static void set_status_label(PendingStatus status) {
  g_pending_status_id = status;
  g_pending_status_valid = true;
}

// 自定义文本状态（如激活码）
static void set_status_label_custom(const char* text) {
  strncpy(g_pending_status, text, sizeof(g_pending_status) - 1);
  g_pending_status[sizeof(g_pending_status) - 1] = '\0';
  g_pending_status_id = PendingStatus::kCustom;
  g_pending_status_valid = true;
}

static void set_msg_label(const char* text) {
  if (g_msg_queue == nullptr || text == nullptr) {
    return;
  }
  // 生产者（聊天任务）堆上分配字符串，消费者应用后释放
  char* copy = strdup(text);
  if (copy == nullptr) {
    return;  // 内存不足，放弃本条
  }
  if (xQueueSend(g_msg_queue, &copy, 0) != pdTRUE) {
    // 队列满（LVGL 定时器间隔 100ms，正常不会积压）：丢弃并打日志
    printf("[chat] msg queue full, drop: %s\n", text);
    free(copy);
  }
}

void chat_loop() {

  auto& engine = ai_vox::Engine::GetInstance();
  const auto events = g_observer->PopEvents();

  for (auto& event : events) {
    if (auto text_received_event = std::get_if<ai_vox::TextReceivedEvent>(&event)) {
      printf("on text received: %s\n", text_received_event->content.c_str());
    } else if (auto activation_event = std::get_if<ai_vox::ActivationEvent>(&event)) {
      printf("activation code: %s, message: %s\n", activation_event->code.c_str(), activation_event->message.c_str());
      set_status_label_custom(activation_event->code.c_str());
    } else if (auto state_changed_event = std::get_if<ai_vox::StateChangedEvent>(&event)) {
      printf("state changed from %" PRIu8 " to %" PRIu8 "\n",
             static_cast<uint8_t>(state_changed_event->old_state),
             static_cast<uint8_t>(state_changed_event->new_state));
      switch (state_changed_event->new_state) {
        case ai_vox::ChatState::kIdle: {
          printf("Idle\n");
          set_status_label(PendingStatus::kIdle);
          break;
        }
        case ai_vox::ChatState::kInitted: {
          printf("kInitted\n");
          break;
        }
        case ai_vox::ChatState::kLoading: {
          printf("Loading...\n");
          set_status_label(PendingStatus::kLoading);
          break;
        }
        case ai_vox::ChatState::kLoadingFailed: {
          printf("Loading failed, please retry\n");
          set_status_label(PendingStatus::kLoadingFailed);
          break;
        }
        case ai_vox::ChatState::kStandby: {
          printf("Standby\n");
          set_status_label(PendingStatus::kStandby);
          break;
        }
        case ai_vox::ChatState::kConnecting: {
          printf("Connecting...\n");
          set_status_label(PendingStatus::kConnecting);
          break;
        }
        case ai_vox::ChatState::kListening: {
          printf("Listening...\n");
          set_status_label(PendingStatus::kListening);
          break;
        }
        case ai_vox::ChatState::kSpeaking: {
          printf("Speaking...\n");
          set_status_label(PendingStatus::kSpeaking);
          break;
        }
        default: {
          break;
        }
      }
    } else if (auto emotion_event = std::get_if<ai_vox::EmotionEvent>(&event)) {
      // 不在 LVGL 线程外操作 UI：只记录待切换表情，由 emotion_switch_timer_cb 应用
      g_pending_emotion_valid = false;
      strncpy(g_pending_emotion, emotion_event->emotion.c_str(), sizeof(g_pending_emotion) - 1);
      g_pending_emotion[sizeof(g_pending_emotion) - 1] = '\0';
      g_pending_emotion_valid = true;
    } else if (auto chat_message_event = std::get_if<ai_vox::ChatMessageEvent>(&event)) {
      switch (chat_message_event->role) {
        case ai_vox::ChatRole::kAssistant: {
          set_msg_label(chat_message_event->content.c_str());
          break;
        }
        case ai_vox::ChatRole::kUser: {
          set_msg_label(chat_message_event->content.c_str());
          break;
        }
      }
    } else if (auto mcp_tool_call_event = std::get_if<ai_vox::McpToolCallEvent>(&event)) {
      if ("self.audio_speaker.set_volume" == mcp_tool_call_event->name) {
        const auto volume_ptr = mcp_tool_call_event->param<int64_t>("volume");
        if (volume_ptr != nullptr) {
          auto& setting = Setting::GetInstance();
          uint8_t vol = static_cast<uint8_t>(std::max(0, std::min(100, (int)*volume_ptr)));
          volume_value_change(vol);
          engine.SendMcpCallResponse(mcp_tool_call_event->id, true);
        } else {
          engine.SendMcpCallError(mcp_tool_call_event->id, "Missing valid argument: volume");
        }
      } else if ("self.audio_speaker.get_volume" == mcp_tool_call_event->name) {
        auto& setting = Setting::GetInstance();
        uint16_t volume = setting.GetVolume();
        engine.SendMcpCallResponse(mcp_tool_call_event->id, volume);
      } else if ("self.led.set" == mcp_tool_call_event->name) {
        const auto state_ptr = mcp_tool_call_event->param<bool>("state");
        if (state_ptr != nullptr) {
          digitalWrite(kLedPin, *state_ptr);
          engine.SendMcpCallResponse(mcp_tool_call_event->id, true);
        } else {
          engine.SendMcpCallError(mcp_tool_call_event->id, "Missing valid argument: state");
        }
      } else if ("self.led.get" == mcp_tool_call_event->name) {
        const auto state = digitalRead(kLedPin) == HIGH;
        engine.SendMcpCallResponse(mcp_tool_call_event->id, state);
      } else if ("self.screen.set_brightness" == mcp_tool_call_event->name) {
        const auto brightness_ptr = mcp_tool_call_event->param<int64_t>("brightness");
        if (brightness_ptr != nullptr) {
          int percent = std::max(0, std::min(100, (int)*brightness_ptr));
          auto& setting = Setting::GetInstance();
          sun_value_change(percent);
          engine.SendMcpCallResponse(mcp_tool_call_event->id, true);
        } else {
          engine.SendMcpCallError(mcp_tool_call_event->id, "Missing valid argument: brightness");
        }
      } else if ("self.screen.get_brightness" == mcp_tool_call_event->name) {
        auto& setting = Setting::GetInstance();
        int percent = setting.GetScreenBrightness() * 100 / 255;
        engine.SendMcpCallResponse(mcp_tool_call_event->id, percent);
      } else if ("self.screen.set_timeout" == mcp_tool_call_event->name) {
        const auto minutes_ptr = mcp_tool_call_event->param<int64_t>("minutes");
        if (minutes_ptr != nullptr) {
          int minutes = static_cast<int>(*minutes_ptr);
          time_value_change(minutes);
          engine.SendMcpCallResponse(mcp_tool_call_event->id, true);
        } else {
          engine.SendMcpCallError(mcp_tool_call_event->id, "Missing valid argument: minutes");
        }
      } else if ("self.screen.get_timeout" == mcp_tool_call_event->name) {
        auto& setting = Setting::GetInstance();
        int minutes = static_cast<int>(setting.GetScreenTimeout() / 60000);
        engine.SendMcpCallResponse(mcp_tool_call_event->id, minutes);
      } else if ("self.photo_album.open" == mcp_tool_call_event->name) {
        // 进入识图模式（不切屏）：机器人屏背景换成随机图片，表情隐藏
        if (Photo_album::GetInstance().getImageFilesSize() == 0) {
          engine.SendMcpCallError(mcp_tool_call_event->id, "相册为空，SD 卡上没有图片");
        } else {
          engine.SendMcpCallResponse(mcp_tool_call_event->id, true);
          g_pending_vision_enter = true;
        }
      } else if ("self.photo_album.close" == mcp_tool_call_event->name) {
        // 退出识图模式：恢复进入前的表情显示
        engine.SendMcpCallResponse(mcp_tool_call_event->id, true);
        g_pending_vision_exit = true;
      } else if ("self.photo_album.next_image" == mcp_tool_call_event->name) {
        // 先回结果，再由 chat_ui_apply_timer_cb 在 LVGL 线程切图（show_image 会动 LVGL 对象）
        engine.SendMcpCallResponse(mcp_tool_call_event->id, true);
        g_pending_next_photo = true;
      } else if ("self.photo_album.describe_image" == mcp_tool_call_event->name) {
        const auto question_ptr = mcp_tool_call_event->param<std::string>("question");
        std::string question =
            (question_ptr != nullptr && !question_ptr->empty()) ? *question_ptr : "请描述这张图片的内容";
        std::string answer;
        const std::string image_path = GetShownImagePath();
        if (image_path.empty()) {
          engine.SendMcpCallError(mcp_tool_call_event->id,
                                  "当前没有显示本地图片（相册为空或为网络图片），无法识图");
        } else if (VisionDescribeImage(question, image_path, answer)) {
          engine.SendMcpCallResponse(mcp_tool_call_event->id, answer);
        } else {
          engine.SendMcpCallError(mcp_tool_call_event->id, answer);
        }
      }
    }
  }
  vTaskDelay(pdMS_TO_TICKS(10));
}

void xiaozhi_chat_task(void *pvParam){
  while(1) {
     chat_loop();
  }
}

void robot_win_load(SCREEN_LOAD_STAUS loadstatus){
   if(loadstatus == LOADED){
        if(WiFi.status() != WL_CONNECTED){
          show_toast(lv_scr_act(), "WIFI未连接!");
          return;
        }
        screen_loaded = true;
        if(!has_initial){
            chat_init();
            has_initial = true;
        }
        // 任务常驻，只在首次进入时创建，避免重复进入页面叠加多个任务
        if (g_chat_task_handle == nullptr) {
          const BaseType_t xCoreID = 1;
          const BaseType_t ret = xTaskCreatePinnedToCore(
                          xiaozhi_chat_task,
                          "xiaozhi_chat_task",
                          1024 * 20,
                          NULL,
                          2 | portPRIVILEGE_BIT,
                          &g_chat_task_handle,
                          xCoreID
            );
          if (ret != pdPASS) {
            g_chat_task_handle = nullptr;  // 创建失败允许下次重试
          }
        }
   }else{
        screen_loaded = false;
        // 离开机器人页：屏幕对象即将销毁/重建，识图模式显示状态失效，复位防止下次进页卡在"表情不显示"
        g_vision_mode = false;
   }
}

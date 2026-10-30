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
#include "src/generated/gui_guider.h"
static lv_obj_t *chat_label;

//#define MAX_MESSAGES 5

// LV_FONT_DECLARE(font_awesome_30_4);
// // 定义消息类型
// typedef enum {
//     MSG_TYPE_OTHER, // 对方（左侧白色气泡）
//     MSG_TYPE_SELF   // 自己（右侧绿色气泡）
// } msg_type_t;

// // 【关键】定义全局或静态样式变量，避免重复创建导致内存泄漏
// static lv_style_t style_bubble_base;
// static lv_style_t style_bubble_other;
// static lv_style_t style_bubble_self;
// static bool styles_initialized = false;
 static bool screen_loaded = false;
 static bool has_initial = false;
 static std::string g_current_emotion = "";

static const std::vector<std::string> kEmotionOrder = {
    "neutral", "happy", "laughing", "funny", "sad", "angry", "crying",
    "loving", "embarrassed", "surprised", "shocked", "thinking", "winking",
    "cool", "relaxed", "delicious", "kissy", "confident", "sleepy", "silly",
    "confused"
};   




// /**
//  * @brief 初始化聊天泡泡样式 (只需调用一次)
//  */
// void init_chat_styles(void) {
//     if (styles_initialized) return;

//     // 1. 基础气泡样式 (内边距、圆角、无边框)
//     lv_style_init(&style_bubble_base);
//     lv_style_set_radius(&style_bubble_base, 8);
//     lv_style_set_border_width(&style_bubble_base, 0);
//     lv_style_set_pad_all(&style_bubble_base, 8);      // 文字距离气泡边缘的距离
//     lv_style_set_text_font(&style_bubble_base, &font_puhui_16_4); // 替换为你自己的中文字体
//     lv_style_set_bg_opa(&style_bubble_base, LV_OPA_COVER);

//     // 2. 对方气泡颜色 (白色)
//     lv_style_init(&style_bubble_other);
//     lv_style_set_bg_color(&style_bubble_other, lv_color_hex(0xF5DFA6));

//     // 3. 自己气泡颜色 (微信绿 #95EC69)
//     lv_style_init(&style_bubble_self);
//     lv_style_set_bg_color(&style_bubble_self, lv_color_hex(0x95EC69));

//     styles_initialized = true;
// }

// /**
//  * @brief 添加一条聊天消息 (LVGL 8.3.6 标准写法)
//  */
// void add_chat_message(lv_obj_t *parent, const char *text, msg_type_t type) {
//     // 确保样式已初始化
//     init_chat_styles();

//       // 检查消息数量是否超过限制
//    uint32_t child_count = lv_obj_get_child_cnt(parent);
//    if (child_count >= MAX_MESSAGES) {
//     // 删除最早的消息（第一个子对象）
//     lv_obj_t* first_child = lv_obj_get_child(parent, 0);
//     //lv_obj_t* last_child = lv_obj_get_child(parent, child_count - 1);
//     if (first_child != nullptr) {
//       lv_obj_del(first_child);
//     }
//     // Scroll to the last message immediately
//     //if (last_child != nullptr) {
//     //  lv_obj_scroll_to_view_recursive(last_child, LV_ANIM_OFF);
//     //}
//   }

//     // --- 1. 创建消息行容器 ---
//     lv_obj_t * msg_row = lv_obj_create(parent);
//     if (!msg_row) return; // 内存分配失败保护
//     lv_obj_remove_style_all(msg_row); // 清除默认容器的阴影和边框
//     lv_obj_set_size(msg_row, LV_PCT(100), LV_SIZE_CONTENT);
    
//     // 设置 Flex 布局：水平排列，垂直居中
//     lv_obj_set_flex_flow(msg_row, LV_FLEX_FLOW_ROW);
//     lv_obj_set_flex_align(msg_row, 
//         type == MSG_TYPE_OTHER ? LV_FLEX_ALIGN_START : LV_FLEX_ALIGN_END, // 主轴对齐
//         LV_FLEX_ALIGN_CENTER, // 交叉轴对齐
//         LV_FLEX_ALIGN_CENTER  // 追踪轴对齐
//     );
//     lv_obj_set_style_pad_bottom(msg_row, 10, 0); // 消息之间的间距

//     // --- 3. 创建气泡容器并应用样式 ---
//     lv_obj_t *bubble = lv_obj_create(msg_row);
//     if (!bubble) {
//         lv_obj_del(msg_row); // 清理半成品的 row
//         return;
//     }
//     lv_obj_remove_style_all(bubble); // 必须清除默认样式，否则会有灰色背景和边框
    
//     // 添加基础样式
//     lv_obj_add_style(bubble, &style_bubble_base, 0);
//     // 根据类型添加颜色样式
//     lv_obj_add_style(bubble, type == MSG_TYPE_OTHER ? &style_bubble_other : &style_bubble_self, 0);
    
//     // 限制气泡最大宽度，防止长文本占满整行
//     lv_obj_set_width(bubble, LV_PCT(70)); 
//     lv_obj_set_height(bubble, LV_SIZE_CONTENT); 

//     // --- 4. 添加文字标签 ---
//     lv_obj_t *label = lv_label_create(bubble);
//     if (!label) {
//         lv_obj_del(bubble);
//         lv_obj_del(msg_row);
//         return;
//     }
//     lv_label_set_text(label, text);
//     lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP); // 自动换行
//     lv_obj_set_width(label, lv_pct(100));              // 撑满气泡宽度以触发换行

//     // --- 6. 自动滚动到底部 ---
//     lv_obj_scroll_to_view_recursive(msg_row, LV_ANIM_ON);
  
// }

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
}
}

void SetEmotion(const std::string& emotion) {
  if (guider_ui.screen_robot == nullptr) {
    return;
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

   //lv_obj_add_flag(guider_ui.screen_robot, LV_OBJ_FLAG_CLICKABLE);
   //lv_obj_add_flag(guider_ui.screen_robot, LV_OBJ_FLAG_GESTURE_BUBBLE);
   //lv_obj_add_event_cb(guider_ui.screen_robot, RobotWinLongPressCb, LV_EVENT_LONG_PRESSED, nullptr);

   // 初始化聊天列表容器时
  //lv_obj_set_flex_grow(guider_ui.screen_robot_cont_msg, 1);
  //lv_obj_set_flex_flow(guider_ui.screen_robot_cont_msg, LV_FLEX_FLOW_COLUMN); // 垂直排列
  //lv_obj_set_flex_align(guider_ui.screen_robot_cont_msg,LV_FLEX_ALIGN_START,LV_FLEX_ALIGN_START,LV_FLEX_ALIGN_START);
  //lv_obj_set_scrollbar_mode(guider_ui.screen_robot_cont_msg, LV_SCROLLBAR_MODE_OFF);
  //lv_obj_set_scroll_dir(guider_ui.screen_robot_cont_msg, LV_DIR_VER); // 允许垂直滚动      
  //emoji
  //lv_img_set_src(guider_ui.screen_robot_img_emoji, &_neutral_alpha_32x32);
  /* ---- 创建 Toast 容器 ---- */

    SetEmotion("sleepy");

}

void chat_loop() {

  auto& engine = ai_vox::Engine::GetInstance();
  const auto events = g_observer->PopEvents();

  for (auto& event : events) {
    if (auto text_received_event = std::get_if<ai_vox::TextReceivedEvent>(&event)) {
      printf("on text received: %s\n", text_received_event->content.c_str());
    } else if (auto activation_event = std::get_if<ai_vox::ActivationEvent>(&event)) {
      printf("activation code: %s, message: %s\n", activation_event->code.c_str(), activation_event->message.c_str());
      lv_label_set_text(page_head.time_label,activation_event->code.c_str());
    } else if (auto state_changed_event = std::get_if<ai_vox::StateChangedEvent>(&event)) {
      printf("state changed from %" PRIu8 " to %" PRIu8 "\n",
             static_cast<uint8_t>(state_changed_event->old_state),
             static_cast<uint8_t>(state_changed_event->new_state));
      switch (state_changed_event->new_state) {
        case ai_vox::ChatState::kIdle: {
          printf("Idle\n");
          lv_label_set_text(page_head.time_label,"空闲中...");
          break;
        }
        case ai_vox::ChatState::kInitted: {
          printf("kInitted\n");
        
          break;
        }
        case ai_vox::ChatState::kLoading: {
          printf("Loading...\n");
          lv_label_set_text(page_head.time_label,"加载中...");
          break;
        }
        case ai_vox::ChatState::kLoadingFailed: {
          printf("Loading failed, please retry\n");
          lv_label_set_text(page_head.time_label,"加载失败...");
          break;
        }
        case ai_vox::ChatState::kStandby: {
          printf("Standby\n");
          lv_label_set_text(page_head.time_label,"待命中...");
          break;
        }
        case ai_vox::ChatState::kConnecting: {
          printf("Connecting...\n");
          lv_label_set_text(page_head.time_label,"连接中...");
          break;
        }
        case ai_vox::ChatState::kListening: {
          printf("Listening...\n");
          lv_label_set_text(page_head.time_label,"聆听中...");
          break;
        }
        case ai_vox::ChatState::kSpeaking: {
          printf("Speaking...\n");
          lv_label_set_text(page_head.time_label,"说话中...");
          break;
        }
        default: {
          break;
        }
      }
    } else if (auto emotion_event = std::get_if<ai_vox::EmotionEvent>(&event)) {
      printf("emotion: %s\n", emotion_event->emotion.c_str());
      SetEmotion(emotion_event->emotion.c_str());
    } else if (auto chat_message_event = std::get_if<ai_vox::ChatMessageEvent>(&event)) {
      switch (chat_message_event->role) {
        case ai_vox::ChatRole::kAssistant: {
          printf("role: assistant, content: %s\n", chat_message_event->content.c_str());
          lv_label_set_text(guider_ui.screen_robot_label_msg,chat_message_event->content.c_str());
          break;
        }
        case ai_vox::ChatRole::kUser: {
          printf("role: user, content: %s\n", chat_message_event->content.c_str());
          lv_label_set_text(guider_ui.screen_robot_label_msg,chat_message_event->content.c_str());
          break;
        }
      }
    } else if (auto mcp_tool_call_event = std::get_if<ai_vox::McpToolCallEvent>(&event)) {
      printf("on mcp tool call: %s\n", mcp_tool_call_event->ToString().c_str());
      if ("self.audio_speaker.set_volume" == mcp_tool_call_event->name) {
        const auto volume_ptr = mcp_tool_call_event->param<int64_t>("volume");
        if (volume_ptr != nullptr) {
          auto& setting = Setting::GetInstance();
          uint8_t vol = static_cast<uint8_t>(std::max(0, std::min(100, (int)*volume_ptr)));
          volume_value_change(vol);
          printf("on mcp tool call: self.audio_speaker.set_volume, volume: %d\n", vol);
          engine.SendMcpCallResponse(mcp_tool_call_event->id, true);
        } else {
          engine.SendMcpCallError(mcp_tool_call_event->id, "Missing valid argument: volume");
        }
      } else if ("self.audio_speaker.get_volume" == mcp_tool_call_event->name) {
        auto& setting = Setting::GetInstance();
        uint16_t volume = setting.GetVolume();
        printf("on mcp tool call: self.audio_speaker.get_volume, volume: %d\n", volume);
        engine.SendMcpCallResponse(mcp_tool_call_event->id, volume);
      } else if ("self.led.set" == mcp_tool_call_event->name) {
        const auto state_ptr = mcp_tool_call_event->param<bool>("state");
        if (state_ptr != nullptr) {
          printf("on mcp tool call: self.led.set, state: %d\n", *state_ptr);
          digitalWrite(kLedPin, *state_ptr);
          engine.SendMcpCallResponse(mcp_tool_call_event->id, true);
        } else {
          engine.SendMcpCallError(mcp_tool_call_event->id, "Missing valid argument: state");
        }
      } else if ("self.led.get" == mcp_tool_call_event->name) {
        const auto state = digitalRead(kLedPin) == HIGH;
        printf("on mcp tool call: self.led.get, state: %d\n", state);
        engine.SendMcpCallResponse(mcp_tool_call_event->id, state);
      } else if ("self.screen.set_brightness" == mcp_tool_call_event->name) {
        const auto brightness_ptr = mcp_tool_call_event->param<int64_t>("brightness");
        if (brightness_ptr != nullptr) {
          int percent = std::max(0, std::min(100, (int)*brightness_ptr));
          auto& setting = Setting::GetInstance();
          sun_value_change(percent);
          printf("on mcp tool call: self.screen.set_brightness, percent: %d", percent);
          engine.SendMcpCallResponse(mcp_tool_call_event->id, true);
        } else {
          engine.SendMcpCallError(mcp_tool_call_event->id, "Missing valid argument: brightness");
        }
      } else if ("self.screen.get_brightness" == mcp_tool_call_event->name) {
        auto& setting = Setting::GetInstance();
        int percent = setting.GetScreenBrightness() * 100 / 255;
        printf("on mcp tool call: self.screen.get_brightness, percent: %d\n", percent);
        engine.SendMcpCallResponse(mcp_tool_call_event->id, percent);
      } else if ("self.screen.set_timeout" == mcp_tool_call_event->name) {
        const auto minutes_ptr = mcp_tool_call_event->param<int64_t>("minutes");
        if (minutes_ptr != nullptr) {
          int minutes = static_cast<int>(*minutes_ptr);
          time_value_change(minutes);
          printf("on mcp tool call: self.screen.set_timeout, minutes: %d\n", minutes);
          engine.SendMcpCallResponse(mcp_tool_call_event->id, true);
        } else {
          engine.SendMcpCallError(mcp_tool_call_event->id, "Missing valid argument: minutes");
        }
      } else if ("self.screen.get_timeout" == mcp_tool_call_event->name) {
        auto& setting = Setting::GetInstance();
        int minutes = static_cast<int>(setting.GetScreenTimeout() / 60000);
        printf("on mcp tool call: self.screen.get_timeout, minutes: %d\n", minutes);
        engine.SendMcpCallResponse(mcp_tool_call_event->id, minutes);
      }
    }
  }
  vTaskDelay(pdMS_TO_TICKS(10));
}

void xiaozhi_chat_task(void *pvParam){
  while (screen_loaded) 
  {
     chat_loop();
  }
  vTaskDelete(NULL);
}

void robot_win_load(SCREEN_LOAD_STAUS loadstatus){
   if(loadstatus == LOADED){
        screen_loaded = true;
        if(!has_initial){
            chat_init();
            has_initial = true;
        }
        const BaseType_t xCoreID = 1;
        xTaskCreatePinnedToCore(
                        xiaozhi_chat_task,
                        "xiaozhi_chat_task",
                        1024 * 20,
                        NULL,
                        2 | portPRIVILEGE_BIT,
                        NULL,
                        xCoreID
          );    
   }else{
        screen_loaded = false;
   }
}

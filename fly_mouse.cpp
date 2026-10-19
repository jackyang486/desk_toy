#include "fly_mouse.h"
#include "ESP32_board.h"
#include "ai_vox_engine.h"

constexpr uint8_t kQmi8658Address = 0x6B;
constexpr uint8_t kMouseButtonLeft = 1u << 0;
constexpr uint8_t kMouseButtonRight = 1u << 1;
constexpr int kButtonStateRelease = 0;

static lv_obj_t* statusLabel = nullptr;
static lv_obj_t* g_ball_canvas = nullptr;
static lv_draw_rect_dsc_t g_ball_draw_dsc;
static lv_color_t g_canvas_buffer[240 * 240];
static int16_t g_ball_x = 10;
static int16_t g_ball_y = 10;

static const uint8_t kMouseReportMap[] = {
    0x05, 0x01,  // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,  // USAGE (Mouse)
    0xA1, 0x01,  // COLLECTION (Application)
    0x09, 0x01,  // USAGE (Pointer)
    0xA1, 0x00,  // COLLECTION (Physical)
    0x85, 0x01,  // REPORT_ID (1)
    0x05, 0x09,  // USAGE_PAGE (Button)
    0x19, 0x01,  // USAGE_MINIMUM (Button 1)
    0x29, 0x03,  // USAGE_MAXIMUM (Button 3)
    0x15, 0x00,  // LOGICAL_MINIMUM (0)
    0x25, 0x01,  // LOGICAL_MAXIMUM (1)
    0x75, 0x01,  // REPORT_SIZE (1)
    0x95, 0x03,  // REPORT_COUNT (3)
    0x81, 0x02,  // INPUT (Data,Var,Abs)
    0x75, 0x05,  // REPORT_SIZE (5)
    0x95, 0x01,  // REPORT_COUNT (1)
    0x81, 0x01,  // INPUT (Cnst,Var,Abs)
    0x05, 0x01,  // USAGE_PAGE (Generic Desktop)
    0x09, 0x30,  // USAGE (X)
    0x09, 0x31,  // USAGE (Y)
    0x09, 0x38,  // USAGE (Wheel)
    0x15, 0x81,  // LOGICAL_MINIMUM (-127)
    0x25, 0x7F,  // LOGICAL_MAXIMUM (127)
    0x75, 0x08,  // REPORT_SIZE (8)
    0x95, 0x03,  // REPORT_COUNT (3)
    0x81, 0x06,  // INPUT (Data,Var,Rel)
    0xC0,        // END_COLLECTION
    0xC0         // END_COLLECTION
};

lv_timer_t *g_motion_timer = nullptr;

QMI8658 g_imu;
NimBLEHIDDevice *g_hid = nullptr;
NimBLECharacteristic *g_mouse_report = nullptr;

bool g_left_pressed = false;
bool g_right_pressed = false;
bool g_initialized = false;

void fly_mouse_timer_cb(lv_timer_t *timer) {
  (void)timer;
  fly_mouse_loop();
}

void SendMouseReport(int8_t dx, int8_t dy, int8_t wheel) {
  if (g_mouse_report == nullptr) {
    return;
  }

  uint8_t buttons = 0;
  if (g_left_pressed) {
    buttons |= kMouseButtonLeft;
  }
  if (g_right_pressed) {
    buttons |= kMouseButtonRight;
  }

  uint8_t report[4] = {buttons, static_cast<uint8_t>(dx), static_cast<uint8_t>(dy), static_cast<uint8_t>(wheel)};
  g_mouse_report->setValue(report, sizeof(report));
  g_mouse_report->notify();
}

void HandleButtonEvent(lv_event_t *event) {
  if (event == nullptr) {
    return;
  }

  auto *target = lv_event_get_target(event);
  const auto code = lv_event_get_code(event);

  if (target == guider_ui.screen_mouse_btn_left) {
    if (code == LV_EVENT_PRESSED || code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_CLICKED) {
      g_left_pressed = true;
      g_right_pressed = false;
      lv_label_set_text(statusLabel, "left press");
    } else if (code == LV_EVENT_RELEASED) {
      g_left_pressed = false;
      g_right_pressed = false;
      lv_label_set_text(statusLabel, "left release");
    }
    SendMouseReport(0, 0, 0);
  } else if (target == guider_ui.screen_mouse_btn_right) {
    if (code == LV_EVENT_PRESSED || code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_CLICKED) {
      g_right_pressed = true;
      g_left_pressed = false;
      lv_label_set_text(statusLabel, "right press");
    } else if (code == LV_EVENT_RELEASED) {
      g_right_pressed = false;
      g_left_pressed = false;
      lv_label_set_text(statusLabel, "right release");
    }
    SendMouseReport(0, 0, 0);
  }
}

void CreateMouseButtons() {

  //左边按钮绑定事件
  lv_obj_add_event_cb(guider_ui.screen_mouse_btn_left, HandleButtonEvent, LV_EVENT_PRESSED, nullptr);
  lv_obj_add_event_cb(guider_ui.screen_mouse_btn_left, HandleButtonEvent, LV_EVENT_SHORT_CLICKED, nullptr);
  lv_obj_add_event_cb(guider_ui.screen_mouse_btn_left, HandleButtonEvent, LV_EVENT_RELEASED, nullptr);

  //右边按钮绑定事件
  lv_obj_add_event_cb(guider_ui.screen_mouse_btn_right, HandleButtonEvent, LV_EVENT_PRESSED, nullptr);
  lv_obj_add_event_cb(guider_ui.screen_mouse_btn_right, HandleButtonEvent, LV_EVENT_SHORT_CLICKED, nullptr);
  lv_obj_add_event_cb(guider_ui.screen_mouse_btn_right, HandleButtonEvent, LV_EVENT_RELEASED, nullptr);

}

void InitImu() {

  calData cal = {};
  cal.valid = false;

  const int ret = g_imu.init(board.get_I2C_master_bus_handle(),cal, kQmi8658Address);
  if (ret != 0) {
    lv_label_set_text(statusLabel, "QMI8658 init failed");
    return;
  }

  g_imu.setAccelRange(2);
  g_imu.setGyroRange(250);
  g_imu.setAccelODR(500);
  g_imu.setGyroODR(500);
  lv_label_set_text(statusLabel, "QMI8658 init ok");
}

void InitBleMouse() {
  if (g_hid != nullptr) {
    return;
  }

  NimBLEDevice::init("IMU_bluetooth");
  NimBLEServer *server = NimBLEDevice::createServer();
  g_hid = new NimBLEHIDDevice(server);
  g_mouse_report = g_hid->getInputReport(1);

  g_hid->setManufacturer("DeskToy");
  g_hid->setPnp(0x01, 0x02, 0x03, 0x04);
  g_hid->setHidInfo(0x00, 0x01);
  g_hid->setReportMap((uint8_t *)kMouseReportMap, sizeof(kMouseReportMap));
  g_hid->startServices();

  NimBLEAdvertising *advertising = NimBLEDevice::getAdvertising();
  advertising->setAppearance(HID_MOUSE);
  advertising->addServiceUUID(g_hid->getHidService()->getUUID());
  advertising->start();

  Serial.println("BLE mouse initialized");
}

// void UpdateBallCanvas(void) {
//   if (g_ball_canvas == nullptr) {
//     return;
//   }

//   lv_canvas_fill_bg(g_ball_canvas, lv_color_make(0xF0, 0xF0, 0xF0), LV_OPA_COVER);
//   lv_canvas_draw_rect(g_ball_canvas, g_ball_x, g_ball_y, 5, 5, &g_ball_draw_dsc);
// }

void fly_mouse_init(void) {
  if (g_initialized) {
    return;
  }

  statusLabel = lv_label_create(lv_scr_act());
  lv_obj_set_pos(statusLabel, 50, 5);
  lv_obj_set_size(statusLabel, 200, 20);
  lv_obj_center(statusLabel);
  lv_obj_set_style_text_font(statusLabel, &font_puhui_16_4, LV_PART_MAIN | LV_STATE_DEFAULT);

  // g_ball_canvas = lv_canvas_create(lv_scr_act());
  // lv_obj_set_pos(g_ball_canvas, 0, 80);
  // lv_obj_set_size(g_ball_canvas, 240, 240);
  // lv_canvas_set_buffer(g_ball_canvas, g_canvas_buffer, 240, 240, LV_IMG_CF_TRUE_COLOR);
  // lv_obj_clear_flag(g_ball_canvas, LV_OBJ_FLAG_SCROLLABLE);
  // lv_canvas_fill_bg(g_ball_canvas, lv_color_make(0xF0, 0xF0, 0xF0), LV_OPA_COVER);

  // lv_draw_rect_dsc_init(&g_ball_draw_dsc);
  // g_ball_draw_dsc.bg_opa = LV_OPA_COVER;
  // g_ball_draw_dsc.bg_color = lv_color_hex(0xFF0000);
  // g_ball_draw_dsc.border_width = 0;
  // g_ball_draw_dsc.radius = LV_RADIUS_CIRCLE;

  // UpdateBallCanvas();

  CreateMouseButtons();
  InitImu();
  InitBleMouse();
  g_initialized = true;
}

void fly_mouse_win_load(SCREEN_LOAD_STAUS loadstatus) {
    if (loadstatus == LOADED) {
        printf("blue tooth inital");
        fly_mouse_init();
        if (g_motion_timer == nullptr) {
        g_motion_timer = lv_timer_create(fly_mouse_timer_cb, 10, nullptr);
      }
    } else {
      if (g_motion_timer != nullptr) {
        lv_timer_del(g_motion_timer);
        g_motion_timer = nullptr;
      }
      g_left_pressed = false;
      g_right_pressed = false;
      g_initialized = false;
   }
}

void fly_mouse_loop(void) {
  if (!g_initialized) {
    return;
  }
  g_imu.update();

  GyroData gyro = {};
  g_imu.getGyro(&gyro);

  const float angle_x = constrain(gyro.gyroY, -200.0f, 200.0f) / 200.0f;
  const float angle_y = constrain(gyro.gyroX, -200.0f, 200.0f) / 200.0f;

  g_ball_x += static_cast<int16_t>(angle_x * 4.0f);
  g_ball_y += static_cast<int16_t>(angle_y * 4.0f);

  if (g_ball_x < 0) {
    g_ball_x = 0;
  } else if (g_ball_x > 240 - 5) {
    g_ball_x = 240 - 5;
  }

  if (g_ball_y < 0) {
    g_ball_y = 0;
  } else if (g_ball_y > 240 - 5) {
    g_ball_y = 240 - 5;
  }

  AccelData accel = {};
  g_imu.getAccel(&accel);

  const float x_axis = constrain(accel.accelX, -2.0f, 2.0f) * 32.0f;
  const float y_axis = constrain(accel.accelY, -2.0f, 2.0f) * 32.0f;

  const int8_t dx = static_cast<int8_t>(x_axis);
  const int8_t dy = static_cast<int8_t>(-y_axis);
  SendMouseReport(dx, dy, 0);
}

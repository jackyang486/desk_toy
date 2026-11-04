/**************************************************************************/
/*!
  @file     FT6336U.cpp
  Author: Atsushi Sasaki (https://github.com/aselectroworks)
  Modified: ESP32 i2c_master driver adaptation
  License: MIT (see LICENSE)
*/
/**************************************************************************/

#include "MY_FT6336U.h"
#include "driver/i2c_master.h"
#include "esp_log.h"

#define I2C_MASTER_FREQ_HZ 400000
#define I2C_MASTER_TIMEOUT_MS 1000

static const char *TAG = "FT6336U";

FT6336U::FT6336U(uint8_t rst_n, uint8_t int_n)
: rst_n(rst_n), int_n(int_n) {
}

#if defined(ARDUINO_ARCH_ESP32)
FT6336U::FT6336U(int8_t sda, int8_t scl, uint8_t rst_n, uint8_t int_n)
: sda(sda), scl(scl), rst_n(rst_n), int_n(int_n)  {
}
#endif

FT6336U::~FT6336U() {
    if (i2c_dev_handle_ != nullptr) {
        i2c_master_bus_rm_device(i2c_dev_handle_);
        i2c_dev_handle_ = nullptr;
    }
}

void FT6336U::begin(void) {
    pinMode(int_n, INPUT);
    pinMode(rst_n, OUTPUT);
    digitalWrite(rst_n, LOW);
    delay(10);
    digitalWrite(rst_n, HIGH);
    delay(500);
}

void FT6336U::begin(i2c_master_bus_handle_t i2c_bus_handle) {
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_ADDR_FT6336U,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };

    esp_err_t err = i2c_master_bus_add_device(i2c_bus_handle, &dev_cfg, &i2c_dev_handle_);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add FT6336U device to I2C bus: %s", esp_err_to_name(err));
        return;
    }

    begin();
}

uint8_t FT6336U::read_device_mode(void) {
    return (readByte(FT6336U_ADDR_DEVICE_MODE) & 0x70) >> 4;
}

void FT6336U::write_device_mode(DEVICE_MODE_Enum mode) {
    writeByte(FT6336U_ADDR_DEVICE_MODE, (mode & 0x07) << 4);
}

uint8_t FT6336U::read_gesture_id(void) {
    return readByte(FT6336U_ADDR_GESTURE_ID);
}

uint8_t FT6336U::read_td_status(void) {
    return readByte(FT6336U_ADDR_TD_STATUS);
}

uint8_t FT6336U::read_touch_number(void) {
    return readByte(FT6336U_ADDR_TD_STATUS) & 0x0F;
}

uint16_t FT6336U::read_touch1_x(void) {
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_TOUCH1_X);
    read_buf[1] = readByte(FT6336U_ADDR_TOUCH1_X + 1);
    return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}

uint16_t FT6336U::read_touch1_y(void) {
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_TOUCH1_Y);
    read_buf[1] = readByte(FT6336U_ADDR_TOUCH1_Y + 1);
    return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}

uint8_t FT6336U::read_touch1_event(void) {
    return readByte(FT6336U_ADDR_TOUCH1_EVENT) >> 6;
}

uint8_t FT6336U::read_touch1_id(void) {
    return readByte(FT6336U_ADDR_TOUCH1_ID) >> 4;
}

uint8_t FT6336U::read_touch1_weight(void) {
    return readByte(FT6336U_ADDR_TOUCH1_WEIGHT);
}

uint8_t FT6336U::read_touch1_misc(void) {
    return readByte(FT6336U_ADDR_TOUCH1_MISC) >> 4;
}

uint16_t FT6336U::read_touch2_x(void) {
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_TOUCH2_X);
    read_buf[1] = readByte(FT6336U_ADDR_TOUCH2_X + 1);
    return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}

uint16_t FT6336U::read_touch2_y(void) {
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_TOUCH2_Y);
    read_buf[1] = readByte(FT6336U_ADDR_TOUCH2_Y + 1);
    return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}

uint8_t FT6336U::read_touch2_event(void) {
    return readByte(FT6336U_ADDR_TOUCH2_EVENT) >> 6;
}

uint8_t FT6336U::read_touch2_id(void) {
    return readByte(FT6336U_ADDR_TOUCH2_ID) >> 4;
}

uint8_t FT6336U::read_touch2_weight(void) {
    return readByte(FT6336U_ADDR_TOUCH2_WEIGHT);
}

uint8_t FT6336U::read_touch2_misc(void) {
    return readByte(FT6336U_ADDR_TOUCH2_MISC) >> 4;
}

uint8_t FT6336U::read_touch_threshold(void) {
    return readByte(FT6336U_ADDR_THRESHOLD);
}

uint8_t FT6336U::read_filter_coefficient(void) {
    return readByte(FT6336U_ADDR_FILTER_COE);
}

uint8_t FT6336U::read_ctrl_mode(void) {
    return readByte(FT6336U_ADDR_CTRL);
}

void FT6336U::write_ctrl_mode(CTRL_MODE_Enum mode) {
    writeByte(FT6336U_ADDR_CTRL, mode);
}

uint8_t FT6336U::read_time_period_enter_monitor(void) {
    return readByte(FT6336U_ADDR_TIME_ENTER_MONITOR);
}

uint8_t FT6336U::read_active_rate(void) {
    return readByte(FT6336U_ADDR_ACTIVE_MODE_RATE);
}

uint8_t FT6336U::read_monitor_rate(void) {
    return readByte(FT6336U_ADDR_MONITOR_MODE_RATE);
}

uint8_t FT6336U::read_radian_value(void) {
    return readByte(FT6336U_ADDR_RADIAN_VALUE);
}

void FT6336U::write_radian_value(uint8_t val) {
    writeByte(FT6336U_ADDR_RADIAN_VALUE, val);
}

uint8_t FT6336U::read_offset_left_right(void) {
    return readByte(FT6336U_ADDR_OFFSET_LEFT_RIGHT);
}

void FT6336U::write_offset_left_right(uint8_t val) {
    writeByte(FT6336U_ADDR_OFFSET_LEFT_RIGHT, val);
}

uint8_t FT6336U::read_offset_up_down(void) {
    return readByte(FT6336U_ADDR_OFFSET_UP_DOWN);
}

void FT6336U::write_offset_up_down(uint8_t val) {
    writeByte(FT6336U_ADDR_OFFSET_UP_DOWN, val);
}

uint8_t FT6336U::read_distance_left_right(void) {
    return readByte(FT6336U_ADDR_DISTANCE_LEFT_RIGHT);
}

void FT6336U::write_distance_left_right(uint8_t val) {
    writeByte(FT6336U_ADDR_DISTANCE_LEFT_RIGHT, val);
}

uint8_t FT6336U::read_distance_up_down(void) {
    return readByte(FT6336U_ADDR_DISTANCE_UP_DOWN);
}

void FT6336U::write_distance_up_down(uint8_t val) {
    writeByte(FT6336U_ADDR_DISTANCE_UP_DOWN, val);
}

uint8_t FT6336U::read_distance_zoom(void) {
    return readByte(FT6336U_ADDR_DISTANCE_ZOOM);
}

void FT6336U::write_distance_zoom(uint8_t val) {
    writeByte(FT6336U_ADDR_DISTANCE_ZOOM, val);
}

uint16_t FT6336U::read_library_version(void) {
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_LIBRARY_VERSION_H);
    read_buf[1] = readByte(FT6336U_ADDR_LIBRARY_VERSION_L);
    return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}

uint8_t FT6336U::read_chip_id(void) {
    return readByte(FT6336U_ADDR_CHIP_ID);
}

uint8_t FT6336U::read_g_mode(void) {
    return readByte(FT6336U_ADDR_G_MODE);
}

void FT6336U::write_g_mode(G_MODE_Enum mode){
    writeByte(FT6336U_ADDR_G_MODE, mode);
}

uint8_t FT6336U::read_pwrmode(void) {
    return readByte(FT6336U_ADDR_POWER_MODE);
}

uint8_t FT6336U::read_firmware_id(void) {
    return readByte(FT6336U_ADDR_FIRMARE_ID);
}

uint8_t FT6336U::read_focaltech_id(void) {
    return readByte(FT6336U_ADDR_FOCALTECH_ID);
}

uint8_t FT6336U::read_release_code_id(void) {
    return readByte(FT6336U_ADDR_RELEASE_CODE_ID);
}

uint8_t FT6336U::read_state(void) {
    return readByte(FT6336U_ADDR_STATE);
}

FT6336U_TouchPointType FT6336U::scan(void){
    // 一次性把 0x02~0x0C（TD_STATUS + 两个触点的坐标块）整段读回来。
    //
    // 原来每个字段单独发起一次 I2C 事务，一旦总线抖动就会拼出"触点数=垃圾值、
    // 坐标=0"的混合数据：坐标 (0,0) 正好落在顶栏 pageHeadCont 上，
    // 于是一次 press+release 就误触发了顶栏点击；随机坐标还可能点到"下一曲"按钮，
    // 表现为歌放到一半突然切歌。整块读 + 严格校验可以消除这种不一致。
    uint8_t buf[11];
    if (!readBlock(FT6336U_ADDR_TD_STATUS, buf, sizeof(buf))) {
        // 读失败时不要编造数据（原来 readByte 失败返回 0，会被当成坐标 0）
        touchPoint.touch_count = 0;
        touchPoint.tp[0].status = release;
        touchPoint.tp[1].status = release;
        return touchPoint;
    }

    // TD_STATUS: bit7:4 恒为 0，bit3:0 为触点数(<=2)
    const uint8_t td = buf[0];
    uint8_t cnt = td & 0x0F;
    if ((td & 0xF0) != 0 || cnt > 2) cnt = 0;

    // 0x03 P1_XH: bit7:6 事件(0=DOWN 1=UP 2=MOVE 3=保留)，bit5:4 ID，bit3:0 X[11:8]
    // 0x05 P1_YH: bit7:4 ID，bit3:0 Y[11:8]
    // 0x09 P2_XH / 0x0B P2_YH 同理
    const uint8_t ev1 = buf[1] >> 6;
    const uint8_t id1 = buf[3] >> 4;
    const uint8_t ev2 = buf[7] >> 6;
    const uint8_t id2 = buf[9] >> 4;

    // 交叉校验：点数说有触点，但事件位为"保留(3)"或 ID 非法，说明数据是坏的
    if (cnt >= 1 && (ev1 == 3 || id1 > 1)) cnt = 0;
    if (cnt == 2 && (ev2 == 3 || id2 > 1 || id2 == id1)) cnt = 1;

    touchPoint.touch_count = cnt;

    if (cnt == 0) {
        touchPoint.tp[0].status = release;
        touchPoint.tp[1].status = release;
        return touchPoint;
    }

    const uint16_t x1 = ((uint16_t)(buf[1] & 0x0F) << 8) | buf[2];
    const uint16_t y1 = ((uint16_t)(buf[3] & 0x0F) << 8) | buf[4];
    touchPoint.tp[id1].status = (touchPoint.tp[id1].status == release) ? touch : stream;
    touchPoint.tp[id1].x = x1;
    touchPoint.tp[id1].y = y1;
    touchPoint.tp[~id1 & 0x01].status = release;

    if (cnt == 2) {
        const uint16_t x2 = ((uint16_t)(buf[7] & 0x0F) << 8) | buf[8];
        const uint16_t y2 = ((uint16_t)(buf[9] & 0x0F) << 8) | buf[10];
        touchPoint.tp[id2].status = (touchPoint.tp[id2].status == release) ? touch : stream;
        touchPoint.tp[id2].x = x2;
        touchPoint.tp[id2].y = y2;
    }

    return touchPoint;
}

// 从 addr 起连续读 len 字节（单次 I2C 事务），任何一步失败都返回 false，
// 调用方据此丢弃整帧数据，而不是拿半截数据继续用。
bool FT6336U::readBlock(uint8_t addr, uint8_t *buf, size_t len) {
    if (i2c_dev_handle_ == nullptr) {
        ESP_LOGE(TAG, "I2C device handle is not initialized");
        return false;
    }
    esp_err_t ret = i2c_master_transmit_receive(i2c_dev_handle_, &addr, 1, buf, len,
                                                pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    if (ret != ESP_OK) {
        // 触摸数据每几十毫秒读一次，偶发失败很常见，用 W 级日志避免刷屏
        ESP_LOGD(TAG, "Read block failed: addr=0x%02X, len=%u, err=%s",
                 addr, (unsigned)len, esp_err_to_name(ret));
        return false;
    }
    return true;
}

uint8_t FT6336U::readByte(uint8_t addr) {
    if (i2c_dev_handle_ == nullptr) {
        ESP_LOGE(TAG, "I2C device handle is not initialized");
        return 0;
    }

    uint8_t rdData = 0;
    esp_err_t ret = i2c_master_transmit_receive(i2c_dev_handle_, &addr, 1, &rdData, 1, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Read byte failed: addr=0x%02X, err=%s", addr, esp_err_to_name(ret));
    }

    return rdData;
}

void FT6336U::writeByte(uint8_t addr, uint8_t data) {
    DEBUG_PRINTLN("")
    DEBUG_PRINT("writeI2C reg 0x")
    DEBUG_PRINT(addr, HEX)
    DEBUG_PRINT(" -> 0x") DEBUG_PRINTLN(data, HEX);

    if (i2c_dev_handle_ == nullptr) {
        ESP_LOGE(TAG, "I2C device handle is not initialized");
        return;
    }

    uint8_t write_buf[2] = {addr, data};
    esp_err_t ret = i2c_master_transmit(i2c_dev_handle_, write_buf, sizeof(write_buf), pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Write byte failed: addr=0x%02X, data=0x%02X, err=%s", addr, data, esp_err_to_name(ret));
    }
}
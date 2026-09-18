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
    touchPoint.touch_count = read_td_status();

    if(touchPoint.touch_count == 0) {
        touchPoint.tp[0].status = release;
        touchPoint.tp[1].status = release;
    }
    else if(touchPoint.touch_count == 1) {
        uint8_t id1 = read_touch1_id();
        touchPoint.tp[id1].status = (touchPoint.tp[id1].status == release) ? touch : stream;
        touchPoint.tp[id1].x = read_touch1_x();
        touchPoint.tp[id1].y = read_touch1_y();
        touchPoint.tp[~id1 & 0x01].status = release;
    }
    else {
        uint8_t id1 = read_touch1_id();
        touchPoint.tp[id1].status = (touchPoint.tp[id1].status == release) ? touch : stream;
        touchPoint.tp[id1].x = read_touch1_x();
        touchPoint.tp[id1].y = read_touch1_y();
        uint8_t id2 = read_touch2_id();
        touchPoint.tp[id2].status = (touchPoint.tp[id2].status == release) ? touch : stream;
        touchPoint.tp[id2].x = read_touch2_x();
        touchPoint.tp[id2].y = read_touch2_y();
    }

    return touchPoint;
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
#ifndef _WEATHER_H_
#define _WEATHER_H_
#include <arduino.h>
#include "src/custom/custom.h"
// NTP 相关已迁移到 main.cpp，入口声明见 wifi_conf.h
void getCityWeater(String cityCode);
void weaterData(String *cityDZ,String *dataSK,String *dataFC);
extern volatile bool time_enable;
extern int Hour_sign;
extern int Minute_sign;
void digitalClockDisplay(time_t nowTime);
void weather_screen_load(SCREEN_LOAD_STAUS status);
#endif
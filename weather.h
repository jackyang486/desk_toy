#ifndef _WEATHER_H_
#define _WEATHER_H_
#include <arduino.h>
void wifi_udp_init(void);
void weather_page_load(bool pageLoad);
void getCityWeater(String cityCode);
void weaterData(String *cityDZ,String *dataSK,String *dataFC);
extern volatile bool time_enable;
void digitalClockDisplay(time_t nowTime);
#endif
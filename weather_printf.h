#ifndef  __WEATHER_PRINTF_H_
#define  __WEATHER_PRINTF_H_
#include "lvgl.h"
#include <Arduino.h>

class WeatherPrintf
{
private:
public:
  //void init();
  void printfW3660(lv_obj_t* lv_obj,int numn);
  void printfO3660(lv_obj_t* lv_obj,int numn);
  void printfW1830(lv_obj_t* lv_obj,int numn);
  void printfweather(lv_obj_t* lv_obj,int numw);
};


#endif

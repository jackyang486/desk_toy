#include "weather.h"
#include <HTTPClient.h>
#include "ArduinoJson.h"
#include <TimeLib.h>
#include <atomic>
#include <WiFi.h>
#include "weather_printf.h"
#include "wifi_conf.h"
#include "src/generated/gui_guider.h"
typedef enum{
 HOUR_TYPE,
 MINUTE_TYPE,
 SECOND_TYPE
} TIME_TYPE;
#define ANIM_DURATION  400
WeatherPrintf weatherPrintf;
WiFiClient wificlient;
void getLocalCityWeather(void);
void slide_switch(lv_obj_t *frontImg,lv_obj_t *backImg,TIME_TYPE time_type,int num);

//时分秒滚动动画
void slide_switch(lv_obj_t *frontImg,lv_obj_t *backImg,TIME_TYPE time_type,int num)
{
    lv_anim_t anim1, anim2;
    lv_image_set_src(frontImg, lv_image_get_src(backImg));
    if(time_type == SECOND_TYPE){
       weatherPrintf.printfW1830(backImg,num);
    }else if(time_type == MINUTE_TYPE){
       weatherPrintf.printfO3660(backImg,num); 
    }else if(time_type == HOUR_TYPE){
       weatherPrintf.printfW3660(backImg,num);
    }
    // -------- 动画1：当前前景图 向上滑出 --------
    lv_anim_init(&anim1);
    lv_anim_set_var(&anim1, frontImg);
    lv_anim_set_exec_cb(&anim1, (lv_anim_exec_xcb_t)lv_obj_set_y);
    if(time_type == SECOND_TYPE){
       lv_anim_set_values(&anim1, 0, -30);
    }else{
       lv_anim_set_values(&anim1, 0, -60);  
    }
    lv_anim_set_time(&anim1, ANIM_DURATION);
    lv_anim_set_path_cb(&anim1, lv_anim_path_linear);
    lv_anim_start(&anim1);

    // -------- 动画2：背景图 从底部滑入 --------
    lv_anim_init(&anim2);
    lv_anim_set_var(&anim2, backImg);
    lv_anim_set_exec_cb(&anim2, (lv_anim_exec_xcb_t)lv_obj_set_y);
    if(time_type == SECOND_TYPE){
       lv_anim_set_values(&anim2, 30, 0);
    }else{
       lv_anim_set_values(&anim2, 60, 0);  
    }
    lv_anim_set_time(&anim2, ANIM_DURATION);
    lv_anim_set_path_cb(&anim2, lv_anim_path_linear);
    lv_anim_start(&anim2);
}

// 获取城市天气
void getCityWeater(String cityCode){
 
 //String URL = "http://d1.weather.com.cn/dingzhi/" + cityCode + ".html?_="+String(now());//新
  String URL = "http://d1.weather.com.cn/weather_index/" + cityCode + ".html?_="+String(now());//原来
  //创建 HTTPClient 对象
  HTTPClient httpClient;
  
  httpClient.begin(wificlient,URL); 
  
  //设置请求头中的User-Agent
  httpClient.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
  httpClient.addHeader("Referer", "http://www.weather.com.cn/");
 
  //启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  
  //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
  if (httpCode == HTTP_CODE_OK) {

    String str = httpClient.getString();
    int indexStart = str.indexOf("weatherinfo\":");
    int indexEnd = str.indexOf("};var alarmDZ");

    String jsonCityDZ = str.substring(indexStart+13,indexEnd);

    indexStart = str.indexOf("dataSK =");
    indexEnd = str.indexOf(";var dataZS");
    String jsonDataSK = str.substring(indexStart+8,indexEnd);
    
    indexStart = str.indexOf("\"f\":[");
    indexEnd = str.indexOf(",{\"fa");
    String jsonFC = str.substring(indexStart+5,indexEnd);
    weaterData(&jsonCityDZ,&jsonDataSK,&jsonFC);
    Serial.println("获取成功");
  } else {
    Serial.println("请求城市天气错误：");
    Serial.print(httpCode);
  }
  //关闭HTTP连接
  httpClient.end();
}

//星期
String week()
{
  String wk[7] = {"日","一","二","三","四","五","六"};
  String s = "周" + wk[weekday()-1];
  return s;
}

//月日
String YearMonthDay()
{
  String s = String(year()); 
  s = s + "年" + month() + "月" + day() + "日";
  return s;
}


int Hour_sign   = 24;
int Minute_sign = 60;
int Second_sign = 60;
void digitalClockDisplay(time_t nowTime)
{ 
  if(hour(nowTime)!=Hour_sign)//时钟刷新
  {
    slide_switch(guider_ui.screen_weather_img_hour_2,guider_ui.screen_weather_img_hour_shi,HOUR_TYPE,hour(nowTime)/10);
    slide_switch(guider_ui.screen_weather_img_hour_1,guider_ui.screen_weather_img_hour_ge,HOUR_TYPE,hour(nowTime)%10);
    Hour_sign = hour(nowTime);
  }
  if(minute(nowTime)!=Minute_sign)//分钟刷新
  {
    slide_switch(guider_ui.screen_weather_img_minute_2,guider_ui.screen_weather_img_minute_shi,MINUTE_TYPE,minute(nowTime)/10);
    slide_switch(guider_ui.screen_weather_img_minute_1,guider_ui.screen_weather_img_minute_ge,MINUTE_TYPE,minute(nowTime)%10);
    Minute_sign = minute(nowTime);
    char time_buff[128];
    sprintf(time_buff, "%d%d:%d%d", 
    hour(nowTime)/10, hour(nowTime)%10,minute(nowTime)/10,minute(nowTime)%10);
    lv_label_set_text(page_head.time_label,time_buff);
  }
  if(second(nowTime)!=Second_sign)//分钟刷新
  {
    slide_switch(guider_ui.screen_weather_img_second_2,guider_ui.screen_weather_img_second_shi,SECOND_TYPE,second(nowTime)/10);
    slide_switch(guider_ui.screen_weather_img_second_1,guider_ui.screen_weather_img_second_ge,SECOND_TYPE,second(nowTime)%10);
    Second_sign = second(nowTime);
    lv_label_set_text(guider_ui.screen_weather_label_year_month,YearMonthDay().c_str());
    lv_label_set_text(guider_ui.screen_weather_label_week,week().c_str());
  }
 
}


//天气信息写到屏幕上
void weaterData(String *cityDZ,String *dataSK,String *dataFC)
{
  //解析第一段JSON
  DynamicJsonDocument skDoc(512);
  DynamicJsonDocument dzDoc(512);
  DynamicJsonDocument fcDoc(512);
  
  deserializeJson(skDoc, *dataSK);
  JsonObject sk = skDoc.as<JsonObject>();
  
  lv_obj_invalidate(guider_ui.screen_weather);

  //设置温度
  int tempnum = sk["temp"].as<int>();
  tempnum = tempnum+10;
  String tempStr = sk["temp"].as<String>()+"℃";
  lv_label_set_text(guider_ui.screen_weather_label_temp,tempStr.c_str());
  lv_bar_set_value(guider_ui.screen_weather_bar_temp,tempnum, LV_ANIM_OFF);

  //设置湿度
  int humidity = atoi((sk["SD"].as<String>()).substring(0,2).c_str());
  lv_bar_set_value(guider_ui.screen_weather_bar_hum,humidity, LV_ANIM_OFF);
  char humBuf[10] = {0};
  sprintf(humBuf,"%d%%",humidity);
  lv_label_set_text(guider_ui.screen_weather_label_hum, humBuf);


  //设置城市
  lv_label_set_text(guider_ui.screen_weather_label_city,sk["cityname"].as<String>().c_str());

  //设置PM2.5空气指数
  String aqiTxt = "优";
  int pm25V = sk["aqi"];
  if(pm25V>200){
    aqiTxt = "重度";
    lv_obj_set_style_bg_color(guider_ui.screen_weather_label_qulaity, lv_color_make(136,11,32), LV_PART_MAIN|LV_STATE_DEFAULT);
  }else if(pm25V>150){
    aqiTxt = "中度";
    lv_obj_set_style_bg_color(guider_ui.screen_weather_label_qulaity, lv_color_make(186,55,121), LV_PART_MAIN|LV_STATE_DEFAULT);
  }else if(pm25V>100){
    aqiTxt = "轻度";
    lv_obj_set_style_bg_color(guider_ui.screen_weather_label_qulaity, lv_color_make(242,159,57), LV_PART_MAIN|LV_STATE_DEFAULT);
  }else if(pm25V>50){
    aqiTxt = "良";
    lv_obj_set_style_bg_color(guider_ui.screen_weather_label_qulaity, lv_color_make(247,219,100), LV_PART_MAIN|LV_STATE_DEFAULT);
  }
  lv_label_set_text(guider_ui.screen_weather_label_qulaity,aqiTxt.c_str());

  //天气图标
  int numw = atoi((sk["weathercode"].as<String>()).substring(1,3).c_str()); 
  weatherPrintf.printfweather(guider_ui.screen_weather_img_weather0,numw);
  //解析第二段JSON
  deserializeJson(dzDoc, *cityDZ);
  JsonObject dz = dzDoc.as<JsonObject>();
  deserializeJson(fcDoc, *dataFC);
  JsonObject fc = fcDoc.as<JsonObject>();

  // 栈上分配或 static 分配，完全不触碰堆内存
  static char weatherBuf[256]; 
  snprintf(weatherBuf, sizeof(weatherBuf),
    " 实时天气:%s 风向:%s%s 今日:%s 最低温度:%s度 最高温度:%s度",
    sk["weather"].as<const char*>(),
    sk["WD"].as<const char*>(),
    sk["WS"].as<const char*>(),
    dz["weather"].as<const char*>(),
    fc["fd"].as<const char*>(),
    fc["fc"].as<const char*>()
  );
  lv_label_set_text(guider_ui.screen_weather_label_weather_banner,weatherBuf);
  lv_label_set_long_mode(guider_ui.screen_weather_label_weather_banner, LV_LABEL_LONG_SCROLL);
  
  lv_obj_invalidate(guider_ui.screen_weather);
}

void weather_screen_load(SCREEN_LOAD_STAUS status){
    if(status == LOADED){ 
       //字体初始化
      lv_label_set_text(guider_ui.screen_weather_label_week,"");
      lv_label_set_text(guider_ui.screen_weather_label_year_month,"");
      lv_obj_set_style_text_font(guider_ui.screen_weather_label_weather_banner, &font_puhui_16_4,  LV_PART_MAIN|LV_STATE_DEFAULT);
      lv_obj_set_style_text_font(guider_ui.screen_weather_label_city, &font_puhui_16_4, LV_PART_MAIN|LV_STATE_DEFAULT);
      lv_obj_set_style_text_font(guider_ui.screen_weather_label_qulaity, &font_puhui_16_4, LV_PART_MAIN|LV_STATE_DEFAULT);
      if(WiFi.status() == WL_CONNECTED){  
        getCityWeater("101250101");
      }
    }
}


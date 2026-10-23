#include "weather.h"
#include <HTTPClient.h>
#include "ArduinoJson.h"
#include <TimeLib.h>
#include <atomic>
#include "src/custom/custom.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include "weather_printf.h"
#include "wifi_conf.h"
#include "src/generated/gui_guider.h"
//NTP服务器
static const char ntpServerName[] = "ntp6.aliyun.com";
const int timeZone = 8;     //东八区
typedef enum{
 HOUR_TYPE,
 MINUTE_TYPE,
 SECOND_TYPE
} TIME_TYPE;
#define ANIM_DURATION  400
WeatherPrintf weatherPrintf;
WiFiClient wificlient;
WiFiUDP Udp;
volatile bool time_enable = false;
time_t getNtpTime(void);
void sendNTPpacket(IPAddress &address);
void getLocalCityWeather(void);
void slide_switch(lv_obj_t *frontImg,lv_obj_t *backImg,TIME_TYPE time_type,int num);

void wifi_udp_init(){
  uint8_t localPort = 8000;
  Serial.println("启动UDP");
  Udp.begin(localPort);
  Serial.println("等待同步...");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  //字体初始化
  lv_obj_set_style_text_font(guider_ui.screen_weather_label_weather_banner, &font_puhui_16_4,  LV_PART_MAIN|LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(guider_ui.screen_weather_label_city, &font_puhui_16_4, LV_PART_MAIN|LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(guider_ui.screen_weather_label_qulaity, &font_puhui_16_4, LV_PART_MAIN|LV_STATE_DEFAULT);
  vTaskDelay(pdMS_TO_TICKS(500));    
}
/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP时间在消息的前48字节中
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  WiFi.hostByName(ntpServerName, ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      //Serial.println(secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR);
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // 无法获取时间时返回0
}

// 向NTP服务器发送请求
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
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


//每5分钟天气刷新一次
void data_fresh_task(void *pvParam){
  vTaskDelay(pdMS_TO_TICKS(500));
  auto& wifiConfig = wifi_config::GetInstance(); 
  while(time_enable && WiFi.status() == WL_CONNECTED){  
    getCityWeater("101250101");
    vTaskDelay(pdMS_TO_TICKS(300000));
  }
  // 退出任务
  vTaskDelete(NULL);
}

//天气每5分钟刷新任务
void weather_page_load(bool pageLoad){
  if(pageLoad){
      time_enable = true;
      xTaskCreate(data_fresh_task, "data_fresh_task", 4096,NULL, 1, NULL);
  }else{
      time_enable= false;
      Udp.flush();
      Udp.clear();
      Udp.stop();
  }
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


int Hour_sign   = 60;
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
  }
  if(second(nowTime)!=Second_sign)//分钟刷新
  {
    slide_switch(guider_ui.screen_weather_img_second_2,guider_ui.screen_weather_img_second_shi,SECOND_TYPE,second(nowTime)/10);
    slide_switch(guider_ui.screen_weather_img_second_1,guider_ui.screen_weather_img_second_ge,SECOND_TYPE,second(nowTime)%10);
    Second_sign = second(nowTime);
  }
  lv_label_set_text(guider_ui.screen_weather_label_year_month,YearMonthDay().c_str());
  lv_label_set_text(guider_ui.screen_weather_label_week,week().c_str());
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


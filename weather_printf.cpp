#include "weather_printf.h"
#include "src/generated/gui_guider.h"

//显示白色36*60大小数字
void WeatherPrintf::printfW3660(lv_obj_t* lv_obj,int numn)
{
  switch(numn)
  {
    case 0:
      lv_img_set_src(lv_obj, &_W_3660_i0_alpha_36x60);
      break;
    case 1:
      lv_img_set_src(lv_obj, &_W_3660_i1_alpha_36x60);
      break;
    case 2:
      lv_img_set_src(lv_obj, &_W_3660_i2_alpha_36x60);
      break;
    case 3:
      lv_img_set_src(lv_obj, &_W_3660_i3_alpha_36x60);
      break;
    case 4:
      lv_img_set_src(lv_obj, &_W_3660_i4_alpha_36x60);
      break;
    case 5:
      lv_img_set_src(lv_obj, &_W_3660_i5_alpha_36x60);
      break;
    case 6:
      lv_img_set_src(lv_obj, &_W_3660_i6_alpha_36x60);
      break;
    case 7:
      lv_img_set_src(lv_obj, &_W_3660_i7_alpha_36x60);
      break;
    case 8:
      lv_img_set_src(lv_obj, &_W_3660_i8_alpha_36x60);
      break;
    case 9:
      lv_img_set_src(lv_obj, &_W_3660_i9_alpha_36x60);
      break;
    default:
      Serial.println("显示W_3660数字错误");
      break;
  }
}
//显示橙色36*60大小数字
void WeatherPrintf::printfO3660(lv_obj_t* lv_obj,int numn)
{
  switch(numn)
  {
    case 0:
      lv_img_set_src(lv_obj, &_O_3660_i0_alpha_36x60);
      break;
    case 1:
      lv_img_set_src(lv_obj, &_O_3660_i1_alpha_36x60);
      break;
    case 2:
      lv_img_set_src(lv_obj, &_O_3660_i2_alpha_36x60);
      break;
    case 3:
      lv_img_set_src(lv_obj, &_O_3660_i3_alpha_36x60);
      break;
    case 4:
      lv_img_set_src(lv_obj, &_O_3660_i4_alpha_36x60);
      break;
    case 5:
      lv_img_set_src(lv_obj, &_O_3660_i5_alpha_36x60);
      break;
    case 6:
      lv_img_set_src(lv_obj, &_O_3660_i6_alpha_36x60);
      break;
    case 7:
      lv_img_set_src(lv_obj, &_O_3660_i7_alpha_36x60);
      break;
    case 8:
      lv_img_set_src(lv_obj, &_O_3660_i8_alpha_36x60);
      break;
    case 9:
      lv_img_set_src(lv_obj, &_O_3660_i9_alpha_36x60);
      break;
    default:
      Serial.println("显示O_3660数字错误");
      break;
  }
}
//显示白色18*30大小数字
void WeatherPrintf::printfW1830(lv_obj_t* lv_obj,int numn)
{
  switch(numn)
  {
    case 0:
      lv_img_set_src(lv_obj, &_W_1830_i0_alpha_18x30);
      break;
    case 1:
      lv_img_set_src(lv_obj, &_W_1830_i1_alpha_18x30);
      break;
    case 2:
      lv_img_set_src(lv_obj, &_W_1830_i2_alpha_18x30);
      break;
    case 3:
      lv_img_set_src(lv_obj, &_W_1830_i3_alpha_18x30);
      break;
    case 4:
      lv_img_set_src(lv_obj, &_W_1830_i4_alpha_18x30);
      break;
    case 5:
      lv_img_set_src(lv_obj, &_W_1830_i5_alpha_18x30);
      break;
    case 6:
      lv_img_set_src(lv_obj, &_W_1830_i6_alpha_18x30);
      break;
    case 7:
      lv_img_set_src(lv_obj, &_W_1830_i7_alpha_18x30);
      break;
    case 8:
      lv_img_set_src(lv_obj, &_W_1830_i8_alpha_18x30);
      break;
    case 9:
      lv_img_set_src(lv_obj, &_W_1830_i9_alpha_18x30);
      break;
    default:
      Serial.println("显示O_1830数字错误");
      break;
  }
}

//显示天气图标
void WeatherPrintf::printfweather(lv_obj_t* lv_obj,int numw)
{
  if(numw==00)
  {
     lv_img_set_src(lv_obj, &t0);
  }
  else if(numw==01)
  {
     lv_img_set_src(lv_obj, &t1);
  }
  else if(numw==02)
  {
     lv_img_set_src(lv_obj, &t2);
  }
  else if(numw==03)
  {
    lv_img_set_src(lv_obj, &t3);
  }
  else if(numw==04)
  {
    lv_img_set_src(lv_obj, &t4);
  }
  else if(numw==05)
  {
    lv_img_set_src(lv_obj, &t5);
  }
  else if(numw==06)
  {
    lv_img_set_src(lv_obj, &t6);
  }
  else if(numw==07||numw==8||numw==21||numw==22)
  {
    lv_img_set_src(lv_obj, &t7);
  }
  else if(numw==9||numw==10||numw==23||numw==24)
  {
    lv_img_set_src(lv_obj, &t9);
  }
  else if(numw==11||numw==12||numw==25||numw==301)
  {
    lv_img_set_src(lv_obj, &t11);
  }
  else if(numw==13)
  {
    lv_img_set_src(lv_obj, &t13);
  }
  else if(numw==14||numw==26)
  {
   lv_img_set_src(lv_obj, &t14);
  }
  else if(numw==15||numw==27)
  {
    lv_img_set_src(lv_obj, &t15);
  }
  else if(numw==16||numw==17||numw==28||numw==302)
  {
    lv_img_set_src(lv_obj, &t16);
  }
  else if(numw==18)
  {
    lv_img_set_src(lv_obj, &t18);
  }
  else if(numw==19)
  {
    lv_img_set_src(lv_obj, &t19);
  }
  else if(numw==20)
  {
    lv_img_set_src(lv_obj, &t20);
  }
  else if(numw==29)
  {
    lv_img_set_src(lv_obj, &t29);
  }
  else if(numw==30)
  {
    lv_img_set_src(lv_obj, &t30);
  }
  else if(numw==31)
  {
    lv_img_set_src(lv_obj, &t31);
  }
  else if(numw==53||numw==32||numw==49||numw==54||numw==55||numw==56||numw==57||numw==58)
  {
    lv_img_set_src(lv_obj, &t53);
  }
  else
  {
    lv_img_set_src(lv_obj, &t99);
  }
}

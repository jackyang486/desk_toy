#include "weather_printf.h"
#include "src/generated/gui_guider.h"

static const lv_image_dsc_t *const white_digits_3660[] = {
  &W_3660_i0, &W_3660_i1, &W_3660_i2, &W_3660_i3, &W_3660_i4,
  &W_3660_i5, &W_3660_i6, &W_3660_i7, &W_3660_i8, &W_3660_i9
};

static const lv_image_dsc_t *const orange_digits_3660[] = {
  &O_3660_i0, &O_3660_i1, &O_3660_i2, &O_3660_i3, &O_3660_i4,
  &O_3660_i5, &O_3660_i6, &O_3660_i7, &O_3660_i8, &O_3660_i9
};

static const lv_image_dsc_t *const white_digits_1830[] = {
  &W_1830_i0, &W_1830_i1, &W_1830_i2, &W_1830_i3, &W_1830_i4,
  &W_1830_i5, &W_1830_i6, &W_1830_i7, &W_1830_i8, &W_1830_i9
};

static void set_digit(lv_obj_t *image, int number, const lv_image_dsc_t *const digits[])
{
  if(number < 0 || number > 9)
  {
    Serial.println("显示时钟数字错误");
    return;
  }
  lv_image_set_src(image, digits[number]);
}

//显示白色36*60大小数字
void WeatherPrintf::printfW3660(lv_obj_t* lv_obj,int numn)
{
  set_digit(lv_obj, numn, white_digits_3660);
}
//显示橙色36*60大小数字
void WeatherPrintf::printfO3660(lv_obj_t* lv_obj,int numn)
{
  set_digit(lv_obj, numn, orange_digits_3660);
}

//显示白色18*30大小数字
void WeatherPrintf::printfW1830(lv_obj_t* lv_obj,int numn)
{
  set_digit(lv_obj, numn, white_digits_1830);
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

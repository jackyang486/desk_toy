#ifndef FLY_MOUSE_H
#define FLY_MOUSE_H

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLEHIDDevice.h>
#include "src/generated/gui_guider.h"
#include "src/custom/custom.h"
#include "FastIMU.h"

void fly_mouse_init(void);
void fly_mouse_loop(void);
void fly_mouse_win_load(SCREEN_LOAD_STAUS loadstatus);

#endif

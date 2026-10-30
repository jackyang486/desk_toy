#ifndef GAME_H_
#define GAME_H_

#include "src/custom/custom.h"

void game_screen_load(SCREEN_LOAD_STAUS status);
void game_process_pending(void);
bool game_is_active(void);

// 手柄输入（由 game.cpp 管理）：走 WiFi 裸 TCP server（3333），APP / 脚本客户端
// 连到板子 IP:3333，每帧发 2 字节按键报告（详见 game.cpp 顶部的协议说明）。
// 生命周期挂在游戏列表页面上：进入游戏列表界面 gamepad_start()，退出游戏界面 gamepad_stop()。
// 收包跑在独立任务里（固定 core 0），所以 nofrendo 跑满 ui_task 时也收得到按键。
void gamepad_start(void);
void gamepad_stop(void);
// 手柄按键位图（bit=0 表示按下），供 nofrendo 的 osd_getinput() 读取
uint32_t gamepad_read_state(void);

#endif
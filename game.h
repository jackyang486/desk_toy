#ifndef GAME_H_
#define GAME_H_

#include "src/custom/custom.h"

void game_screen_load(SCREEN_LOAD_STAUS status);
void game_process_pending(void);
bool game_is_active(void);

#endif
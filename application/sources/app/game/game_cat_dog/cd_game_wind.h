#ifndef __CD_GAME_WIND_H__
#define __CD_GAME_WIND_H__

#include <stdio.h>
#include <stdlib.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

// Wind speed range, negative blows to the left (toward the cat)
#define WIND_SPEED_MAX (5)

typedef struct
{
	int8_t speed;
} cd_game_wind_t;

extern cd_game_wind_t wind;

#endif //__CD_GAME_WIND_H__

#ifndef __CD_GAME_CPU_H__
#define __CD_GAME_CPU_H__

#include <stdio.h>
#include <stdlib.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

#include "cd_game_match.h"
#include "cd_game_cat.h"
#include "cd_game_dog.h"
#include "cd_game_wind.h"
#include "cd_game_power.h"
#include "cd_game_missile.h"

// Angles the computer tries, low angles always hit the wall
#define CPU_ANGLE_MIN (45)
#define CPU_ANGLE_MAX (70)

// Power step between two simulated throws
#define CPU_POWER_STEP (5)

// Random error added to the planned power, larger is easier to beat
#define CPU_POWER_ERROR_EASY (30)
#define CPU_POWER_ERROR_HARD (16)

// Throw used when no simulated throw hits (strong wind against the computer)
#define CPU_FALLBACK_ANGLE (CD_GAME_ANGLE_DEFAULT)
#define CPU_FALLBACK_POWER (POWER_VALUE_MAX / 2)

typedef struct
{
	uint8_t angle;
	uint8_t power;
	uint8_t hit_from; // power window that hits at the chosen angle, for stt
	uint8_t hit_to;
} cd_game_cpu_t;

extern cd_game_cpu_t cpu;

#endif //__CD_GAME_CPU_H__

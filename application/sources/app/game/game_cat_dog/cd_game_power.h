#ifndef __CD_GAME_POWER_H__
#define __CD_GAME_POWER_H__

#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

// Power range, it sweeps up and down while MODE is held
#define POWER_VALUE_MAX (100)

// Power added or removed every tick while charging
#define POWER_STEP (4)

// Power states: idle, charging while MODE is held, locked once MODE is released
enum
{
	POWER_STATE_IDLE = 0,
	POWER_STATE_CHARGE,
	POWER_STATE_LOCKED,
};

typedef struct
{
	uint8_t state;
	uint8_t value;
	bool rising;
} cd_game_power_t;

extern cd_game_power_t power;

#endif //__CD_GAME_POWER_H__

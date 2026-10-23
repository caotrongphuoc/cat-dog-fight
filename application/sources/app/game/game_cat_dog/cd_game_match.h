#ifndef __CD_GAME_MATCH_H__
#define __CD_GAME_MATCH_H__

#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

// Ground line, every object stands on it
#define CD_GAME_GROUND_AXIS_Y (63)

// Throw angle in degrees, UP / DOWN change it by one step
#define CD_GAME_ANGLE_MIN (15)
#define CD_GAME_ANGLE_MAX (75)
#define CD_GAME_ANGLE_STEP (5)
#define CD_GAME_ANGLE_DEFAULT (45)

// HP lost by a player hit by a fish or a bone
#define CD_GAME_HIT_DAMAGE (20)

// Role of this board, picked at build time (make BOARD=cat|dog)
#if defined(CD_GAME_BOARD_DOG)
#define CD_GAME_BOARD_ME (MATCH_TURN_DOG)
#else
#define CD_GAME_BOARD_ME (MATCH_TURN_CAT)
#endif

// Match states
enum
{
	MATCH_STATE_OFF = 0,
	MATCH_STATE_PLAY,
	MATCH_STATE_OVER,
};

// Whose turn it is, cat always opens the match
enum
{
	MATCH_TURN_CAT = 0,
	MATCH_TURN_DOG,
};

// Phases of a turn: aim and charge, then the missile flies
enum
{
	MATCH_PHASE_AIM = 0,
	MATCH_PHASE_FLY,
};

typedef struct
{
	uint8_t state;
	uint8_t turn;
	uint8_t phase;
} cd_game_match_t;

extern cd_game_match_t match;

#endif //__CD_GAME_MATCH_H__

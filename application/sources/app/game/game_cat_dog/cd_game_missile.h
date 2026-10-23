#ifndef __CD_GAME_MISSILE_H__
#define __CD_GAME_MISSILE_H__

#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

#include "view_render.h"

#include "cd_game_match.h"
#include "cd_game_cat.h"
#include "cd_game_dog.h"
#include "cd_game_border.h"
#include "cd_game_wind.h"
#include "cd_game_power.h"

// Fixed point: positions and speeds are kept in 1/256 pixel so both boards compute the same path
#define MISSILE_FIXED_SHIFT (8)

// Gravity added to the vertical speed every tick (0.25 px/tick^2)
#define MISSILE_GRAVITY (64)

// Throw speed at power 0 and at full power (2.0 and 6.5 px/tick)
#define MISSILE_SPEED_MIN (512)
#define MISSILE_SPEED_MAX (1664)

// Horizontal acceleration added per unit of wind speed every tick
#define MISSILE_WIND_FORCE (4)

// Size of fish bitmap, thrown by the cat
#define MISSILE_FISH_SIZE_BITMAP_X (8)
#define MISSILE_FISH_SIZE_BITMAP_Y (5)

// Size of bone bitmap, thrown by the dog
#define MISSILE_BONE_SIZE_BITMAP_X (9)
#define MISSILE_BONE_SIZE_BITMAP_Y (4)

// What is flying
enum
{
	MISSILE_TYPE_FISH = 0,
	MISSILE_TYPE_BONE,
};

// Flight state
enum
{
	MISSILE_STATE_IDLE = 0,
	MISSILE_STATE_FLY,
};

// How the last throw ended
enum
{
	MISSILE_RESULT_NONE = 0,
	MISSILE_RESULT_HIT_CAT,
	MISSILE_RESULT_HIT_DOG,
	MISSILE_RESULT_HIT_BORDER,
	MISSILE_RESULT_MISS,
};

typedef struct
{
	uint8_t type;
	uint8_t state;
	uint8_t result;
	int32_t x;
	int32_t y;
	int32_t vx;
	int32_t vy;
} cd_game_missile_t;

extern cd_game_missile_t missile;

extern int16_t cd_game_missile_cos(uint8_t angle);
extern int16_t cd_game_missile_sin(uint8_t angle);

#endif //__CD_GAME_MISSILE_H__

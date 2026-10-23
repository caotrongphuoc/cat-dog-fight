#ifndef __CD_GAME_CAT_H__
#define __CD_GAME_CAT_H__

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

// Size of cat bitmap
#define CAT_SIZE_BITMAP_X (18)
#define CAT_SIZE_BITMAP_Y (18)

// Cat stands at the left edge
#define CAT_AXIS_X (2)
#define CAT_AXIS_Y (CD_GAME_GROUND_AXIS_Y - CAT_SIZE_BITMAP_Y)

// Health at the start of a match
#define CAT_HP_MAX (100)

// Where the missile leaves the cat, relative to its bitmap
#define CAT_HAND_OFFSET_Y (4)

// Hit effect: ticks it lasts and shake in pixels
#define CAT_HIT_TICKS (12)
#define CAT_HIT_SHAKE (1)

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t hp;
	uint8_t angle;
	uint8_t hit_ticks;
} cd_game_cat_t;

extern cd_game_cat_t cat;

#endif //__CD_GAME_CAT_H__

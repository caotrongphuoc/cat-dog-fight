#ifndef __CD_GAME_DOG_H__
#define __CD_GAME_DOG_H__

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

// Size of dog bitmap
#define DOG_SIZE_BITMAP_X (18)
#define DOG_SIZE_BITMAP_Y (17)

// Dog stands at the right edge
#define DOG_AXIS_X (LCD_WIDTH - DOG_SIZE_BITMAP_X - 2)
#define DOG_AXIS_Y (CD_GAME_GROUND_AXIS_Y - DOG_SIZE_BITMAP_Y)

// Health at the start of a match
#define DOG_HP_MAX (100)

// Where the missile leaves the dog, relative to its bitmap
#define DOG_HAND_OFFSET_Y (4)

// Hit effect: ticks it lasts and shake in pixels
#define DOG_HIT_TICKS (12)
#define DOG_HIT_SHAKE (1)

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t hp;
	uint8_t angle;
	uint8_t hit_ticks;
} cd_game_dog_t;

extern cd_game_dog_t dog;

#endif //__CD_GAME_DOG_H__

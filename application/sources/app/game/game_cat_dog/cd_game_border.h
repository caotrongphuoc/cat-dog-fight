#ifndef __CD_GAME_BORDER_H__
#define __CD_GAME_BORDER_H__

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

// Size of the wall in the middle of the field
#define BORDER_WIDTH (6)
#define BORDER_HEIGHT (26)

// Wall is centered on the screen and stands on the ground
#define BORDER_AXIS_X ((LCD_WIDTH - BORDER_WIDTH) / 2)
#define BORDER_AXIS_Y (CD_GAME_GROUND_AXIS_Y - BORDER_HEIGHT)

// Height of one brick row drawn on the wall
#define BORDER_BRICK_HEIGHT (4)

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
} cd_game_border_t;

extern cd_game_border_t border;

#endif //__CD_GAME_BORDER_H__

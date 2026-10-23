#ifndef __SCR_GAME_CAT_DOG_H__
#define __SCR_GAME_CAT_DOG_H__

#include <stdio.h>
#include <stdlib.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"
#include "view_render.h"

#include "screens.h"
#include "screens_bitmap.h"

#include "cd_game_match.h"
#include "cd_game_cat.h"
#include "cd_game_dog.h"
#include "cd_game_border.h"
#include "cd_game_wind.h"
#include "cd_game_power.h"
#include "cd_game_missile.h"

extern view_dynamic_t dyn_view_game_cat_dog;
extern view_screen_t scr_game_cat_dog;
extern void scr_game_cat_dog_handle(ak_msg_t* msg);

// HP bars in the top corners, cat on the left, dog on the right
#define CD_GAME_HUD_AXIS_Y (0)
#define CD_GAME_HUD_HP_BAR_WIDTH (44)
#define CD_GAME_HUD_HP_BAR_HEIGHT (5)
#define CD_GAME_HUD_CAT_HP_BAR_AXIS_X (0)
#define CD_GAME_HUD_DOG_HP_BAR_AXIS_X (LCD_WIDTH - CD_GAME_HUD_HP_BAR_WIDTH)

// Dot under the HP bar of the player whose turn it is
#define CD_GAME_HUD_TURN_MARK_AXIS_Y (CD_GAME_HUD_HP_BAR_HEIGHT + 2)
#define CD_GAME_HUD_TURN_MARK_SIZE (2)

// Wind arrow in the top middle, pixels per unit of wind speed
#define CD_GAME_HUD_WIND_AXIS_X (LCD_WIDTH / 2)
#define CD_GAME_HUD_WIND_AXIS_Y (2)
#define CD_GAME_HUD_WIND_STEP (3)
#define CD_GAME_HUD_WIND_HEAD (2)

// Power bar over the head of the player whose turn it is
#define CD_GAME_HUD_POWER_OFFSET_Y (6)
#define CD_GAME_HUD_POWER_HEIGHT (4)

// Aim line drawn from the hand of the player whose turn it is
#define CD_GAME_HUD_AIM_LENGTH (10)

// Size of the star drawn over a player who was just hit
#define CD_GAME_HUD_STAR_SIZE_BITMAP_X (7)
#define CD_GAME_HUD_STAR_SIZE_BITMAP_Y (7)
#define CD_GAME_HUD_STAR_GAP (2)

// Wind strength printed under the arrow
#define CD_GAME_HUD_WIND_TEXT_AXIS_X (CD_GAME_HUD_WIND_AXIS_X - 2)
#define CD_GAME_HUD_WIND_TEXT_AXIS_Y (CD_GAME_HUD_WIND_AXIS_Y + CD_GAME_HUD_WIND_HEAD + 3)

#endif //__SCR_GAME_CAT_DOG_H__

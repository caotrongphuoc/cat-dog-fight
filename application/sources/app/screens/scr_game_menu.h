#ifndef __SCR_GAME_MENU_H__
#define __SCR_GAME_MENU_H__

#include <stdio.h>
#include <string.h>

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

#include "buzzer.h"

#include "screens.h"
#include "screens_bitmap.h"

#include "cd_game_match.h"
#include "cd_game_cat.h"
#include "cd_game_dog.h"

extern view_dynamic_t dyn_view_game_menu;
extern view_screen_t scr_game_menu;
extern void scr_game_menu_handle(ak_msg_t* msg);

// Menu entries, in the order UP / DOWN walks them
enum
{
	MENU_ITEM_ONE_PLAYER = 0,
	MENU_ITEM_TWO_PLAYERS,
	MENU_ITEM_LEVEL,
	MENU_ITEM_SOUND,
	MENU_ITEM_EXIT,
	MENU_ITEM_NUMBER,
};

// Title, centered on the top line in text size 2
#define MENU_TITLE_AXIS_X (4)
#define MENU_TITLE_AXIS_Y (0)

// Selected entry under the title, centered
#define MENU_ITEM_AXIS_Y (20)

// Role of this board under the entry
#define MENU_ROLE_AXIS_X (40)
#define MENU_ROLE_AXIS_Y (30)

// Wins saved in EEPROM, centered between the cat and the dog
#define MENU_SCORE_AXIS_Y (40)

#endif //__SCR_GAME_MENU_H__

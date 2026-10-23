#ifndef __SCR_GAME_OVER_H__
#define __SCR_GAME_OVER_H__

#include <stdio.h>

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

extern view_dynamic_t dyn_view_game_over;
extern view_screen_t scr_game_over;
extern void scr_game_over_handle(ak_msg_t* msg);

// Winner line in text size 2 ("CAT WINS" / "DOG WINS")
#define GAME_OVER_TITLE_AXIS_X (16)
#define GAME_OVER_TITLE_AXIS_Y (2)

// Result for this board under the title
#define GAME_OVER_RESULT_AXIS_X (40)
#define GAME_OVER_RESULT_AXIS_Y (22)

// Winner bitmap in the middle, hint on the bottom line
#define GAME_OVER_WINNER_AXIS_X ((LCD_WIDTH - CAT_SIZE_BITMAP_X) / 2)
#define GAME_OVER_WINNER_AXIS_Y (34)
#define GAME_OVER_HINT_AXIS_X (7)
#define GAME_OVER_HINT_AXIS_Y (56)

#endif //__SCR_GAME_OVER_H__

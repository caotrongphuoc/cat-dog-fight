#include "scr_game_over.h"

/*****************************************************************************/
/* View - Game over */
/*****************************************************************************/

static void view_scr_game_over();

view_dynamic_t dyn_view_game_over = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game_over};

view_screen_t scr_game_over = {
    &dyn_view_game_over,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

void view_scr_game_over()
{
	bool cat_win = (match.winner == MATCH_TURN_CAT);

	view_render.setTextColor(WHITE);
	view_render.setTextSize(2);
	view_render.setCursor(GAME_OVER_TITLE_AXIS_X, GAME_OVER_TITLE_AXIS_Y);
	view_render.print(cat_win ? "CAT WINS" : "DOG WINS");

	view_render.setTextSize(1);
	view_render.setCursor(GAME_OVER_RESULT_AXIS_X, GAME_OVER_RESULT_AXIS_Y);
	view_render.print(match.winner == CD_GAME_BOARD_ME ? "You win!" : "You lose");

	if (cat_win)
	{
		view_render.drawBitmap(GAME_OVER_WINNER_AXIS_X, GAME_OVER_WINNER_AXIS_Y, bitmap_cat, CAT_SIZE_BITMAP_X, CAT_SIZE_BITMAP_Y, WHITE);
	}
	else
	{
		view_render.drawBitmap(GAME_OVER_WINNER_AXIS_X, GAME_OVER_WINNER_AXIS_Y, bitmap_dog, DOG_SIZE_BITMAP_X, DOG_SIZE_BITMAP_Y, WHITE);
	}

	view_render.setCursor(GAME_OVER_HINT_AXIS_X, GAME_OVER_HINT_AXIS_Y);
	view_render.print("UP:again  MODE:menu");
}

/*****************************************************************************/
/* Handle - Game over */
/*****************************************************************************/

void scr_game_over_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case SCREEN_ENTRY:
	{
		APP_DBG_SIG("SCREEN_ENTRY\n");
	}
	break;

	case AC_DISPLAY_BUTTON_UP_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_PRESSED\n");
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		SCREEN_TRAN(scr_game_cat_dog_handle, &scr_game_cat_dog);
	}
	break;

	case AC_DISPLAY_BUTTON_MODE_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_PRESSED\n");
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		SCREEN_TRAN(scr_game_menu_handle, &scr_game_menu);
	}
	break;

	default:
		break;
	}
}

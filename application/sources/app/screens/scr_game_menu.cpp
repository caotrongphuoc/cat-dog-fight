#include "scr_game_menu.h"

/*****************************************************************************/
/* Variable Declaration - Game menu */
/*****************************************************************************/

static uint8_t menu_item;

static const char* const menu_item_name[MENU_ITEM_NUMBER] = {
    "<< Play >>",
    "<< Exit >>",
};

/*****************************************************************************/
/* View - Game menu */
/*****************************************************************************/

static void view_scr_game_menu();

view_dynamic_t dyn_view_game_menu = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game_menu};

view_screen_t scr_game_menu = {
    &dyn_view_game_menu,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

void view_scr_game_menu()
{
	view_render.setTextColor(WHITE);
	view_render.setTextSize(2);
	view_render.setCursor(MENU_TITLE_AXIS_X, MENU_TITLE_AXIS_Y);
	view_render.print("CAT vs DOG");

	view_render.setTextSize(1);
	view_render.setCursor(MENU_ITEM_AXIS_X, MENU_ITEM_AXIS_Y);
	view_render.print(menu_item_name[menu_item]);
	view_render.setCursor(MENU_ROLE_AXIS_X, MENU_ROLE_AXIS_Y);
	view_render.print(CD_GAME_BOARD_ME == MATCH_TURN_CAT ? "You: CAT" : "You: DOG");

	view_render.drawBitmap(CAT_AXIS_X, CAT_AXIS_Y, bitmap_cat, CAT_SIZE_BITMAP_X, CAT_SIZE_BITMAP_Y, WHITE);
	view_render.drawBitmap(DOG_AXIS_X, DOG_AXIS_Y, bitmap_dog, DOG_SIZE_BITMAP_X, DOG_SIZE_BITMAP_Y, WHITE);
	view_render.drawFastHLine(0, CD_GAME_GROUND_AXIS_Y, LCD_WIDTH, WHITE);
}

/*****************************************************************************/
/* Handle - Game menu */
/*****************************************************************************/

void scr_game_menu_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case SCREEN_ENTRY:
	{
		APP_DBG_SIG("SCREEN_ENTRY\n");
		menu_item = MENU_ITEM_PLAY;
	}
	break;

	case AC_DISPLAY_BUTTON_UP_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_PRESSED\n");
		menu_item = (menu_item == 0) ? (MENU_ITEM_NUMBER - 1) : (menu_item - 1);
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
	}
	break;

	case AC_DISPLAY_BUTTON_DOWN_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_PRESSED\n");
		menu_item = (menu_item + 1) % MENU_ITEM_NUMBER;
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
	}
	break;

	case AC_DISPLAY_BUTTON_MODE_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_PRESSED\n");
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		if (menu_item == MENU_ITEM_PLAY)
		{
			SCREEN_TRAN(scr_game_cat_dog_handle, &scr_game_cat_dog);
		}
		else
		{
			SCREEN_TRAN(scr_startup_handle, &scr_startup);
		}
	}
	break;

	default:
		break;
	}
}

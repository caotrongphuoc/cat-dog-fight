#include "scr_game_cat_dog.h"

/*****************************************************************************/
/* View - Game cat dog */
/*****************************************************************************/

static void view_scr_game_cat_dog();

view_dynamic_t dyn_view_game_cat_dog = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game_cat_dog};

view_screen_t scr_game_cat_dog = {
    &dyn_view_game_cat_dog,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

static void cd_game_hp_bar_display(uint8_t x, uint8_t hp, uint8_t hp_max)
{
	uint8_t fill = (uint16_t)hp * (CD_GAME_HUD_HP_BAR_WIDTH - 2) / hp_max;
	view_render.drawRect(x, CD_GAME_HUD_AXIS_Y, CD_GAME_HUD_HP_BAR_WIDTH, CD_GAME_HUD_HP_BAR_HEIGHT, WHITE);
	view_render.fillRect(x + 1, CD_GAME_HUD_AXIS_Y + 1, fill, CD_GAME_HUD_HP_BAR_HEIGHT - 2, WHITE);
}

static void cd_game_hud_display()
{
	cd_game_hp_bar_display(CD_GAME_HUD_CAT_HP_BAR_AXIS_X, cat.hp, CAT_HP_MAX);
	cd_game_hp_bar_display(CD_GAME_HUD_DOG_HP_BAR_AXIS_X, dog.hp, DOG_HP_MAX);

	uint8_t mark_x = (match.turn == MATCH_TURN_CAT) ? CD_GAME_HUD_CAT_HP_BAR_AXIS_X : (LCD_WIDTH - CD_GAME_HUD_TURN_MARK_SIZE);
	view_render.fillRect(mark_x, CD_GAME_HUD_TURN_MARK_AXIS_Y, CD_GAME_HUD_TURN_MARK_SIZE, CD_GAME_HUD_TURN_MARK_SIZE, WHITE);
}

static void cd_game_wind_display()
{
	int16_t x = CD_GAME_HUD_WIND_AXIS_X;
	int16_t y = CD_GAME_HUD_WIND_AXIS_Y;
	int16_t tip = x + wind.speed * CD_GAME_HUD_WIND_STEP;

	view_render.drawFastVLine(x, y - CD_GAME_HUD_WIND_HEAD, 2 * CD_GAME_HUD_WIND_HEAD + 1, WHITE);
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(CD_GAME_HUD_WIND_TEXT_AXIS_X, CD_GAME_HUD_WIND_TEXT_AXIS_Y);
	view_render.print(abs(wind.speed));
	if (wind.speed == 0)
	{
		return;
	}

	int16_t back = (wind.speed > 0) ? -CD_GAME_HUD_WIND_HEAD : CD_GAME_HUD_WIND_HEAD;
	view_render.drawLine(x, y, tip, y, WHITE);
	view_render.drawLine(tip, y, tip + back, y - CD_GAME_HUD_WIND_HEAD, WHITE);
	view_render.drawLine(tip, y, tip + back, y + CD_GAME_HUD_WIND_HEAD, WHITE);
}

static void cd_game_border_display()
{
	view_render.drawRect(border.x, border.y, border.width, border.height, WHITE);
	for (uint8_t y = border.y + BORDER_BRICK_HEIGHT; y < border.y + border.height; y += BORDER_BRICK_HEIGHT)
	{
		view_render.drawFastHLine(border.x, y, border.width, WHITE);
	}
	if (border.hit_ticks > 0)
	{
		const unsigned char* dust = (border.hit_ticks % 2) ? bitmap_dust_I : bitmap_dust_II;
		view_render.fillRect(border.hit_x, border.hit_y, BORDER_DUST_SIZE_BITMAP_X, BORDER_DUST_SIZE_BITMAP_Y, BLACK);
		view_render.drawBitmap(border.hit_x, border.hit_y, dust, BORDER_DUST_SIZE_BITMAP_X, BORDER_DUST_SIZE_BITMAP_Y, WHITE);
	}
}

// Shake and blink while hit, with a star over the head
static void cd_game_player_display(int16_t x, int16_t y, uint8_t w, uint8_t h, const unsigned char* bitmap, uint8_t hit_ticks, uint8_t shake)
{
	if (hit_ticks == 0)
	{
		view_render.drawBitmap(x, y, bitmap, w, h, WHITE);
		return;
	}
	int16_t dx = (hit_ticks % 2) ? shake : -shake;
	if ((hit_ticks / 2) % 2 == 0)
	{
		view_render.drawBitmap(x + dx, y, bitmap, w, h, WHITE);
	}
	view_render.drawBitmap(x + (w - CD_GAME_HUD_STAR_SIZE_BITMAP_X) / 2, y - CD_GAME_HUD_STAR_SIZE_BITMAP_Y - CD_GAME_HUD_STAR_GAP, bitmap_star, CD_GAME_HUD_STAR_SIZE_BITMAP_X, CD_GAME_HUD_STAR_SIZE_BITMAP_Y, WHITE);
}

static void cd_game_power_display()
{
	if (match.phase == MATCH_PHASE_AIM && power.state == POWER_STATE_IDLE)
	{
		return;
	}
	bool cat_turn = (match.turn == MATCH_TURN_CAT);
	uint8_t x = cat_turn ? cat.x : dog.x;
	uint8_t y = (cat_turn ? cat.y : dog.y) - CD_GAME_HUD_POWER_OFFSET_Y;
	uint8_t width = cat_turn ? CAT_SIZE_BITMAP_X : DOG_SIZE_BITMAP_X;
	uint8_t fill = (uint16_t)power.value * (width - 2) / POWER_VALUE_MAX;
	view_render.drawRect(x, y, width, CD_GAME_HUD_POWER_HEIGHT, WHITE);
	view_render.fillRect(x + 1, y + 1, fill, CD_GAME_HUD_POWER_HEIGHT - 2, WHITE);
}

static void cd_game_aim_display()
{
	if (match.state != MATCH_STATE_PLAY || match.phase != MATCH_PHASE_AIM || match.turn != CD_GAME_BOARD_ME)
	{
		return;
	}
	bool cat_turn = (match.turn == MATCH_TURN_CAT);
	uint8_t angle = cat_turn ? cat.angle : dog.angle;
	int16_t x = cat_turn ? (cat.x + CAT_SIZE_BITMAP_X) : dog.x;
	int16_t y = cat_turn ? (cat.y + CAT_HAND_OFFSET_Y) : (dog.y + DOG_HAND_OFFSET_Y);
	int16_t dx = (CD_GAME_HUD_AIM_LENGTH * cd_game_missile_cos(angle)) >> MISSILE_FIXED_SHIFT;
	int16_t dy = (CD_GAME_HUD_AIM_LENGTH * cd_game_missile_sin(angle)) >> MISSILE_FIXED_SHIFT;
	view_render.drawLine(x, y, cat_turn ? (x + dx) : (x - dx), y - dy, WHITE);
}

static void cd_game_missile_display()
{
	if (missile.state != MISSILE_STATE_FLY)
	{
		return;
	}
	int16_t x = missile.x >> MISSILE_FIXED_SHIFT;
	int16_t y = missile.y >> MISSILE_FIXED_SHIFT;
	if (missile.type == MISSILE_TYPE_FISH)
	{
		view_render.drawBitmap(x, y, bitmap_fish, MISSILE_FISH_SIZE_BITMAP_X, MISSILE_FISH_SIZE_BITMAP_Y, WHITE);
	}
	else
	{
		view_render.drawBitmap(x, y, bitmap_bone, MISSILE_BONE_SIZE_BITMAP_X, MISSILE_BONE_SIZE_BITMAP_Y, WHITE);
	}
}

static void cd_game_wait_display()
{
	if (match.state != MATCH_STATE_WAIT)
	{
		return;
	}
	view_render.fillRect(CD_GAME_HUD_WAIT_AXIS_X, CD_GAME_HUD_WAIT_AXIS_Y, CD_GAME_HUD_WAIT_WIDTH, CD_GAME_HUD_WAIT_HEIGHT, BLACK);
	view_render.drawRect(CD_GAME_HUD_WAIT_AXIS_X, CD_GAME_HUD_WAIT_AXIS_Y, CD_GAME_HUD_WAIT_WIDTH, CD_GAME_HUD_WAIT_HEIGHT, WHITE);
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(CD_GAME_HUD_WAIT_TEXT_AXIS_X, CD_GAME_HUD_WAIT_TEXT_AXIS_Y);
	view_render.print(CD_GAME_BOARD_ME == MATCH_TURN_CAT ? "Waiting for DOG" : "Waiting for CAT");
}

void view_scr_game_cat_dog()
{
	cd_game_hud_display();
	cd_game_wind_display();
	cd_game_border_display();
	cd_game_player_display(cat.x, cat.y, CAT_SIZE_BITMAP_X, CAT_SIZE_BITMAP_Y, bitmap_cat, cat.hit_ticks, CAT_HIT_SHAKE);
	cd_game_player_display(dog.x, dog.y, DOG_SIZE_BITMAP_X, DOG_SIZE_BITMAP_Y, bitmap_dog, dog.hit_ticks, DOG_HIT_SHAKE);
	view_render.drawFastHLine(0, CD_GAME_GROUND_AXIS_Y, LCD_WIDTH, WHITE);
	cd_game_power_display();
	cd_game_aim_display();
	cd_game_missile_display();
	cd_game_wait_display();
}

static bool cd_game_can_aim()
{
	return (match.state == MATCH_STATE_PLAY) && (match.phase == MATCH_PHASE_AIM) && (match.turn == CD_GAME_BOARD_ME);
}

// Something moves on the field, the tick (and the 43 ms OLED refresh) is only needed then
static bool cd_game_is_animating()
{
	return (power.state == POWER_STATE_CHARGE) || (missile.state == MISSILE_STATE_FLY) || (cat.hit_ticks > 0) || (dog.hit_ticks > 0) || (border.hit_ticks > 0);
}

static void cd_game_aim_post(uint8_t cat_sig, uint8_t dog_sig)
{
	if (match.turn == MATCH_TURN_CAT)
	{
		task_post_pure_msg(CD_GAME_CAT_ID, cat_sig);
	}
	else
	{
		task_post_pure_msg(CD_GAME_DOG_ID, dog_sig);
	}
}

/*****************************************************************************/
/* Handle - Game cat dog */
/*****************************************************************************/

void scr_game_cat_dog_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case SCREEN_ENTRY:
	{
		APP_DBG_SIG("SCREEN_ENTRY\n");
		task_post_pure_msg(CD_GAME_MATCH_ID, CD_GAME_MATCH_SETUP);
		task_post_pure_msg(CD_GAME_CAT_ID, CD_GAME_CAT_SETUP);
		task_post_pure_msg(CD_GAME_DOG_ID, CD_GAME_DOG_SETUP);
		task_post_pure_msg(CD_GAME_BORDER_ID, CD_GAME_BORDER_SETUP);
		task_post_pure_msg(CD_GAME_WIND_ID, CD_GAME_WIND_SETUP);
		task_post_pure_msg(CD_GAME_POWER_ID, CD_GAME_POWER_SETUP);
		task_post_pure_msg(CD_GAME_MISSILE_ID, CD_GAME_MISSILE_SETUP);
		task_post_pure_msg(CD_GAME_LINK_ID, CD_GAME_LINK_SETUP);
		timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
	}
	break;

	case CD_GAME_ANIMATE:
	{
		APP_DBG_SIG("CD_GAME_ANIMATE\n");
		timer_set(AC_TASK_DISPLAY_ID, CD_GAME_TIME_TICK, CD_GAME_TIME_TICK_INTERVAL, TIMER_PERIODIC);
	}
	break;

	case CD_GAME_TIME_TICK:
	{
		task_post_pure_msg(CD_GAME_POWER_ID, CD_GAME_POWER_RUN);
		task_post_pure_msg(CD_GAME_MISSILE_ID, CD_GAME_MISSILE_RUN);
		task_post_pure_msg(CD_GAME_CAT_ID, CD_GAME_CAT_RUN);
		task_post_pure_msg(CD_GAME_DOG_ID, CD_GAME_DOG_RUN);
		task_post_pure_msg(CD_GAME_BORDER_ID, CD_GAME_BORDER_RUN);
		if (!cd_game_is_animating())
		{
			timer_remove_attr(AC_TASK_DISPLAY_ID, CD_GAME_TIME_TICK);
		}
	}
	break;

	case CD_GAME_EXIT_GAME:
	{
		APP_DBG_SIG("CD_GAME_EXIT_GAME\n");
		timer_remove_attr(AC_TASK_DISPLAY_ID, CD_GAME_TIME_TICK);
		task_post_pure_msg(CD_GAME_LINK_ID, CD_GAME_LINK_RESET);
		task_post_pure_msg(CD_GAME_MATCH_ID, CD_GAME_MATCH_RESET);
		SCREEN_TRAN(scr_game_over_handle, &scr_game_over);
	}
	break;

	case AC_DISPLAY_BUTTON_UP_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_PRESSED\n");
		if (cd_game_can_aim())
		{
			cd_game_aim_post(CD_GAME_CAT_AIM_UP, CD_GAME_DOG_AIM_UP);
		}
	}
	break;

	case AC_DISPLAY_BUTTON_DOWN_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_PRESSED\n");
		if (cd_game_can_aim())
		{
			cd_game_aim_post(CD_GAME_CAT_AIM_DOWN, CD_GAME_DOG_AIM_DOWN);
		}
	}
	break;

	case AC_DISPLAY_BUTTON_MODE_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_PRESSED\n");
		if (match.state == MATCH_STATE_WAIT)
		{
			// Give up waiting for the rival board
			timer_remove_attr(AC_TASK_DISPLAY_ID, CD_GAME_TIME_TICK);
			task_post_pure_msg(CD_GAME_LINK_ID, CD_GAME_LINK_RESET);
			task_post_pure_msg(CD_GAME_MATCH_ID, CD_GAME_MATCH_RESET);
			SCREEN_TRAN(scr_game_menu_handle, &scr_game_menu);
			break;
		}
		if (cd_game_can_aim())
		{
			task_post_pure_msg(CD_GAME_POWER_ID, CD_GAME_POWER_CHARGE);
			timer_set(AC_TASK_DISPLAY_ID, CD_GAME_TIME_TICK, CD_GAME_TIME_TICK_INTERVAL, TIMER_PERIODIC);
		}
	}
	break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		task_post_pure_msg(CD_GAME_POWER_ID, CD_GAME_POWER_RELEASE);
	}
	break;

	default:
		break;
	}
}

#include "cd_game_border.h"

cd_game_border_t border;

void cd_game_border_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case CD_GAME_BORDER_SETUP:
	{
		APP_DBG_SIG("CD_GAME_BORDER_SETUP\n");
		border.x = BORDER_AXIS_X;
		border.y = BORDER_AXIS_Y;
		border.width = BORDER_WIDTH;
		border.height = BORDER_HEIGHT;
		border.hit_ticks = 0;
	}
	break;

	case CD_GAME_BORDER_HIT:
	{
		APP_DBG_SIG("CD_GAME_BORDER_HIT\n");
		int16_t y = missile.y >> MISSILE_FIXED_SHIFT;
		border.hit_x = (missile.type == MISSILE_TYPE_FISH) ? (border.x - BORDER_DUST_SIZE_BITMAP_X / 2) : (border.x + border.width - BORDER_DUST_SIZE_BITMAP_X / 2);
		border.hit_y = (y < border.y) ? (border.y - BORDER_DUST_SIZE_BITMAP_Y) : y;
		border.hit_ticks = BORDER_HIT_TICKS;
	}
	break;

	case CD_GAME_BORDER_RUN:
	{
		if (border.hit_ticks > 0)
		{
			border.hit_ticks--;
		}
	}
	break;

	default:
		break;
	}
}

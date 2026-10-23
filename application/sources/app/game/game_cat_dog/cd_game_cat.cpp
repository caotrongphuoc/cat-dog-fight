#include "cd_game_cat.h"

cd_game_cat_t cat;

void cd_game_cat_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case CD_GAME_CAT_SETUP:
	{
		APP_DBG_SIG("CD_GAME_CAT_SETUP\n");
		cat.x = CAT_AXIS_X;
		cat.y = CAT_AXIS_Y;
		cat.hp = CAT_HP_MAX;
		cat.angle = CD_GAME_ANGLE_DEFAULT;
		cat.hit_ticks = 0;
	}
	break;

	case CD_GAME_CAT_AIM_UP:
	{
		APP_DBG_SIG("CD_GAME_CAT_AIM_UP\n");
		if (cat.angle + CD_GAME_ANGLE_STEP <= CD_GAME_ANGLE_MAX)
		{
			cat.angle += CD_GAME_ANGLE_STEP;
		}
	}
	break;

	case CD_GAME_CAT_AIM_DOWN:
	{
		APP_DBG_SIG("CD_GAME_CAT_AIM_DOWN\n");
		if (cat.angle >= CD_GAME_ANGLE_MIN + CD_GAME_ANGLE_STEP)
		{
			cat.angle -= CD_GAME_ANGLE_STEP;
		}
	}
	break;

	case CD_GAME_CAT_HIT:
	{
		APP_DBG_SIG("CD_GAME_CAT_HIT\n");
		cat.hp = (cat.hp > CD_GAME_HIT_DAMAGE) ? (cat.hp - CD_GAME_HIT_DAMAGE) : 0;
		cat.hit_ticks = CAT_HIT_TICKS;
	}
	break;

	case CD_GAME_CAT_RUN:
	{
		if (cat.hit_ticks > 0)
		{
			cat.hit_ticks--;
		}
	}
	break;

	default:
		break;
	}
}

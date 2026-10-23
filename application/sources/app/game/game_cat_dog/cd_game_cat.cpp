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
	}
	break;

	default:
		break;
	}
}

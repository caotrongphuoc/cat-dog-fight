#include "cd_game_wind.h"

cd_game_wind_t wind;

void cd_game_wind_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case CD_GAME_WIND_SETUP:
	{
		APP_DBG_SIG("CD_GAME_WIND_SETUP\n");
		srand(sys_ctrl_millis());
		wind.speed = (rand() % (2 * WIND_SPEED_MAX + 1)) - WIND_SPEED_MAX;
	}
	break;

	default:
		break;
	}
}

#include "cd_game_wind.h"

cd_game_wind_t wind;

static int8_t cd_game_wind_random()
{
	return (rand() % (2 * WIND_SPEED_MAX + 1)) - WIND_SPEED_MAX;
}

void cd_game_wind_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case CD_GAME_WIND_SETUP:
	{
		APP_DBG_SIG("CD_GAME_WIND_SETUP\n");
		srand(sys_ctrl_millis());
		wind.speed = cd_game_wind_random();
	}
	break;

	case CD_GAME_WIND_CHANGE:
	{
		APP_DBG_SIG("CD_GAME_WIND_CHANGE\n");
		wind.speed = cd_game_wind_random();
	}
	break;

	default:
		break;
	}
}

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

	case CD_GAME_WIND_SET:
	{
		APP_DBG_SIG("CD_GAME_WIND_SET\n");
		if (get_data_len_common_msg(msg) != sizeof(int8_t))
		{
			break;
		}
		int8_t speed = (int8_t)*get_data_common_msg(msg);
		if (speed >= -WIND_SPEED_MAX && speed <= WIND_SPEED_MAX)
		{
			wind.speed = speed;
		}
	}
	break;

	default:
		break;
	}
}

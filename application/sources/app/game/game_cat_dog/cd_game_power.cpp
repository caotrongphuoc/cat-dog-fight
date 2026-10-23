#include "cd_game_power.h"

cd_game_power_t power;

void cd_game_power_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case CD_GAME_POWER_SETUP:
	{
		APP_DBG_SIG("CD_GAME_POWER_SETUP\n");
		power.state = POWER_STATE_IDLE;
		power.value = 0;
		power.rising = true;
	}
	break;

	case CD_GAME_POWER_CHARGE:
	{
		APP_DBG_SIG("CD_GAME_POWER_CHARGE\n");
		if (power.state != POWER_STATE_IDLE)
		{
			break;
		}
		power.state = POWER_STATE_CHARGE;
		power.value = 0;
		power.rising = true;
	}
	break;

	case CD_GAME_POWER_RUN:
	{
		if (power.state != POWER_STATE_CHARGE)
		{
			break;
		}
		if (power.rising)
		{
			power.value += POWER_STEP;
			if (power.value >= POWER_VALUE_MAX)
			{
				power.value = POWER_VALUE_MAX;
				power.rising = false;
			}
		}
		else
		{
			power.value = (power.value > POWER_STEP) ? (power.value - POWER_STEP) : 0;
			if (power.value == 0)
			{
				power.rising = true;
			}
		}
	}
	break;

	case CD_GAME_POWER_RELEASE:
	{
		APP_DBG_SIG("CD_GAME_POWER_RELEASE\n");
		if (power.state != POWER_STATE_CHARGE)
		{
			break;
		}
		power.state = POWER_STATE_LOCKED;
		task_post_pure_msg(CD_GAME_MATCH_ID, CD_GAME_MATCH_THROW);
	}
	break;

	default:
		break;
	}
}

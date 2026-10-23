#include "cd_game_dog.h"

cd_game_dog_t dog;

void cd_game_dog_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case CD_GAME_DOG_SETUP:
	{
		APP_DBG_SIG("CD_GAME_DOG_SETUP\n");
		dog.x = DOG_AXIS_X;
		dog.y = DOG_AXIS_Y;
		dog.hp = DOG_HP_MAX;
		dog.angle = CD_GAME_ANGLE_DEFAULT;
		dog.hit_ticks = 0;
	}
	break;

	case CD_GAME_DOG_AIM_UP:
	{
		APP_DBG_SIG("CD_GAME_DOG_AIM_UP\n");
		if (dog.angle + CD_GAME_ANGLE_STEP <= CD_GAME_ANGLE_MAX)
		{
			dog.angle += CD_GAME_ANGLE_STEP;
		}
	}
	break;

	case CD_GAME_DOG_AIM_DOWN:
	{
		APP_DBG_SIG("CD_GAME_DOG_AIM_DOWN\n");
		if (dog.angle >= CD_GAME_ANGLE_MIN + CD_GAME_ANGLE_STEP)
		{
			dog.angle -= CD_GAME_ANGLE_STEP;
		}
	}
	break;

	case CD_GAME_DOG_HIT:
	{
		APP_DBG_SIG("CD_GAME_DOG_HIT\n");
		dog.hp = (dog.hp > CD_GAME_HIT_DAMAGE) ? (dog.hp - CD_GAME_HIT_DAMAGE) : 0;
		dog.hit_ticks = DOG_HIT_TICKS;
	}
	break;

	case CD_GAME_DOG_RUN:
	{
		if (dog.hit_ticks > 0)
		{
			dog.hit_ticks--;
		}
	}
	break;

	default:
		break;
	}
}

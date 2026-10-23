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
	}
	break;

	default:
		break;
	}
}

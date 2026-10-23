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
	}
	break;

	default:
		break;
	}
}

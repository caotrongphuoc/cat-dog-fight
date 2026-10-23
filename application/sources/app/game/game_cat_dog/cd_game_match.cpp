#include "cd_game_match.h"

cd_game_match_t match;

void cd_game_match_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case CD_GAME_MATCH_SETUP:
	{
		APP_DBG_SIG("CD_GAME_MATCH_SETUP\n");
		match.state = MATCH_STATE_PLAY;
		match.turn = MATCH_TURN_CAT;
	}
	break;

	default:
		break;
	}
}

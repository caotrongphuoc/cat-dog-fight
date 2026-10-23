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
		match.phase = MATCH_PHASE_AIM;
	}
	break;

	case CD_GAME_MATCH_THROW:
	{
		APP_DBG_SIG("CD_GAME_MATCH_THROW\n");
		if (match.state != MATCH_STATE_PLAY || match.phase != MATCH_PHASE_AIM)
		{
			break;
		}
		match.phase = MATCH_PHASE_FLY;
		task_post_pure_msg(CD_GAME_MISSILE_ID, CD_GAME_MISSILE_THROW);
	}
	break;

	case CD_GAME_MATCH_TURN_END:
	{
		APP_DBG_SIG("CD_GAME_MATCH_TURN_END\n");
		if (match.phase != MATCH_PHASE_FLY)
		{
			break;
		}
		match.turn = (match.turn == MATCH_TURN_CAT) ? MATCH_TURN_DOG : MATCH_TURN_CAT;
		match.phase = MATCH_PHASE_AIM;
		task_post_pure_msg(CD_GAME_POWER_ID, CD_GAME_POWER_SETUP);
		task_post_pure_msg(CD_GAME_WIND_ID, CD_GAME_WIND_CHANGE);
	}
	break;

	default:
		break;
	}
}

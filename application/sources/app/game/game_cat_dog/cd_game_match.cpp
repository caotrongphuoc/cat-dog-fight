#include "cd_game_match.h"

cd_game_match_t match;

// Test builds (APP_SILENT_EN) stay silent whatever the saved setting says
static void cd_game_match_apply_sound()
{
#if defined(APP_SILENT_EN)
	BUZZER_Silent(BUZZER_SILENT_ON);
#else
	BUZZER_Silent(match.record.silent ? BUZZER_SILENT_ON : BUZZER_SILENT_OFF);
#endif
}

// This board decides how a turn ends: every turn against the computer, only its own over RF24
static bool cd_game_match_is_local(uint8_t turn)
{
	return (match.mode == MATCH_MODE_CPU) || (turn == CD_GAME_BOARD_ME);
}

// Hand the turn to the computer when it plays the other animal
static void cd_game_match_cpu_turn()
{
	if (match.mode == MATCH_MODE_CPU && match.turn != CD_GAME_BOARD_ME)
	{
		task_post_pure_msg(CD_GAME_CPU_ID, CD_GAME_CPU_PLAY);
	}
}

// Next turn, or game over when a player has no HP left
static void cd_game_match_next_turn(uint8_t cat_hp, uint8_t dog_hp)
{
	match.rival_landed = false;
	match.rival_result = false;
	if (cat_hp == 0 || dog_hp == 0)
	{
		match.state = MATCH_STATE_OVER;
		match.winner = (cat_hp == 0) ? MATCH_TURN_DOG : MATCH_TURN_CAT;
		if (match.winner == MATCH_TURN_CAT)
		{
			match.record.cat_wins++;
		}
		else
		{
			match.record.dog_wins++;
		}
		cd_game_record_write(&match.record);
		BUZZER_PlaySound(match.winner == CD_GAME_BOARD_ME ? BUZZER_SOUND_HIGHSCORE : BUZZER_SOUND_LOWSCORE);
		timer_set(AC_TASK_DISPLAY_ID, CD_GAME_EXIT_GAME, CD_GAME_TIME_EXIT_INTERVAL, TIMER_ONE_SHOT);
		return;
	}
	match.turn = (match.turn == MATCH_TURN_CAT) ? MATCH_TURN_DOG : MATCH_TURN_CAT;
	match.phase = MATCH_PHASE_AIM;
	task_post_pure_msg(CD_GAME_POWER_ID, CD_GAME_POWER_SETUP);
	task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_RENDER_SCREEN);
	cd_game_match_cpu_turn();
}

// Rival turn ends once its result is in, the thrower's HP and wind win over the local copy
static void cd_game_match_rival_turn_end()
{
	task_post_common_msg(CD_GAME_CAT_ID, CD_GAME_CAT_HP_SET, &match.result_cat_hp, sizeof(match.result_cat_hp));
	task_post_common_msg(CD_GAME_DOG_ID, CD_GAME_DOG_HP_SET, &match.result_dog_hp, sizeof(match.result_dog_hp));
	task_post_common_msg(CD_GAME_WIND_ID, CD_GAME_WIND_SET, (uint8_t*)&match.result_wind, sizeof(match.result_wind));
	cd_game_match_next_turn(match.result_cat_hp, match.result_dog_hp);
}

void cd_game_match_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case CD_GAME_MATCH_RECORD_LOAD:
	{
		APP_DBG_SIG("CD_GAME_MATCH_RECORD_LOAD\n");
		cd_game_record_read(&match.record);
		cd_game_match_apply_sound();
		BUZZER_PlaySound(BUZZER_SOUND_STARTUP);
	}
	break;

	case CD_GAME_MATCH_SOUND_TOGGLE:
	{
		APP_DBG_SIG("CD_GAME_MATCH_SOUND_TOGGLE\n");
		match.record.silent = !match.record.silent;
		cd_game_record_write(&match.record);
		cd_game_match_apply_sound();
	}
	break;

	case CD_GAME_MATCH_LEVEL_TOGGLE:
	{
		APP_DBG_SIG("CD_GAME_MATCH_LEVEL_TOGGLE\n");
		match.record.level = (match.record.level == MATCH_LEVEL_EASY) ? MATCH_LEVEL_HARD : MATCH_LEVEL_EASY;
		cd_game_record_write(&match.record);
	}
	break;

	case CD_GAME_MATCH_MODE_SET:
	{
		APP_DBG_SIG("CD_GAME_MATCH_MODE_SET\n");
		if (get_data_len_common_msg(msg) != sizeof(uint8_t))
		{
			break;
		}
		uint8_t mode = *get_data_common_msg(msg);
		if (mode == MATCH_MODE_CPU || mode == MATCH_MODE_RF)
		{
			match.mode = mode;
		}
	}
	break;

	case CD_GAME_MATCH_SETUP:
	{
		APP_DBG_SIG("CD_GAME_MATCH_SETUP\n");
		match.state = MATCH_STATE_WAIT;
		match.turn = MATCH_TURN_CAT;
		match.phase = MATCH_PHASE_AIM;
		match.winner = MATCH_TURN_CAT;
		match.rival_landed = false;
		match.rival_result = false;
		if (match.mode == MATCH_MODE_RF)
		{
			task_post_pure_msg(CD_GAME_LINK_ID, CD_GAME_LINK_SETUP);
			break;
		}
		// Against the computer the match starts right away
		match.state = MATCH_STATE_PLAY;
		BUZZER_PlaySound(BUZZER_SOUND_LETS_GO);
		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_RENDER_SCREEN);
		cd_game_match_cpu_turn();
	}
	break;

	case CD_GAME_MATCH_START:
	{
		APP_DBG_SIG("CD_GAME_MATCH_START\n");
		if (match.state == MATCH_STATE_WAIT)
		{
			match.state = MATCH_STATE_PLAY;
			BUZZER_PlaySound(BUZZER_SOUND_LETS_GO);
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_RENDER_SCREEN);
		}
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
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		task_post_pure_msg(CD_GAME_MISSILE_ID, CD_GAME_MISSILE_THROW);
		task_post_pure_msg(AC_TASK_DISPLAY_ID, CD_GAME_ANIMATE);
		if (match.mode == MATCH_MODE_RF && match.turn == CD_GAME_BOARD_ME)
		{
			task_post_pure_msg(CD_GAME_LINK_ID, CD_GAME_LINK_SEND_THROW);
		}
	}
	break;

	case CD_GAME_MATCH_TURN_END:
	{
		APP_DBG_SIG("CD_GAME_MATCH_TURN_END\n");
		if (match.state != MATCH_STATE_PLAY || match.phase != MATCH_PHASE_FLY)
		{
			break;
		}
		if (!cd_game_match_is_local(match.turn))
		{
			match.rival_landed = true;
			if (match.rival_result)
			{
				cd_game_match_rival_turn_end();
			}
			break;
		}
		// This board decides: it picks the next wind and tells the rival board how the throw ended
		if (cat.hp != 0 && dog.hp != 0)
		{
			task_post_pure_msg(CD_GAME_WIND_ID, CD_GAME_WIND_CHANGE);
		}
		if (match.mode == MATCH_MODE_RF)
		{
			task_post_pure_msg(CD_GAME_LINK_ID, CD_GAME_LINK_SEND_RESULT);
		}
		cd_game_match_next_turn(cat.hp, dog.hp);
	}
	break;

	case CD_GAME_MATCH_RESULT:
	{
		APP_DBG_SIG("CD_GAME_MATCH_RESULT\n");
		if (match.state != MATCH_STATE_PLAY || match.turn == CD_GAME_BOARD_ME || get_data_len_common_msg(msg) != sizeof(cd_game_link_result_t))
		{
			break;
		}
		cd_game_link_result_t* result = (cd_game_link_result_t*)get_data_common_msg(msg);
		match.result_cat_hp = result->cat_hp;
		match.result_dog_hp = result->dog_hp;
		match.result_wind = result->wind;
		match.rival_result = true;
		// Wait for the local flight to land, unless the THROW never arrived
		if (match.rival_landed || match.phase == MATCH_PHASE_AIM)
		{
			cd_game_match_rival_turn_end();
		}
	}
	break;

	case CD_GAME_MATCH_LINK_LOST:
	{
		APP_DBG_SIG("CD_GAME_MATCH_LINK_LOST\n");
		if (match.state != MATCH_STATE_PLAY)
		{
			break;
		}
		match.state = MATCH_STATE_LOST;
		BUZZER_PlaySound(BUZZER_SOUND_GOODBYE);
		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_RENDER_SCREEN);
		timer_set(AC_TASK_DISPLAY_ID, CD_GAME_EXIT_GAME, CD_GAME_TIME_EXIT_INTERVAL, TIMER_ONE_SHOT);
	}
	break;

	case CD_GAME_MATCH_RESET:
	{
		APP_DBG_SIG("CD_GAME_MATCH_RESET\n");
		match.state = MATCH_STATE_OFF;
		match.phase = MATCH_PHASE_AIM;
	}
	break;

	default:
		break;
	}
}

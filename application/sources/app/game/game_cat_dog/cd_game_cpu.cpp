#include "cd_game_cpu.h"

cd_game_cpu_t cpu;

static bool cd_game_cpu_turn()
{
	return (match.mode == MATCH_MODE_CPU) && (match.state == MATCH_STATE_PLAY) && (match.phase == MATCH_PHASE_AIM) && (match.turn != CD_GAME_BOARD_ME);
}

// Try every angle and power with the game's own flight, keep the angle with the widest hitting power window
static void cd_game_cpu_plan()
{
	uint8_t target = (match.turn == MATCH_TURN_CAT) ? MISSILE_RESULT_HIT_DOG : MISSILE_RESULT_HIT_CAT;
	uint8_t best_width = 0;

	cpu.angle = CPU_FALLBACK_ANGLE;
	cpu.hit_from = CPU_FALLBACK_POWER;
	cpu.hit_to = CPU_FALLBACK_POWER;

	for (uint8_t angle = CPU_ANGLE_MIN; angle <= CPU_ANGLE_MAX; angle += CD_GAME_ANGLE_STEP)
	{
		int16_t run_from = -1;
		for (uint8_t value = 0; value <= POWER_VALUE_MAX; value += CPU_POWER_STEP)
		{
			bool hit = (cd_game_missile_predict(match.turn, angle, value, wind.speed) == target);
			if (hit && run_from < 0)
			{
				run_from = value;
			}
			bool run_ends = !hit || (value + CPU_POWER_STEP > POWER_VALUE_MAX);
			if (run_from >= 0 && run_ends)
			{
				uint8_t run_to = hit ? value : (value - CPU_POWER_STEP);
				if (run_to - run_from + CPU_POWER_STEP > best_width)
				{
					best_width = run_to - run_from + CPU_POWER_STEP;
					cpu.angle = angle;
					cpu.hit_from = run_from;
					cpu.hit_to = run_to;
				}
				run_from = -1;
			}
		}
	}

	int16_t error = (match.record.level == MATCH_LEVEL_HARD) ? CPU_POWER_ERROR_HARD : CPU_POWER_ERROR_EASY;
	int16_t value = (cpu.hit_from + cpu.hit_to) / 2 + (rand() % (2 * error + 1)) - error;
	cpu.power = (value < 0) ? 0 : ((value > POWER_VALUE_MAX) ? POWER_VALUE_MAX : value);
}

void cd_game_cpu_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case CD_GAME_CPU_PLAY:
	{
		APP_DBG_SIG("CD_GAME_CPU_PLAY\n");
		if (!cd_game_cpu_turn())
		{
			break;
		}
		cd_game_cpu_plan();
		if (match.turn == MATCH_TURN_CAT)
		{
			task_post_common_msg(CD_GAME_CAT_ID, CD_GAME_CAT_AIM_SET, &cpu.angle, sizeof(cpu.angle));
		}
		else
		{
			task_post_common_msg(CD_GAME_DOG_ID, CD_GAME_DOG_AIM_SET, &cpu.angle, sizeof(cpu.angle));
		}
		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_RENDER_SCREEN);
		timer_set(CD_GAME_CPU_ID, CD_GAME_CPU_THINK, CD_GAME_CPU_THINK_INTERVAL, TIMER_ONE_SHOT);
	}
	break;

	case CD_GAME_CPU_THINK:
	{
		APP_DBG_SIG("CD_GAME_CPU_THINK\n");
		if (!cd_game_cpu_turn())
		{
			break;
		}
		task_post_common_msg(CD_GAME_POWER_ID, CD_GAME_POWER_SET, &cpu.power, sizeof(cpu.power));
		task_post_pure_msg(CD_GAME_MATCH_ID, CD_GAME_MATCH_THROW);
	}
	break;

	case CD_GAME_CPU_RESET:
	{
		APP_DBG_SIG("CD_GAME_CPU_RESET\n");
		timer_remove_attr(CD_GAME_CPU_ID, CD_GAME_CPU_THINK);
	}
	break;

	default:
		break;
	}
}

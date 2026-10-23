#include "cd_game_missile.h"

cd_game_missile_t missile;

// cos(angle) * 256 for CD_GAME_ANGLE_MIN..CD_GAME_ANGLE_MAX in CD_GAME_ANGLE_STEP
static const int16_t missile_cos_table[] = {247, 241, 232, 222, 210, 196, 181, 165, 147, 128, 108, 88, 66};

int16_t cd_game_missile_cos(uint8_t angle)
{
	return missile_cos_table[(angle - CD_GAME_ANGLE_MIN) / CD_GAME_ANGLE_STEP];
}

int16_t cd_game_missile_sin(uint8_t angle)
{
	return cd_game_missile_cos(90 - angle);
}

static bool cd_game_missile_overlap(int16_t x, int16_t y, uint8_t w, uint8_t h, int16_t rx, int16_t ry, uint8_t rw, uint8_t rh)
{
	return (x < rx + rw) && (rx < x + w) && (y < ry + rh) && (ry < y + h);
}

static void cd_game_missile_launch(cd_game_missile_t* m, uint8_t thrower, uint8_t angle, uint8_t power_value)
{
	int32_t speed = MISSILE_SPEED_MIN + (int32_t)(MISSILE_SPEED_MAX - MISSILE_SPEED_MIN) * power_value / POWER_VALUE_MAX;

	if (thrower == MATCH_TURN_CAT)
	{
		m->type = MISSILE_TYPE_FISH;
		m->x = (int32_t)(cat.x + CAT_SIZE_BITMAP_X) << MISSILE_FIXED_SHIFT;
		m->y = (int32_t)(cat.y + CAT_HAND_OFFSET_Y) << MISSILE_FIXED_SHIFT;
		m->vx = (speed * cd_game_missile_cos(angle)) >> MISSILE_FIXED_SHIFT;
	}
	else
	{
		m->type = MISSILE_TYPE_BONE;
		m->x = (int32_t)(dog.x - MISSILE_BONE_SIZE_BITMAP_X) << MISSILE_FIXED_SHIFT;
		m->y = (int32_t)(dog.y + DOG_HAND_OFFSET_Y) << MISSILE_FIXED_SHIFT;
		m->vx = -((speed * cd_game_missile_cos(angle)) >> MISSILE_FIXED_SHIFT);
	}
	m->vy = -((speed * cd_game_missile_sin(angle)) >> MISSILE_FIXED_SHIFT);
	m->state = MISSILE_STATE_FLY;
	m->result = MISSILE_RESULT_NONE;
}

static uint8_t cd_game_missile_check(uint8_t type, int16_t x, int16_t y, uint8_t w, uint8_t h)
{
	if (cd_game_missile_overlap(x, y, w, h, border.x, border.y, border.width, border.height))
	{
		return MISSILE_RESULT_HIT_BORDER;
	}
	if (type == MISSILE_TYPE_FISH && cd_game_missile_overlap(x, y, w, h, dog.x, dog.y, DOG_SIZE_BITMAP_X, DOG_SIZE_BITMAP_Y))
	{
		return MISSILE_RESULT_HIT_DOG;
	}
	if (type == MISSILE_TYPE_BONE && cd_game_missile_overlap(x, y, w, h, cat.x, cat.y, CAT_SIZE_BITMAP_X, CAT_SIZE_BITMAP_Y))
	{
		return MISSILE_RESULT_HIT_CAT;
	}
	if (y + h >= CD_GAME_GROUND_AXIS_Y || x + w < 0 || x > LCD_WIDTH)
	{
		return MISSILE_RESULT_MISS;
	}
	return MISSILE_RESULT_NONE;
}

// One tick of flight, returns how it ended or MISSILE_RESULT_NONE while still flying
static uint8_t cd_game_missile_step(cd_game_missile_t* m, int8_t wind_speed)
{
	m->vx += wind_speed * MISSILE_WIND_FORCE;
	m->vy += MISSILE_GRAVITY;
	m->x += m->vx;
	m->y += m->vy;

	bool fish = (m->type == MISSILE_TYPE_FISH);
	uint8_t w = fish ? MISSILE_FISH_SIZE_BITMAP_X : MISSILE_BONE_SIZE_BITMAP_X;
	uint8_t h = fish ? MISSILE_FISH_SIZE_BITMAP_Y : MISSILE_BONE_SIZE_BITMAP_Y;
	return cd_game_missile_check(m->type, m->x >> MISSILE_FIXED_SHIFT, m->y >> MISSILE_FIXED_SHIFT, w, h);
}

uint8_t cd_game_missile_predict(uint8_t thrower, uint8_t angle, uint8_t power_value, int8_t wind_speed)
{
	cd_game_missile_t m;
	cd_game_missile_launch(&m, thrower, angle, power_value);
	for (uint16_t tick = 0; tick < MISSILE_PREDICT_TICKS_MAX; tick++)
	{
		uint8_t result = cd_game_missile_step(&m, wind_speed);
		if (result != MISSILE_RESULT_NONE)
		{
			return result;
		}
	}
	return MISSILE_RESULT_MISS;
}

void cd_game_missile_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case CD_GAME_MISSILE_SETUP:
	{
		APP_DBG_SIG("CD_GAME_MISSILE_SETUP\n");
		missile.state = MISSILE_STATE_IDLE;
		missile.result = MISSILE_RESULT_NONE;
	}
	break;

	case CD_GAME_MISSILE_THROW:
	{
		APP_DBG_SIG("CD_GAME_MISSILE_THROW\n");
		cd_game_missile_launch(&missile, match.turn, (match.turn == MATCH_TURN_CAT) ? cat.angle : dog.angle, power.value);
	}
	break;

	case CD_GAME_MISSILE_RUN:
	{
		if (missile.state != MISSILE_STATE_FLY)
		{
			break;
		}
		missile.result = cd_game_missile_step(&missile, wind.speed);
		if (missile.result == MISSILE_RESULT_NONE)
		{
			break;
		}

		missile.state = MISSILE_STATE_IDLE;
		if (missile.result == MISSILE_RESULT_HIT_CAT || missile.result == MISSILE_RESULT_HIT_DOG)
		{
			BUZZER_PlaySound(BUZZER_SOUND_BANG);
		}
		else if (missile.result == MISSILE_RESULT_HIT_BORDER)
		{
			BUZZER_PlaySound(BUZZER_SOUND_TONE_1);
		}

		if (missile.result == MISSILE_RESULT_HIT_CAT)
		{
			task_post_pure_msg(CD_GAME_CAT_ID, CD_GAME_CAT_HIT);
		}
		else if (missile.result == MISSILE_RESULT_HIT_DOG)
		{
			task_post_pure_msg(CD_GAME_DOG_ID, CD_GAME_DOG_HIT);
		}
		else if (missile.result == MISSILE_RESULT_HIT_BORDER)
		{
			task_post_pure_msg(CD_GAME_BORDER_ID, CD_GAME_BORDER_HIT);
		}
		task_post_pure_msg(CD_GAME_MATCH_ID, CD_GAME_MATCH_TURN_END);
	}
	break;

	default:
		break;
	}
}

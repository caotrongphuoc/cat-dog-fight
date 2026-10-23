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

static void cd_game_missile_throw()
{
	uint8_t angle;
	int32_t speed = MISSILE_SPEED_MIN + (int32_t)(MISSILE_SPEED_MAX - MISSILE_SPEED_MIN) * power.value / POWER_VALUE_MAX;

	if (match.turn == MATCH_TURN_CAT)
	{
		angle = cat.angle;
		missile.type = MISSILE_TYPE_FISH;
		missile.x = (int32_t)(cat.x + CAT_SIZE_BITMAP_X) << MISSILE_FIXED_SHIFT;
		missile.y = (int32_t)(cat.y + CAT_HAND_OFFSET_Y) << MISSILE_FIXED_SHIFT;
		missile.vx = (speed * cd_game_missile_cos(angle)) >> MISSILE_FIXED_SHIFT;
	}
	else
	{
		angle = dog.angle;
		missile.type = MISSILE_TYPE_BONE;
		missile.x = (int32_t)(dog.x - MISSILE_BONE_SIZE_BITMAP_X) << MISSILE_FIXED_SHIFT;
		missile.y = (int32_t)(dog.y + DOG_HAND_OFFSET_Y) << MISSILE_FIXED_SHIFT;
		missile.vx = -((speed * cd_game_missile_cos(angle)) >> MISSILE_FIXED_SHIFT);
	}
	missile.vy = -((speed * cd_game_missile_sin(angle)) >> MISSILE_FIXED_SHIFT);
	missile.state = MISSILE_STATE_FLY;
	missile.result = MISSILE_RESULT_NONE;
}

static uint8_t cd_game_missile_check(int16_t x, int16_t y, uint8_t w, uint8_t h)
{
	if (cd_game_missile_overlap(x, y, w, h, border.x, border.y, border.width, border.height))
	{
		return MISSILE_RESULT_HIT_BORDER;
	}
	if (missile.type == MISSILE_TYPE_FISH && cd_game_missile_overlap(x, y, w, h, dog.x, dog.y, DOG_SIZE_BITMAP_X, DOG_SIZE_BITMAP_Y))
	{
		return MISSILE_RESULT_HIT_DOG;
	}
	if (missile.type == MISSILE_TYPE_BONE && cd_game_missile_overlap(x, y, w, h, cat.x, cat.y, CAT_SIZE_BITMAP_X, CAT_SIZE_BITMAP_Y))
	{
		return MISSILE_RESULT_HIT_CAT;
	}
	if (y + h >= CD_GAME_GROUND_AXIS_Y || x + w < 0 || x > LCD_WIDTH)
	{
		return MISSILE_RESULT_MISS;
	}
	return MISSILE_RESULT_NONE;
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
		cd_game_missile_throw();
	}
	break;

	case CD_GAME_MISSILE_RUN:
	{
		if (missile.state != MISSILE_STATE_FLY)
		{
			break;
		}
		missile.vx += wind.speed * MISSILE_WIND_FORCE;
		missile.vy += MISSILE_GRAVITY;
		missile.x += missile.vx;
		missile.y += missile.vy;

		bool fish = (missile.type == MISSILE_TYPE_FISH);
		uint8_t w = fish ? MISSILE_FISH_SIZE_BITMAP_X : MISSILE_BONE_SIZE_BITMAP_X;
		uint8_t h = fish ? MISSILE_FISH_SIZE_BITMAP_Y : MISSILE_BONE_SIZE_BITMAP_Y;
		missile.result = cd_game_missile_check(missile.x >> MISSILE_FIXED_SHIFT, missile.y >> MISSILE_FIXED_SHIFT, w, h);
		if (missile.result == MISSILE_RESULT_NONE)
		{
			break;
		}

		missile.state = MISSILE_STATE_IDLE;
		if (missile.result == MISSILE_RESULT_HIT_CAT)
		{
			task_post_pure_msg(CD_GAME_CAT_ID, CD_GAME_CAT_HIT);
		}
		else if (missile.result == MISSILE_RESULT_HIT_DOG)
		{
			task_post_pure_msg(CD_GAME_DOG_ID, CD_GAME_DOG_HIT);
		}
		task_post_pure_msg(CD_GAME_MATCH_ID, CD_GAME_MATCH_TURN_END);
	}
	break;

	default:
		break;
	}
}

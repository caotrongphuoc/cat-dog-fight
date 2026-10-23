#ifndef __CD_GAME_LINK_H__
#define __CD_GAME_LINK_H__

#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "app_data.h"
#include "task_list.h"
#include "task_display.h"

#include "cd_game_match.h"
#include "cd_game_cat.h"
#include "cd_game_dog.h"
#include "cd_game_wind.h"
#include "cd_game_power.h"
#include "cd_game_missile.h"

// Payload of START, sent by the cat board when both boards are ready
typedef struct
{
	int8_t wind;
} __attribute__((packed)) cd_game_link_start_t;

// Payload of THROW, sent by the player who just released MODE
typedef struct
{
	uint8_t angle;
	uint8_t power;
} __attribute__((packed)) cd_game_link_throw_t;

// Payload of RESULT, sent by the thrower once its missile has landed
typedef struct
{
	uint8_t result;
	uint8_t cat_hp;
	uint8_t dog_hp;
	int8_t wind;
} __attribute__((packed)) cd_game_link_result_t;

typedef struct
{
	uint16_t tx;
	uint16_t rx;
	uint8_t last_rx_sig;
} cd_game_link_t;

extern cd_game_link_t link;

#endif //__CD_GAME_LINK_H__

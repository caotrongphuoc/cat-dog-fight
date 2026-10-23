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
#include "sys_ctrl.h"
#include "task_list.h"
#include "task_display.h"

#include "cd_game_match.h"
#include "cd_game_cat.h"
#include "cd_game_dog.h"
#include "cd_game_wind.h"
#include "cd_game_power.h"
#include "cd_game_missile.h"

// PING is sent every LINK_PING_TICKS link ticks while playing
#define LINK_PING_TICKS (2)

// No message from the rival for this long while playing means the link is lost
#define LINK_LOST_TIMEOUT (6000)

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
	uint32_t last_rx_ms;
	uint8_t ticks;

	// The rival sent something that only a playing board sends
	bool rival_playing;

	// RESULT is sent again every tick until the rival acks it
	bool result_pending;
	cd_game_link_result_t result;
} cd_game_link_t;

extern cd_game_link_t link;

#endif //__CD_GAME_LINK_H__

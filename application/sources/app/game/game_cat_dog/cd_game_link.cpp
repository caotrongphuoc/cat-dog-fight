#include "cd_game_link.h"

cd_game_link_t link;

static void cd_game_link_send_pure(uint8_t sig)
{
	ak_msg_t* s_msg = get_pure_msg();

	set_if_src_task_id(s_msg, CD_GAME_LINK_ID);
	set_if_des_task_id(s_msg, CD_GAME_LINK_ID);
	set_if_src_type(s_msg, IF_TYPE_RF24_ME);
	set_if_des_type(s_msg, IF_TYPE_RF24_RIVAL);
	set_if_sig(s_msg, sig);

	set_msg_sig(s_msg, AC_IF_PURE_MSG_OUT);
	task_post(AC_TASK_IF_ID, s_msg);
	link.tx++;
}

static void cd_game_link_send_common(uint8_t sig, uint8_t* data, uint8_t len)
{
	ak_msg_t* s_msg = get_common_msg();

	set_if_src_task_id(s_msg, CD_GAME_LINK_ID);
	set_if_des_task_id(s_msg, CD_GAME_LINK_ID);
	set_if_src_type(s_msg, IF_TYPE_RF24_ME);
	set_if_des_type(s_msg, IF_TYPE_RF24_RIVAL);
	set_if_sig(s_msg, sig);
	set_if_data_common_msg(s_msg, data, len);

	set_msg_sig(s_msg, AC_IF_COMMON_MSG_OUT);
	task_post(AC_TASK_IF_ID, s_msg);
	link.tx++;
}

// Any message from the rival keeps the link alive
static void cd_game_link_received(ak_msg_t* msg, bool from_playing_rival)
{
	link.rx++;
	link.last_rx_sig = msg->sig;
	link.last_rx_ms = sys_ctrl_millis();
	if (from_playing_rival)
	{
		link.rival_playing = true;
	}
}

void cd_game_link_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case CD_GAME_LINK_SETUP:
	{
		APP_DBG_SIG("CD_GAME_LINK_SETUP\n");
		link.tx = 0;
		link.rx = 0;
		link.last_rx_sig = 0;
		link.last_rx_ms = sys_ctrl_millis();
		link.ticks = 0;
		link.rival_playing = false;
		link.result_pending = false;
		cd_game_link_send_pure(CD_GAME_LINK_RX_READY);
		timer_set(CD_GAME_LINK_ID, CD_GAME_LINK_TICK, CD_GAME_LINK_TICK_INTERVAL, TIMER_PERIODIC);
	}
	break;

	case CD_GAME_LINK_TICK:
	{
		link.ticks++;
		if (match.state == MATCH_STATE_WAIT)
		{
			cd_game_link_send_pure(CD_GAME_LINK_RX_READY);
			break;
		}
		if (link.result_pending)
		{
			cd_game_link_send_common(CD_GAME_LINK_RX_RESULT, (uint8_t*)&link.result, sizeof(link.result));
		}
		if (match.state != MATCH_STATE_PLAY)
		{
			break;
		}
		if (link.ticks % LINK_PING_TICKS == 0)
		{
			cd_game_link_send_pure(CD_GAME_LINK_RX_PING);
		}
		if (sys_ctrl_millis() - link.last_rx_ms > LINK_LOST_TIMEOUT)
		{
			task_post_pure_msg(CD_GAME_MATCH_ID, CD_GAME_MATCH_LINK_LOST);
		}
	}
	break;

	case CD_GAME_LINK_SEND_START:
	{
		APP_DBG_SIG("CD_GAME_LINK_SEND_START\n");
		link.last_rx_ms = sys_ctrl_millis();
		cd_game_link_start_t start = {wind.speed};
		cd_game_link_send_common(CD_GAME_LINK_RX_START, (uint8_t*)&start, sizeof(start));
	}
	break;

	case CD_GAME_LINK_SEND_THROW:
	{
		APP_DBG_SIG("CD_GAME_LINK_SEND_THROW\n");
		cd_game_link_throw_t throw_data = {(CD_GAME_BOARD_ME == MATCH_TURN_CAT) ? cat.angle : dog.angle, power.value};
		cd_game_link_send_common(CD_GAME_LINK_RX_THROW, (uint8_t*)&throw_data, sizeof(throw_data));
	}
	break;

	case CD_GAME_LINK_SEND_RESULT:
	{
		APP_DBG_SIG("CD_GAME_LINK_SEND_RESULT\n");
		link.result = {missile.result, cat.hp, dog.hp, wind.speed};
		link.result_pending = true;
		cd_game_link_send_common(CD_GAME_LINK_RX_RESULT, (uint8_t*)&link.result, sizeof(link.result));
	}
	break;

	case CD_GAME_LINK_RESET:
	{
		APP_DBG_SIG("CD_GAME_LINK_RESET\n");
		link.result_pending = false;
		timer_remove_attr(CD_GAME_LINK_ID, CD_GAME_LINK_TICK);
	}
	break;

	case CD_GAME_LINK_RX_READY:
	{
		APP_DBG_SIG("CD_GAME_LINK_RX_READY\n");
		cd_game_link_received(msg, false);
		// READY from a rival that was already playing: it left the match and came back
		if (match.state == MATCH_STATE_PLAY && link.rival_playing)
		{
			task_post_pure_msg(CD_GAME_MATCH_ID, CD_GAME_MATCH_LINK_LOST);
			break;
		}
		if (CD_GAME_BOARD_ME != MATCH_TURN_CAT)
		{
			break;
		}
		// The cat board opens the match, and sends START again if the dog missed it
		if (match.state == MATCH_STATE_WAIT)
		{
			task_post_pure_msg(CD_GAME_WIND_ID, CD_GAME_WIND_SETUP);
			task_post_pure_msg(CD_GAME_LINK_ID, CD_GAME_LINK_SEND_START);
			task_post_pure_msg(CD_GAME_MATCH_ID, CD_GAME_MATCH_START);
		}
		else if (match.state == MATCH_STATE_PLAY)
		{
			task_post_pure_msg(CD_GAME_LINK_ID, CD_GAME_LINK_SEND_START);
		}
	}
	break;

	case CD_GAME_LINK_RX_START:
	{
		APP_DBG_SIG("CD_GAME_LINK_RX_START\n");
		cd_game_link_received(msg, false);
		if (CD_GAME_BOARD_ME == MATCH_TURN_CAT || match.state != MATCH_STATE_WAIT || get_data_len_common_msg(msg) != sizeof(cd_game_link_start_t))
		{
			break;
		}
		cd_game_link_start_t* start = (cd_game_link_start_t*)get_data_common_msg(msg);
		task_post_common_msg(CD_GAME_WIND_ID, CD_GAME_WIND_SET, (uint8_t*)&start->wind, sizeof(start->wind));
		task_post_pure_msg(CD_GAME_MATCH_ID, CD_GAME_MATCH_START);
	}
	break;

	case CD_GAME_LINK_RX_THROW:
	{
		APP_DBG_SIG("CD_GAME_LINK_RX_THROW\n");
		cd_game_link_received(msg, true);
		// The rival only throws after it applied our last RESULT
		link.result_pending = false;
		if (match.state != MATCH_STATE_PLAY || match.phase != MATCH_PHASE_AIM || match.turn == CD_GAME_BOARD_ME || get_data_len_common_msg(msg) != sizeof(cd_game_link_throw_t))
		{
			break;
		}
		cd_game_link_throw_t* throw_data = (cd_game_link_throw_t*)get_data_common_msg(msg);
		if (match.turn == MATCH_TURN_CAT)
		{
			task_post_common_msg(CD_GAME_CAT_ID, CD_GAME_CAT_AIM_SET, &throw_data->angle, sizeof(throw_data->angle));
		}
		else
		{
			task_post_common_msg(CD_GAME_DOG_ID, CD_GAME_DOG_AIM_SET, &throw_data->angle, sizeof(throw_data->angle));
		}
		task_post_common_msg(CD_GAME_POWER_ID, CD_GAME_POWER_SET, &throw_data->power, sizeof(throw_data->power));
		task_post_pure_msg(CD_GAME_MATCH_ID, CD_GAME_MATCH_THROW);
	}
	break;

	case CD_GAME_LINK_RX_RESULT:
	{
		APP_DBG_SIG("CD_GAME_LINK_RX_RESULT\n");
		cd_game_link_received(msg, true);
		// Ack every copy, the thrower keeps sending until it hears back
		cd_game_link_send_pure(CD_GAME_LINK_RX_ACK);
		if (match.state != MATCH_STATE_PLAY || match.turn == CD_GAME_BOARD_ME || get_data_len_common_msg(msg) != sizeof(cd_game_link_result_t))
		{
			break;
		}
		task_post_common_msg(CD_GAME_MATCH_ID, CD_GAME_MATCH_RESULT, get_data_common_msg(msg), sizeof(cd_game_link_result_t));
	}
	break;

	case CD_GAME_LINK_RX_PING:
	{
		cd_game_link_received(msg, true);
	}
	break;

	case CD_GAME_LINK_RX_ACK:
	{
		APP_DBG_SIG("CD_GAME_LINK_RX_ACK\n");
		cd_game_link_received(msg, true);
		link.result_pending = false;
	}
	break;

	default:
		break;
	}
}

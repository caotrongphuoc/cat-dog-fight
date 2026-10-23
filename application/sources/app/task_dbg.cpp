#include <string.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_io.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_if.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_list_if.h"
#include "task_dbg.h"

/* send a stamped ping/pong to an rf24 address, the stamp comes back in the pong */
static void task_dbg_rf24_send(uint8_t if_des_type, uint8_t if_sig, uint32_t stamp) {
	ak_msg_t* s_msg = get_common_msg();

	set_if_src_task_id(s_msg, AC_TASK_DBG_ID);
	set_if_des_task_id(s_msg, AC_TASK_DBG_ID);
	set_if_src_type(s_msg, IF_TYPE_RF24_ME);
	set_if_des_type(s_msg, if_des_type);
	set_if_sig(s_msg, if_sig);
	set_if_data_common_msg(s_msg, (uint8_t*)&stamp, sizeof(stamp));

	set_msg_sig(s_msg, AC_IF_COMMON_MSG_OUT);
	task_post(AC_TASK_IF_ID, s_msg);
}

void task_dbg(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_DBG_RF24_PING_REQ: {
		APP_DBG_SIG("AC_DBG_RF24_PING_REQ\n");
		uint32_t stamp = sys_ctrl_millis();
		task_dbg_rf24_send(IF_TYPE_RF24_RIVAL, AC_DBG_RF24_PING, stamp);
		LOGIN_PRINT("rf ping -> %d stamp %d\n", IF_TYPE_RF24_RIVAL, stamp);
	}
		break;

	case AC_DBG_RF24_PING: {
		APP_DBG_SIG("AC_DBG_RF24_PING\n");
		if (get_data_len_common_msg(msg) != sizeof(uint32_t)) {
			LOGIN_PRINT("rf ping bad len %d\n", get_data_len_common_msg(msg));
			break;
		}
		uint32_t stamp;
		memcpy(&stamp, get_data_common_msg(msg), sizeof(stamp));
		task_dbg_rf24_send(msg->if_src_type, AC_DBG_RF24_PONG, stamp);
		LOGIN_PRINT("rf ping <- %d, pong sent\n", msg->if_src_type);
	}
		break;

	case AC_DBG_RF24_PONG: {
		APP_DBG_SIG("AC_DBG_RF24_PONG\n");
		if (get_data_len_common_msg(msg) != sizeof(uint32_t)) {
			LOGIN_PRINT("rf pong bad len %d\n", get_data_len_common_msg(msg));
			break;
		}
		uint32_t stamp;
		memcpy(&stamp, get_data_common_msg(msg), sizeof(stamp));
		LOGIN_PRINT("rf pong <- %d rtt %d ms\n", msg->if_src_type, sys_ctrl_millis() - stamp);
	}
		break;

	default:
		break;
	}
}

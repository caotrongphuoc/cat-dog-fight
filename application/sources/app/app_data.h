#ifndef __APP_DATA_H__
#define __APP_DATA_H__

#include <stdint.h>

#include "sys_boot.h"
#include "sys_dbg.h"
#include "app.h"
#include "buzzer.h"

#if defined (TASK_MBMASTER_EN)
#include "mbport.h"
#include "mbm.h"
#include "common/mbportlayer.h"
#endif

/******************************************************************************
* IF Type
*******************************************************************************/
/** RF24 interface for modules
 *
*/
#define IF_TYPE_RF24_MIN					(0)
#define IF_TYPE_RF24_GW						(0)
#define IF_TYPE_RF24_AC						(1)
#define IF_TYPE_RF24_CAT					(1)
#define IF_TYPE_RF24_DOG					(2)
#define IF_TYPE_RF24_MAX					(99)

/* board role is chosen at build time: make BOARD=cat|dog */
#if defined (CD_GAME_BOARD_DOG)
#define IF_TYPE_RF24_ME						IF_TYPE_RF24_DOG
#define IF_TYPE_RF24_RIVAL					IF_TYPE_RF24_CAT
#else
#define IF_TYPE_RF24_ME						IF_TYPE_RF24_CAT
#define IF_TYPE_RF24_RIVAL					IF_TYPE_RF24_DOG
#endif

/** APP interface, communication via socket interface
 *
 */
#define IF_TYPE_APP_MIN						(100)
#define IF_TYPE_APP_GW						(100)
#define IF_TYPE_APP_GI						(101)
#define IF_TYPE_APP_MAX						(119)

/** UART interface
 *
 */
#define IF_TYPE_UART_GW_MIN					(120)
#define IF_TYPE_UART_GW						(120)
#define IF_TYPE_UART_AC						(121)
#define IF_TYPE_UART_GW_MAX					(140)


typedef struct {
	uint8_t is_power_on_reset;
} boot_app_share_data_t;

/******************************************************************************
* RS485-MODBUS
*******************************************************************************/
#if defined (TASK_MBMASTER_EN)
extern xMBHandle xMBMMaster;
#endif

#endif //__APP_DATA_H__

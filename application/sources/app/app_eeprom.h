#ifndef __APP_EEPROM_H__
#define __APP_EEPROM_H__

#include <stdint.h>
#include "app.h"

/**
  *****************************************************************************
  * EEPROM define address.
  *
  *****************************************************************************
  */
#define EEPROM_START_ADDR								(0X0000)
#define EEPROM_END_ADDR									(0X1000)

/* cat dog fight record, kept clear of 0x0000 and 0x0100 used by other firmwares */
#define EEPROM_CD_GAME_RECORD_ADDR						(0X0200)
#define CD_GAME_EEPROM_MAGIC_NUMBER						((uint32_t)0x63646674) /* "cdft" */

typedef struct {
	uint8_t silent;		/* 1: buzzer muted */
	uint16_t cat_wins;
	uint16_t dog_wins;
} __attribute__((packed)) cd_game_record_t;

#ifdef __cplusplus
extern "C"
{
#endif

extern bool cd_game_record_read(cd_game_record_t* data);
extern bool cd_game_record_write(cd_game_record_t* data);

#ifdef __cplusplus
}
#endif

#endif //__APP_EEPROM_H__

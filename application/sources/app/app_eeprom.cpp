#include "app_eeprom.h"
#include "eeprom.h"

typedef struct {
	uint32_t magic_number;
	cd_game_record_t data;
	uint8_t check_sum;
} __attribute__((packed)) cd_game_record_eeprom_t;

#define CD_GAME_RECORD_CHECKSUM_SIZE	(sizeof(uint32_t) + sizeof(cd_game_record_t))

static uint8_t cd_game_eeprom_checksum(uint8_t* data, uint32_t size) {
	uint8_t check_sum = 0;

	for (uint32_t i = 0; i < size; i++) {
		check_sum += data[i];
	}

	return check_sum;
}

bool cd_game_record_read(cd_game_record_t* data) {
	cd_game_record_eeprom_t eeprom_data;

	uint8_t ret = eeprom_read(EEPROM_CD_GAME_RECORD_ADDR, (uint8_t*)&eeprom_data, sizeof(eeprom_data));

	if (ret == EEPROM_DRIVER_OK &&
		eeprom_data.magic_number == CD_GAME_EEPROM_MAGIC_NUMBER &&
		eeprom_data.check_sum == cd_game_eeprom_checksum((uint8_t*)&eeprom_data, CD_GAME_RECORD_CHECKSUM_SIZE)) {
		*data = eeprom_data.data;
		return true;
	}

	/* first boot or broken record: sound on, no wins */
	data->silent = 0;
	data->cat_wins = 0;
	data->dog_wins = 0;
	return false;
}

bool cd_game_record_write(cd_game_record_t* data) {
	cd_game_record_eeprom_t eeprom_data;

	eeprom_data.magic_number = CD_GAME_EEPROM_MAGIC_NUMBER;
	eeprom_data.data = *data;
	eeprom_data.check_sum = cd_game_eeprom_checksum((uint8_t*)&eeprom_data, CD_GAME_RECORD_CHECKSUM_SIZE);

	return eeprom_write(EEPROM_CD_GAME_RECORD_ADDR, (uint8_t*)&eeprom_data, sizeof(eeprom_data)) == EEPROM_DRIVER_OK;
}

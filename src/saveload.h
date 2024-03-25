#pragma once
#include <Arduboy2.h>
#include "player.h"
#include "item.h"

// EEPROM game layout
// header:   1 byte
// data:     33 bytes
// checksum: 2 bytes
const uint8_t HEADER_VALUE = 0xAD; // ArduDemon
const uint8_t EEPROM_DATA_SIZE = 33;

uint8_t * const EEPROM_HEADER = (uint8_t *)554;
uint8_t * const EEPROM_DATA = EEPROM_HEADER + sizeof(HEADER_VALUE);
uint8_t * const EEPROM_CHECKSUM = EEPROM_DATA + EEPROM_DATA_SIZE;

void clear_save();
void save_game();
bool load_game();
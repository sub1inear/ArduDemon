#pragma once
#include "Arduboy2.h"

#define NUM_ITEM_TYPES 5

const uint8_t PROGMEM gold_image[32] = {
    0x00, 0x92, 0xe8, 0xb0, 0xf2, 0xff, 0xfa, 0xf8, 0xda, 0x77, 0xe2, 0x66, 0xf7, 0x64, 0xc8, 0x00, 
    0x50, 0xf2, 0x67, 0x02, 0x55, 0x3a, 0x9f, 0xce, 0x9f, 0x17, 0xaf, 0x47, 0x02, 0x67, 0x32, 0x18, 
};
const uint8_t PROGMEM sword_image[8] = {
    0xc0, 0xe0, 0x70, 0x38, 0x1d, 0x0e, 0x06, 0x09, 
};
const uint8_t PROGMEM axe_image[8] = {
    0x80, 0x40, 0x20, 0x13, 0x0f, 0x0e, 0x1c, 0x18, 
};

const uint8_t PROGMEM potion_image[8] = {
0x38, 0x55, 0x63, 0x50, 0x63, 0x55, 0x38, 0x00, 
};
const uint8_t PROGMEM shield_image[] = {
    0x3e, 0x41, 0xab, 0xa5, 0x95, 0xab, 0x41, 0x3e, 
};


const uint8_t * const PROGMEM loot_lut[NUM_ITEM_TYPES] = {
    gold_image,
    potion_image,
    sword_image,
    axe_image,
    shield_image,
};
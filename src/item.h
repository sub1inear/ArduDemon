#pragma once
#include "Arduboy2.h"

#define MAX_ITEMS 6

#define HEALTH_POTION_HP 20

enum item_t: uint8_t {
    GOLD,
    POTION,
    SWORD,
    AXE,
    SHIELD    
};
enum potion_t: uint8_t {
    SMALL,
    LARGE,
    HUGE,
    REJUV,
};

class Item {
    public:
        int16_t x;
        int16_t y;
        uint8_t type;
        uint8_t size;

        uint8_t magic_stat;
        uint8_t magic_bonus;

        uint8_t pwramt; // power or (in the case of gold) amount

        void init(int16_t x, int16_t y, uint8_t type, uint8_t pwramt, uint8_t magic_stat, uint8_t magic_bonus);
        void draw();
    


};
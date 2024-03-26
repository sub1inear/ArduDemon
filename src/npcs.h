#pragma once
#include "Arduboy2.h"
#include "ArduboyTones.h"

extern ArduboyTones sound;

#define ELDER_X 19
#define ELDER_Y 20

#define BLACKSMITH_X 24
#define BLACKSMITH_Y 17

#define HEALER_X 15
#define HEALER_Y 22

#define INNKEEPER_X 17
#define INNKEEPER_Y 17

#define MAGE_X 27
#define MAGE_Y 6

#define SECRET_X 3
#define SECRET_Y 28

#define NPC_TEXT_X 10
#define NPC_TEXT_Y 5


enum npc_t : uint8_t {
    NONPC,
    ELDER,
    BLACKSMITH,
    HEALER,
    INNKEEPER,
    MAGE,
    SECRET,
};

void update_npc_menu();
void draw_npc_menu();
void draw_overlay();
void draw_npcs();



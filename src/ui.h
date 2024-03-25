#pragma once
#include <Arduboy2.h>

// inv screen

#define SLOTS_X 1
#define SLOTS_Y 1

#define SLOTS_OFFSET 20

#define EQUIPPED_X 110
#define EQUIPPED_Y 0

#define EQUIPPED_OFFSET 20

#define TEXT_BOX_X 0
#define TEXT_BOX_Y 39
#define TEXT_BOX_W 128
#define TEXT_BOX_H 24

#define STATS_BOX_X 59
#define STATS_BOX_Y 0
#define STATS_BOX_W 49
#define STATS_BOX_H 38

// health and mana

#define BAR_HEIGHT 20

// game over screen

#define GAME_OVER_X WIDTH / 2 - (4 * 10 - 1) / 2
#define GAME_OVER_Y HEIGHT / 2 - 5 / 2 - 20

// main menu
#define MAIN_TEXT_X 5
#define MAIN_TEXT_Y 8

#define MAIN_FLAME_X 55 + MAIN_TEXT_X
#define MAIN_FLAME_Y 0

#define MENU_START_X 42
#define MENU_START_Y 35

#define MAIN_CREDIT_X 70
#define MAIN_CREDIT_Y 57

// title screen
#define TITLE_DEMON_X 29
#define TITLE_DEMON_Y 0

#define TITLE_TEXT_X 10
#define TITLE_TEXT_Y 39

#define TITLE_FLAME_X 64
#define TITLE_FLAME_Y 31

// win screen
#define WIN_TEXT_X 35
#define WIN_TEXT_Y 18


enum suboptions_t: uint8_t {
    CHECK,
    CROSS
};

void draw_win_screen();
void draw_title_screen();
void update_main_menu();
void draw_main_menu();
void update_dead_menu();
void draw_dead_menu();
void draw_health_xp();
void update_inv();
void draw_inv();
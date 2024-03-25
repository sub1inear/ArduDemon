#pragma once

#include <Arduboy2.h>
#include <ArduboyTones.h>

#include "map.h"
#include "enemy.h"

#include "item.h"

#define PLAYER_OFFSET_X 56
#define PLAYER_OFFSET_Y 24

#define PLAYER_COLLISION_OFFSET_X 7
#define PLAYER_COLLISION_OFFSET_Y 15

#define FORMAT_BIN8 "%c%c%c%c%c%c%c%c"
#define UINT8_TO_BIN8(b) \
    ((b) & 0x80 ? '1' : '0'), ((b) & 0x40 ? '1' : '0'), ((b) & 0x20 ? '1' : '0'), ((b) & 0x10 ? '1' : '0'), \
    ((b) & 0x08 ? '1' : '0'), ((b) & 0x04 ? '1' : '0'), ((b) & 0x02 ? '1' : '0'), ((b) & 0x01 ? '1' : '0')

#define MAX_INV 6

enum stat_t : uint8_t {
    STR,
    CON,
    DEX,
    MNA,
    NOSTAT,
};


class Player {
public:
    
    TileMap *tile_map;

    int16_t x = 0;
    int16_t y = 0;

    int16_t scrollx = x;
    int16_t scrolly = y;

    int8_t x_axis = 0;
    int8_t y_axis = 0;

    uint8_t frame = 0;
    uint8_t dir = 0;

    uint8_t frame_speed = 8;

    Item weapon;
    Item shield;

    uint8_t stats[4] = {
        10, // Strength
        10, // Constitution
        10, // Dexterity
        10 // Mana
    };

    uint8_t xp = 0;
    uint8_t level = 0;

    uint8_t level_up_timer = 0;

    uint16_t max_hp = stats[CON] * 2;
    uint16_t hp = max_hp;
    bool dead = false;

    Enemy *target;
    uint8_t attacked_timer = 0;
    bool attacking = false;

    uint8_t hold_timer = 0;
    Item inventory[MAX_INV];
    uint8_t num_inv = 0;

    bool on_stairs_down = false;
    bool on_stairs_up = false;

    uint8_t npc_target = 0;
    bool talking = false;
    bool old_talking = false;

    bool wizard_hat = false;

    void init();
    bool is_collidable(uint8_t tile_x, uint8_t tile_y);
    void drop_loot(uint8_t index);
    void teleport();
    void die();
    uint8_t check_npcs(uint8_t tile_x, uint8_t tile_y);
    void update();
    void set_camera();
    void draw();
};
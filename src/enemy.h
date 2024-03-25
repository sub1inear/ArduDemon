#pragma once
#include <Arduboy2.h>
#include <ArduboyTones.h>

enum enemy_t: uint8_t {
    SKELETON, // Undead
    ZOMBIE, // Undead
    ORC, // Brute
    WARWOLF, // Brute
    FALLENONE, // Demon
    GARGOYLE, // Demon
    BOSS, /* One Archdemon to rule them all! */
};


typedef struct {
    int16_t x;
    int16_t y;
    int8_t dx;
    int8_t dy;
    uint8_t timer;
    uint8_t respawn_timer;
    bool flipped;
} Fireball;

class Enemy {
    public:
        int16_t x;
        int16_t y;
        
        uint8_t active = 0;
        uint8_t attack_timer = random(100, 150);

        const uint8_t *images;

        uint8_t dir = 0;
        uint8_t frame = 0;
        
        uint8_t stop = 0;

        enemy_t type;

        uint8_t hp = 0;
        bool dead = false;


        void init(uint8_t tile_x, uint8_t tile_y, const uint8_t *images, enemy_t type);

        void damage_player();
        void update();
        void draw();
        
        
};
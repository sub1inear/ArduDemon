#include "enemy.h"
#include "player.h"
#include "dungeon.h"
#include "enemy_images.h"
#include "tools.h"

extern Arduboy2 arduboy;
extern ArduboyTones sound;

extern Player player;

extern Enemy enemies[NUM_ENEMIES];
extern Fireball boss_fireball;

extern uint8_t win;

using uint24_t = __uint24;



static const uint8_t hp_lut[] PROGMEM = {
    4, // Skeleton
    5, // Zombie
    20, // Brute
    30, // Warwolf
    50, // Fallen One
    80, // Gargoyle
    255, // Boss
};

static const uint8_t enemy_damage_lut[] PROGMEM = {
    3, // Skeleton
    8, // Zombie
    12, // Orc
    16, // Warwolf
    23, // Fallen One
    28, // Gargoyle
    40, // Boss
};

static const uint8_t player_defense_lut[] PROGMEM = {
    1,
    3,
    6,
    10,
};

void Enemy::init(uint8_t tile_x, uint8_t tile_y, const uint8_t *images, enemy_t type) {
    this->x = tile_x * 16;
    this->y = tile_y * 16;
    this->images = images;
    this->type = type;
    this->hp = pgm_read_byte(hp_lut + type);
    this->dead = false;
    this->active = 0;
}

void Enemy::damage_player() {
    sound.tone(NOTE_C1, 50, NOTE_A1, 100);
    uint8_t damage_blocked = (player.stats[2] / 10) + pgm_read_byte(player_defense_lut + player.shield.pwramt);
    uint8_t damage = pgm_read_byte(enemy_damage_lut + type);
    if (damage_blocked >= damage) {
        damage = 0;
    } else {
        damage -= damage_blocked;
    }
    if (player.hp > damage) {
        player.hp -= damage;
        player.attacked_timer = 10;
    } else {
        player.die();
    }
}

void Enemy::update() {
    
    if (!dead && (on_screen(x, y, type == BOSS) || active)) {
        
        if (!active) {
            active = 200;
        } else if (!on_screen(x, y, type == BOSS)) {
            active -= 1;
        }

        if (boss_fireball.timer > 0) {
            boss_fireball.x += boss_fireball.dx;
            boss_fireball.y += boss_fireball.dy;
            boss_fireball.timer -= 1;
            uint16_t distsq = (player.x - boss_fireball.x) * (player.x - boss_fireball.x) +
                            (player.y - boss_fireball.y) * (player.y - boss_fireball.y);
            if (distsq < 16 * 16) {
                damage_player();
                boss_fireball.timer = 0;
            }
        } else if (boss_fireball.respawn_timer > 0) {
            boss_fireball.respawn_timer -= 1;
        }

        // stop enemies from running over the player
        uint16_t distsq = (player.x - x) * (player.x - x) +
                            (player.y - y) * (player.y - y);
        uint8_t boss_offset = (type == BOSS) ? 3 : 0; 
        if (distsq < (16 + boss_offset) * (16 + boss_offset)) { // square other num to remove sqrt
            if (attack_timer) {
                attack_timer -= 1;
            } else {
                damage_player();
                attack_timer = random(100, 150);
            }
            frame = 0;
            return;
        } else { 
            if (distsq < 48 * 48 && type == BOSS && boss_fireball.respawn_timer == 0) {
                boss_fireball.timer = 100;
                boss_fireball.respawn_timer = 25;
                boss_fireball.x = x + 6;
                boss_fireball.y = y + 10;
                boss_fireball.flipped = false;

                if (player.x > x) {
                    boss_fireball.dx = 1;
                } else if (player.x < x) {
                    boss_fireball.dx = -1;
                    boss_fireball.flipped = true;
                } else {
                    boss_fireball.dx = 0;
                }
                if (player.y > y) {
                    boss_fireball.dy = 1;
                } else if (player.y < y) {
                    boss_fireball.dy = -1;
                } else {
                    boss_fireball.dy = 0;
                }
            }
            if (!attack_timer) {
                attack_timer = random(100, 150);
            }
        }
        
        // IMPORTANT! Boss code stops here
        if (type == BOSS) {
            return;
        }
        
        if (random(20) == 0) {
            stop = 10;
            frame = 0;
            return;
        }
        
        if (stop) {
            stop -= 1;
            return;
        }

        if (arduboy.everyXFrames(2)) {
            int8_t x_sign = 0;
            int8_t y_sign = 0;

            if (player.x > x) {
                x_sign = 1;
            } else if (player.x < x) {
                x_sign = -1;
            } else {
                x_sign = 0;
            }

            if (player.y > y) {
                y_sign = 1;
            } else if (player.y < y) {
                y_sign = -1;
            } else {
                y_sign = 0;
            }

            x += x_sign;

            uint8_t tile_x = (x + PLAYER_COLLISION_OFFSET_X) / 16;
            uint8_t tile_y = (y + PLAYER_COLLISION_OFFSET_Y) / 16;
    
            if (bitRead(player.tile_map->source[tile_x + (tile_y / 8) * player.tile_map->width], tile_y & 0x7)) {
                x -= x_sign;
            }
            
            y += y_sign;

            tile_x = (x + PLAYER_COLLISION_OFFSET_X) / 16;
            tile_y = (y + PLAYER_COLLISION_OFFSET_Y) / 16;
    
            if (bitRead(player.tile_map->source[tile_x + (tile_y / 8) * player.tile_map->width], tile_y & 0x7)) {
                y -= y_sign;
            }
            


            if (y_sign > 0) {
                dir = 0;
            } else if (y_sign < 0) {
                dir = 4;
            } else if (x_sign > 0) {
                dir = 2;
            } else if (x_sign < 0) {
                dir = 6;
            }
            

        }
        if (arduboy.everyXFrames(8)) {
            frame = !frame;
        }
        for (uint8_t i = 0; i < NUM_ENEMIES; i++) {
            if (&enemies[i] != this) {
                if (enemies[i].active && !enemies[i].dead) {
                    int8_t x_dist = (enemies[i].x - x) / 8;
                    int8_t y_dist = (enemies[i].y - y) / 8;
                    uint16_t distsq = x_dist * x_dist + y_dist * y_dist;
                
                    if (distsq < 2) {
                        x -= x_dist;
                        y -= y_dist;
                    
                }}
            }
        }


    }
}

void Enemy::draw() {
    if (on_screen(x, y, type == BOSS)){
        if (type == BOSS) {
            if (dead) {
                if (win > 0 && win < 25) {
                    arduboy.drawBitmap(x - player.scrollx + 5, y - player.scrolly + 5, blip_image, 8, 8);
                }
            } else {
                arduboy.drawBitmap(x - player.scrollx, y - player.scrolly, images, 16, 24); 
                if (boss_fireball.timer > 0) {
                    const uint8_t *image;
                    if (boss_fireball.flipped) {
                        image = fireball_flipped_image;
                    } else {
                        image = fireball_image;
                    }
                    arduboy.drawBitmap(boss_fireball.x - player.scrollx, boss_fireball.y - player.scrolly, image, 8, 8);
                }
            }
        } else if (!dead) {
            Sprites::drawSelfMasked(x - player.scrollx, y - player.scrolly,
            images, dir + frame);   
        }     
    }
}
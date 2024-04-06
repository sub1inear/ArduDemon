/* 
    ARDUDEMON
    by: SUBLINEAR - ZED
    FONT3X5 by FILMOTE

*/

#include <Arduboy2.h>
#include <ArduboyTones.h>

#include "player.h"

#include "enemy.h"
#include "dungeon.h"
#include "item.h"

#include "npcs.h"
#include "npcs_text.h"
#include "ui.h"

#include "music.h"

#include "Font3x5.h"
#include "map_data.h"
#include "saveload.h"

/* 
Note:
Uses EEPROM locations 564 to 599, hash protected
*/

/* 
IMPORTANT!: To compile for the original Arduboy, disable the CATHY_3K flag in NPCS_TEXT.CPP
*/


ARDUBOY_NO_USB

// globals
Arduboy2Base arduboy;
Player player;
ArduboyTones sound(arduboy.audio.enabled);
Font3x5 font3x5 = Font3x5(6);

uint8_t num_items = 0;
Item items[MAX_ITEMS];

Enemy enemies[NUM_ENEMIES];
Fireball boss_fireball;

TileMap dungeon_map;
TileMap town_map;
uint8_t dungeon_level = 0;
uint8_t reached_dungeon_level = 0;

uint32_t seed = 0;

bool sub_menu = false;
bool start_game = false;
uint8_t win = 0; // timer, when it is 125 win screen is activated


enum GameState: uint8_t {
    GAME,
    INV,
    DEAD,
    TITLESCREEN,
    MAINMENU,
    WIN,
};
GameState state = TITLESCREEN;

void setup() {
    arduboy.beginDoFirst();
    arduboy.waitNoButtons();

    seed = arduboy.generateRandomSeed();
    #ifdef CATHY_3K
        arduboy.initRandomSeed(); // creates random npc dialogue at the start
    #endif
    town_map.init(town_data, 32, 32, false, 23, 26, 9, 7);
    player.tile_map = &town_map;

    player.init(); // put the player at the correct spawn x & y
    player.weapon.init(0, 0, SWORD, 0, NOSTAT, 0);
    player.shield.init(0, 0, SHIELD, 0, NOSTAT, 0);
}

void loop() {
    if (!arduboy.nextFrame()) {
        return;
    }

    arduboy.pollButtons();

    if (win > 0 && win < 125) {
        win += 1;
    }

    if (player.dead) {
        state = DEAD;
    } else if (state == DEAD) {
        state = GAME;
    } else if (start_game) {
        state = GAME;
        start_game = false; 
    } else if (win == 125) {
        state = WIN;
    } else if (arduboy.justPressed(B_BUTTON)) {
        if (state == INV) {
            if (!sub_menu) {
                state = GAME;
            }
        } else if (state != TITLESCREEN && state != MAINMENU && !player.talking){
            player.hold_timer = 0; // stop hold timer going up forever when exiting out
            state = INV;
        }
    } else if (arduboy.justReleased(A_BUTTON) && state == TITLESCREEN) {
        state = MAINMENU;
    }

    switch (state) {
        case GAME:
            if (player.tile_map->dungeon) {    
                for (uint8_t i = 0; i < NUM_ENEMIES; i++) {
                    enemies[i].update();
                }
            }
            if (player.dead) {
                break;
            }
            player.update();
            if (player.talking) {
                if (player.npc_target == HEALER || player.npc_target == BLACKSMITH) {
                    update_npc_menu();
                    draw_npc_menu();

                }
                if (player.talking != player.old_talking) { 
                    draw_overlay();
                }
                draw_health_xp();
                break; // don't redraw screen if player is talking
            }
            arduboy.clear();

            player.tile_map->draw();

            if (player.tile_map->dungeon) {
                for (uint8_t i = 0; i < NUM_ENEMIES; i++) {
                    enemies[i].draw();
                }
            }
            if (!player.tile_map->dungeon) {
                draw_npcs();
            }


            for (uint8_t i = 0; i < num_items; i++) {
                items[i].draw();
            }

            player.draw();
            draw_health_xp();
            break;

        case INV:
            arduboy.clear();
            update_inv();
            draw_inv();
            break;
        case DEAD:
            arduboy.clear();
            update_dead_menu();
            draw_dead_menu();
            break;
        case MAINMENU:
            arduboy.clear();
            update_main_menu();
            draw_main_menu();
            break;
        case TITLESCREEN:
            arduboy.clear();
            draw_title_screen();
            break;
        case WIN:
            arduboy.clear();
            draw_win_screen();
            break;
    }

    arduboy.display();
}

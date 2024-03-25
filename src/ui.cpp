#include "ui.h"
#include "ui_images.h"

#include "item.h"
#include "item_images.h"
#include "item_text.h"

#include "saveload.h"

#include "player.h"

#include "Font3x5.h"


extern Arduboy2 arduboy;
extern Player player;

extern Item items[MAX_ITEMS];
extern uint8_t num_items;

extern Font3x5 font3x5;

extern bool sub_menu;
extern bool start_game;

static int8_t inv_pointer = 0;
static int8_t sub_pointer = 0;

static int8_t main_menu_pointer = 0;

void draw_win_screen() {
    font3x5.setCursor(WIN_TEXT_X, WIN_TEXT_Y);
    font3x5.print(F("CONGRATLATIONS!\n   YOU WIN!"));
}

void draw_title_screen() {
    arduboy.drawBitmap(TITLE_DEMON_X, TITLE_DEMON_Y, title_screen_demon, 75, 39);
    arduboy.drawBitmap(TITLE_TEXT_X, TITLE_TEXT_Y, title_screen_text, 113, 22);
    arduboy.drawBitmap(TITLE_FLAME_X, TITLE_FLAME_Y, title_screen_flame, 4, 8);
}


void update_main_menu() {
    if (arduboy.justPressed(DOWN_BUTTON)) {
        main_menu_pointer++;
    } else if (arduboy.justPressed(UP_BUTTON)) {
        main_menu_pointer--;
    }
    if (main_menu_pointer < 0) {
        main_menu_pointer = 0;
    }
    if (main_menu_pointer > 2) {
        main_menu_pointer = 2;
    }
    if (arduboy.justPressed(A_BUTTON)) {
        switch (main_menu_pointer) {
            case 0:
                start_game = load_game();
                break;
            case 1:
                clear_save();
                start_game = true;
                break;
            case 2:
                arduboy.audio.toggle();
                arduboy.audio.saveOnOff();
                break;
        }
    }
}
void draw_main_menu() {
    arduboy.drawBitmap(MAIN_TEXT_X, MAIN_TEXT_Y, title_screen_text, 113, 22);
    arduboy.drawBitmap(MAIN_FLAME_X, MAIN_FLAME_Y, title_screen_flame, 4, 8);
    font3x5.setCursor(MAIN_CREDIT_X, MAIN_CREDIT_Y);
    font3x5.print(F("BY SUBLINEAR"));
    font3x5.setCursor(MENU_START_X, MENU_START_Y);
    font3x5.println(F("LOAD GAME"));
    font3x5.println(F("NEW GAME"));
    font3x5.println(F("SFX"));
    if (arduboy.audio.enabled()) {
        arduboy.fillRect(MENU_START_X + 14, MENU_START_Y + 13, 5, 5);
    } else {
        arduboy.drawRect(MENU_START_X + 14, MENU_START_Y + 13, 5, 5);
    }

    arduboy.drawBitmap(MENU_START_X + 38, MENU_START_Y + 6 * main_menu_pointer, menu_pointer_image, 4, 8);

    
}


void update_dead_menu() {
    if (arduboy.justReleased(A_BUTTON)) {
        player.dead = false; // go back to game loop
    }
}
void draw_dead_menu() {
    font3x5.setCursor(GAME_OVER_X, GAME_OVER_Y);
    arduboy.drawBitmap(GAME_OVER_X - 9, GAME_OVER_Y - 1, skull_image, 8, 8);
    
    font3x5.println(F("YOU DIED \nGAME OVER"));

    font3x5.setCursor(GAME_OVER_X,  GAME_OVER_Y + 20);
    
    font3x5.println(F("CONTINUE"));

    arduboy.drawBitmap(GAME_OVER_X + 38, GAME_OVER_Y + 20, menu_pointer_image, 4, 8);

}


void draw_health_xp() {
    arduboy.drawBitmap(WIDTH - 9, HEIGHT - 8 - 1, heart_image, 8, 8);
    uint8_t health_height = ((uint16_t)player.hp * (uint16_t)BAR_HEIGHT) / player.max_hp;
    arduboy.drawRect(WIDTH - 7, HEIGHT - 8 - 1 - BAR_HEIGHT, 4, BAR_HEIGHT);
    arduboy.fillRect(WIDTH - 6, HEIGHT - 8 - 1 - health_height, 2, health_height);

    arduboy.drawRect(9, HEIGHT - 4, WIDTH - 20, 3);
    arduboy.drawFastHLine(10, HEIGHT - 3, WIDTH - 22, BLACK);
    arduboy.drawFastHLine(10, HEIGHT - 3, (player.xp * (WIDTH - 22)) / 255);
    
    font3x5.setCursor(1, HEIGHT - 6);
    font3x5.print("XP");
}




void update_inv() {
    

    int8_t x_axis = (arduboy.justPressed(RIGHT_BUTTON) - arduboy.justPressed(LEFT_BUTTON));
    int8_t y_axis = (arduboy.justPressed(DOWN_BUTTON) - arduboy.justPressed(UP_BUTTON));
    
    

    if (sub_menu) {
        int8_t past_sub_pointer = sub_pointer;

        sub_pointer += x_axis;
        sub_pointer += y_axis;

        if (sub_pointer < 0 || sub_pointer > 1) {
            sub_pointer = past_sub_pointer;
        }
        
    } else {
        
        int8_t past_inv_pointer = inv_pointer;

        bool wrap_right = (past_inv_pointer == (MAX_INV / 2) - 1 && x_axis > 0);
        bool wrap_left = (past_inv_pointer == (MAX_INV / 2) && x_axis < 0);

        if (!(wrap_right || wrap_left)) { // prevent wrapping around
            inv_pointer += x_axis;
        }
        inv_pointer += y_axis * (MAX_INV / 2);

        if (inv_pointer < 0 || inv_pointer > MAX_INV - 1) {
            inv_pointer = past_inv_pointer;
        }
    }

    if (arduboy.justReleased(A_BUTTON)) {
        
        if (sub_menu) {
            if (sub_pointer == CHECK) {
                uint8_t item_type = player.inventory[inv_pointer].type;
                switch (item_type) {
                    case SWORD:
                    case AXE: {
                        Item former_weapon = player.weapon;

                        if (former_weapon.magic_stat != NOSTAT) {

                            uint16_t old_max_hp = player.stats[CON] * 2; // old max health
                            player.stats[former_weapon.magic_stat] -= former_weapon.magic_bonus;

                            if (former_weapon.magic_stat == CON) {
                                player.max_hp = player.stats[CON] * 2; // new max health
                                player.hp = (player.hp * player.max_hp) / old_max_hp;
                            }
                        }
                        player.weapon = player.inventory[inv_pointer];
                        
                        if (player.weapon.magic_stat != NOSTAT) {

                            uint16_t old_max_hp = player.stats[CON] * 2; // old max health
                            player.stats[player.weapon.magic_stat] += player.weapon.magic_bonus;

                            if (player.weapon.magic_stat == CON) {
                                player.max_hp = player.stats[CON] * 2; // new max health
                                player.hp = (player.hp * player.max_hp) / old_max_hp;
                            }
                        }
                        player.inventory[inv_pointer] = former_weapon;
                        break;
                    }
                    case SHIELD: { 
                        Item former_shield = player.shield;

                        if (former_shield.magic_stat != NOSTAT) {

                            uint16_t old_max_hp = player.stats[CON] * 2; // old max health
                            player.stats[former_shield.magic_stat] -= former_shield.magic_bonus;

                            if (former_shield.magic_stat == CON) {
                                player.max_hp = player.stats[CON] * 2; // new max health
                                player.hp = (player.hp * player.max_hp) / old_max_hp;
                            }
                        }
                        player.shield = player.inventory[inv_pointer];
                        if (player.shield.magic_stat != NOSTAT) {

                            uint16_t old_max_hp = player.stats[CON] * 2; // old max health
                            player.stats[player.shield.magic_stat] += player.shield.magic_bonus;

                            if (player.shield.magic_stat == CON) {
                                player.max_hp = player.stats[CON] * 2; // new max health
                                player.hp = (player.hp * player.max_hp) / old_max_hp;

                            }
                        }
                        player.inventory[inv_pointer] = former_shield;
                        break;
                    }
                    case POTION:  {
                        uint8_t item_pwramt = player.inventory[inv_pointer].pwramt;
                        
                        uint8_t healing = HEALTH_POTION_HP * (item_pwramt + 1);
                        if (player.hp + healing > player.max_hp) { 
                            player.hp = player.max_hp;
                        } else {
                            player.hp += healing;
                        }

                        player.num_inv -= 1;
                        for (uint8_t i = inv_pointer; i < player.num_inv; i++) {
                            player.inventory[i] = player.inventory[i + 1];
                        }
                        break;
                    }
                         
                }


            } else if (sub_pointer == CROSS) { // commence dropping!
                Item to_drop = player.inventory[inv_pointer];

                uint8_t drop_offset = to_drop.size / 2;

                to_drop.x = player.x /*+ PLAYER_OFFSET_X*/ + PLAYER_COLLISION_OFFSET_X - drop_offset;
                to_drop.y = player.y /*+ PLAYER_OFFSET_Y*/ + PLAYER_COLLISION_OFFSET_Y - drop_offset;



                player.num_inv -= 1;
                for (uint8_t i = inv_pointer; i < player.num_inv; i++) {
                    player.inventory[i] = player.inventory[i + 1];
                }

                num_items += 1;
                if (num_items > MAX_ITEMS) {
                    for (uint8_t i = 0; i < MAX_ITEMS - 1; i++) {
                        items[i] = items[i + 1];
                    }
                    items[MAX_ITEMS - 1] = to_drop; // copy new item to last slot
                    num_items = MAX_ITEMS;
                } else {
                    items[num_items - 1] = to_drop;
                }
            }
            sub_menu = false; // exit no matter what
            sub_pointer = CHECK;

        } else if (inv_pointer < player.num_inv) {        
            sub_menu = true; 
        } 
    }
    if (arduboy.justReleased(B_BUTTON)) { // previous scripts in main prevent b from exiting
        if (sub_menu) {
            sub_menu = false;
            sub_pointer = CHECK;
        }
    }
}

void draw_inv() {
    for (uint8_t i = 0; i < MAX_INV; i++) { // main inv slots

        uint8_t item_x = ((i % (MAX_INV / 2)) * SLOTS_OFFSET);
        uint8_t item_y =  (i / (MAX_INV / 2)) * SLOTS_OFFSET;

        arduboy.drawRect(item_x - 1 + SLOTS_X, item_y - 1 + SLOTS_Y, 16 + 2, 16 + 2);
        if (i < player.num_inv) {
            uint8_t item_offset =  (player.inventory[i].size == 8) ? 4 : 0;
            arduboy.drawBitmap(item_x + item_offset + SLOTS_X, item_y + item_offset + SLOTS_Y, pgm_read_ptr(&loot_lut[player.inventory[i].type]), player.inventory[i].size, player.inventory[i].size);
        }
        if (inv_pointer == i) {
            arduboy.drawBitmap(item_x + 4 + SLOTS_X, item_y + 8 + SLOTS_Y, inv_pointer_image, 8, 8);
        }
    }

    // sword and shield
    arduboy.drawRect(EQUIPPED_X, EQUIPPED_Y, 16 + 2, 16 + 2);
    
    arduboy.drawBitmap(EQUIPPED_X + 5, EQUIPPED_Y + 5, pgm_read_ptr(&loot_lut[player.weapon.type]), 8, 8);
    
    arduboy.drawRect(EQUIPPED_X, EQUIPPED_Y + EQUIPPED_OFFSET, 16 + 2, 16 + 2);
    arduboy.drawBitmap(EQUIPPED_X + 5, EQUIPPED_Y + EQUIPPED_OFFSET + 5, pgm_read_ptr(&loot_lut[player.shield.type]), 8, 8);

    // text box
    arduboy.drawRect(TEXT_BOX_X, TEXT_BOX_Y, TEXT_BOX_W, TEXT_BOX_H);
    font3x5.setCursor(TEXT_BOX_X + 2, TEXT_BOX_Y + 1);
    if (sub_menu) {
        arduboy.drawBitmap(TEXT_BOX_X + 2, TEXT_BOX_Y + 1, check_image, 8, 8);
        arduboy.drawBitmap(TEXT_BOX_X + 11, TEXT_BOX_Y + 1, cross_image, 8, 8);

        arduboy.drawBitmap(TEXT_BOX_X + 2 + 9 * sub_pointer, TEXT_BOX_Y + 11, inv_pointer_image, 8, 8);

    } else if (inv_pointer < player.num_inv) {
        uint8_t item_type = player.inventory[inv_pointer].type;
        uint8_t item_pwramt = player.inventory[inv_pointer].pwramt;
        uint8_t item_magic_stat = player.inventory[inv_pointer].magic_stat;
        uint8_t item_magic_bonus = player.inventory[inv_pointer].magic_bonus;

        if (item_type == GOLD) {
            char buffer[4];

            // if (item_pwramt == 1) {
            //     sprintf(buffer, "%d PIECE OF ", item_pwramt);
            // } else {
            //     sprintf(buffer, "%d PIECES OF ", item_pwramt);
            // }

            // font3x5.print(buffer);
            utoa(item_pwramt, buffer, 10);
            font3x5.print(buffer);
            font3x5.print(F(" PIECES OF "));
        } else {
            if (item_magic_stat != NOSTAT) {
                font3x5.print(F("MAGIC "));
            }
            font3x5.print((const char *) pgm_read_ptr(&item_prefix[(item_type - 1) * 4 + item_pwramt]));
        }
        font3x5.println((const char *) pgm_read_ptr(&item_names[item_type]));
        
        if (item_magic_stat != NOSTAT) {
            char buffer[4];

            //sprintf(buffer, "\n%s +%d", (char *)pgm_read_ptr(&stat_names[item_magic_stat]), item_magic_bonus);

            font3x5.print((char *)pgm_read_ptr(&stat_names[item_magic_stat]));
            font3x5.print(F(" +"));
            
            utoa(item_magic_bonus, buffer, 10);
            font3x5.print(buffer);
        }
        if (item_pwramt == 255 && item_type == GOLD) {
            font3x5.print(F("THIS IS AS MUCH GOLD\nAS YOU CAN CARRY."));
        }

        
        
        
    }
    
    // stats box
    arduboy.drawRect(STATS_BOX_X, STATS_BOX_Y, STATS_BOX_W, STATS_BOX_H); // stats box
    font3x5.setCursor(STATS_BOX_X + 2, STATS_BOX_Y + 1);

    char buffer[4];

    // sprintf(buffer, "STR %d\nCON %d\nDEX %d\nMNA %d\nLVL %d", player.stats[0], player.stats[1], player.stats[2], player.stats[3], player.level);
    font3x5.print(F("STR "));
    font3x5.println(player.stats[0]);

    font3x5.print(F("CON "));
    font3x5.println(player.stats[1]);

    font3x5.print(F("DEX "));
    font3x5.println(player.stats[2]);

    font3x5.print(F("MNA "));
    font3x5.println(player.stats[3]);

    font3x5.print(F("LVL "));
    font3x5.println(player.level);      
}
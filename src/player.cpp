#include "player.h"
#include "player_images.h"

#include "dungeon.h"

#include "item.h"
#include "item_images.h"

#include "npcs.h"

#include "music.h"

#include "Font3x5.h"

#include "saveload.h"

extern Arduboy2 arduboy;
extern ArduboyTones sound;

extern TileMap dungeon_map;
extern TileMap town_map;

extern uint8_t dungeon_level;
extern uint8_t reached_dungeon_level;
extern Enemy enemies[NUM_ENEMIES];

extern Item items[MAX_ITEMS];
extern uint8_t num_items;

extern Font3x5 font3x5;

extern bool win;

static const uint8_t walk_lut[4] PROGMEM = {0, 1, 2, 1};
static const int8_t attack_lut[8] PROGMEM = {
    7, 17, // DOWN
    7, 2,  // UP
    16, 8,
    -2, 9
};
static const uint8_t player_damage_lut[] PROGMEM = {
    1,
    8,
    16,
    25    
};
// player_defense_lut is located in enemy.cpp

using uint24_t = __uint24;


void Player::init() {

    x = tile_map->start_tile_x * 16 /*- PLAYER_OFFSET_X*/;
    y = tile_map->start_tile_y * 16 /*- PLAYER_OFFSET_Y*/;
}

bool Player::is_collidable(uint8_t tile_x, uint8_t tile_y) {

    if (tile_map->dungeon) {
        return bitRead(tile_map->source[tile_x + (tile_y / 8) * tile_map->width], tile_y & 0x7);
    } else {
        uint8_t tile = pgm_read_byte(tile_map->source + tile_x + tile_map->width * tile_y);
        if (tile != 0 && tile != 26 && tile != 28) {
            return true;
        } else {
            return (tile_x == ELDER_X && tile_y == ELDER_Y) ||
                   (tile_x == BLACKSMITH_X && tile_y == BLACKSMITH_Y) ||
                   (tile_x == HEALER_X && tile_y == HEALER_Y) ||
                   (tile_x == INNKEEPER_X && tile_y == INNKEEPER_Y) ||
                   (tile_x == MAGE_X && tile_y == MAGE_Y);

        }
    }
}

void Player::drop_loot(uint8_t index) {

    uint8_t item_type = random(0, NUM_ITEM_TYPES);
    
    uint8_t item_pwramt = 0;

    uint8_t item_magic_stat = NOSTAT;
    uint8_t item_magic_bonus = 0;

    if (item_type == GOLD) {
        item_pwramt = random(1, 6 + dungeon_level * 2);
    } else {
        if (item_type != POTION) {
            if (random(0, 3) != 0) {
                item_magic_stat = random(0, 3); // between STR, CON, and DEX
            }
        }
        if (dungeon_level == 0) {
            item_pwramt = 0;
            item_magic_bonus = random(1, 3) * 5; // 5 to 10 
        } else if (dungeon_level < 3) { // dungeon_levels 1 and 2
            item_pwramt = 1;
            item_magic_bonus = random(3, 6) * 5; // 15 to 25
        } else if (dungeon_level < 5) { // dungeon_levels 3 and 4
            item_pwramt = 2;
            item_magic_bonus = random(6, 9) * 5; // 30 to 40
        } else { // dungeon_level 5 and 6
            item_pwramt = 3;
            item_magic_bonus = random(9, 12) * 5; // 45 to 55
        }
    }

    uint8_t image_offset = ((item_type == GOLD) ? 0 : 4);

    items[index].init(target->x + image_offset, target->y + image_offset, item_type, item_pwramt, item_magic_stat, item_magic_bonus);
}

void Player::die() { // sets player.dead and also does work for when we respawn
    
    dead = true;
    num_items = 0;
    
    tile_map = &town_map;
    
    x = tile_map->start_tile_x * 16 /*- PLAYER_OFFSET_X*/;
    y = tile_map->start_tile_y * 16 /*- PLAYER_OFFSET_Y*/; 

    hp = max_hp;
    if (attacking) {
        attacking = false;
        dir -= 12;
        frame = 0;
    }
}

uint8_t Player::check_npcs(uint8_t tile_x, uint8_t tile_y) {
    
    int8_t x_dist = ELDER_X - tile_x;
    int8_t y_dist = ELDER_Y - tile_y;
    uint16_t distsq = x_dist * x_dist + y_dist * y_dist;

    if (distsq <= 2) {
        return ELDER;
    }
    x_dist = BLACKSMITH_X - tile_x;
    y_dist = BLACKSMITH_Y - tile_y;
    distsq = x_dist * x_dist + y_dist * y_dist;

    if (distsq <= 2) {
        return BLACKSMITH;
    }
    x_dist = HEALER_X - tile_x;
    y_dist = HEALER_Y - tile_y;
    distsq = x_dist * x_dist + y_dist * y_dist;

    if (distsq <= 2) {
        return HEALER;
    }
    x_dist = INNKEEPER_X - tile_x;
    y_dist = INNKEEPER_Y - tile_y;
    distsq = x_dist * x_dist + y_dist * y_dist;

    if (distsq <= 2) {
        return INNKEEPER;
    }
    x_dist = MAGE_X - tile_x;
    y_dist = MAGE_Y - tile_y;
    distsq = x_dist * x_dist + y_dist * y_dist;

    if (distsq <= 2) {
        return MAGE;
    }
    x_dist = SECRET_X - tile_x;
    y_dist = SECRET_Y - tile_y;
    distsq = x_dist * x_dist + y_dist * y_dist;

    if (distsq <= 2) {
        return SECRET;
    }

    return NONPC;
}
void Player::teleport() {
    if (tile_map->dungeon) {
        if (on_stairs_down) { // doesn't register if on last level
            dungeon_level += 1;
            if (dungeon_level != 6) { // no boss
                reached_dungeon_level = dungeon_level;
            }
            gen_dungeon();
            init();
        } else if (on_stairs_up) {
            tile_map = &town_map;
            x = tile_map->exit_tile_x * 16 /*- PLAYER_OFFSET_X*/;
            y = tile_map->exit_tile_y * 16 /*- PLAYER_OFFSET_Y*/;
        }
    } else {
        tile_map = &dungeon_map;
        dungeon_level = reached_dungeon_level;
        gen_dungeon();
        init();
    }
    num_items = 0; // clear items
    save_game();
}

void Player::update() {

    // read buttons
    if (arduboy.justPressed(A_BUTTON)) {
        hold_timer = 1;
    }
    if (hold_timer > 0) {
        if (hold_timer < 255) {
            hold_timer += 1;
        } else {
            wizard_hat = true;
        }
    } 
    
    old_talking = talking; // used to register change in talking
    if (arduboy.justReleased(A_BUTTON)) {
        if (on_stairs_down || on_stairs_up) { // on_stairs is prevented from being true if last level
            teleport();
        } else if (hold_timer > 40) { // item pickup
            for (uint8_t i = 0; i < num_items; i++) {
                uint8_t image_offset = items[i].size / 2; // Gold is 16x16, rest are 8x8

                uint16_t x_dist = abs((items[i].x + image_offset) - (x + 7));
                uint16_t y_dist = abs((items[i].y + image_offset) - (y + 7));
                uint24_t distsq = (uint24_t)x_dist * (uint24_t)x_dist + (uint24_t)y_dist * (uint24_t)y_dist;

                if (distsq < (image_offset + 3) * (image_offset + 3)) {
                    
                    // combining gold code
                    if (items[i].type == GOLD) { // add pieces to current existing gold in inv if there is one
                        bool break_flag = false;
                        for (uint8_t h = 0; h < num_inv; h++) {

                            if (inventory[h].type == GOLD) {
                                if (inventory[h].pwramt + items[i].pwramt > 255) {
                                    break_flag = true;
                                    break;
                                }
                                inventory[h].pwramt += items[i].pwramt;
                                // delete item from items
                                for (uint8_t g = i; g < num_items; g++) {
                                    items[g] = items[g + 1];
                                }
                                num_items -= 1;
                                break_flag = true;

                                break;                                
                            }
                        }
                        if (break_flag) {
                            break;
                        }
                    }
                    // adding item code (can be gold if no gold already in inventory)
                    if (num_inv < MAX_INV || items[i].type == GOLD) { 
                        inventory[num_inv] = items[i];

                        for (uint8_t h = i; h < num_items; h++) {
                            items[h] = items[h + 1];
                        }
                        num_inv += 1;
                        num_items -= 1;
                        break;
                    }
                } 
            }
            
        } else if (tile_map->dungeon && !attacking) {    
            attacking = true;
            frame = 0;
            frame_speed = (weapon.type == AXE) ? 9 : 6;
                    
            dir += 12; // convert walking directions into attack directions
        } else if (!tile_map->dungeon && npc_target !=  NONPC) {
            talking = !talking;
        }
        hold_timer = 0;
    }
    if (talking) {
        return;
    }

    if (attacking) {
        frame++;
        
        if (frame == 3 * frame_speed) { // stop attacking
            attacking = false;
            dir -= 12;
            frame = 0;
            
            for (uint8_t i = 0; i < NUM_ENEMIES; i++) { // do damage
                if (enemies[i].active && !enemies[i].dead) {
                    int16_t x_dist = (enemies[i].x + 8) - (x + (int8_t) pgm_read_byte(attack_lut + dir / 3 * 2 + 0));
                    int16_t y_dist = (enemies[i].y + 8) - (y + (int8_t) pgm_read_byte(attack_lut + dir / 3 * 2 + 1));
                    uint24_t distsq = (uint24_t)x_dist * (uint24_t)x_dist + (uint24_t)y_dist * (uint24_t)y_dist;

                    if (distsq < 11 * 11) {
                        target = &enemies[i];
                        
                        uint8_t damage = pgm_read_byte(player_damage_lut + weapon.pwramt) + stats[0] / 5;
                        if (weapon.type == AXE) {
                            damage += damage / 2;
                        }
                        if (target->hp <= damage) {
                            target->dead = true;
                            
                            if (target->type == BOSS) {
                                win = 1;
                                sound.tone(NOTE_C5, 50, NOTE_G5, 100);
                                break;
                            } else {
                                sound.tone(NOTE_C0, 50, NOTE_A0, 100);
                            }

                            

                            num_items += 1;
                            if (num_items > MAX_ITEMS) {
                                
                                for (uint8_t i = 0; i < MAX_ITEMS - 1; i++) {
                                    items[i] = items[i + 1];
                                }
                                drop_loot(MAX_ITEMS - 1); // copy item to last slot.
                                num_items = MAX_ITEMS;
                            } else {
                                drop_loot(num_items - 1);
                            }
                            
                            uint8_t xp_gained = 20;
                            
                            if (255 - xp <= xp_gained) {
                                // level up!
                                if (level < 20) { // 100 (max gain from levels) + 10 (starting stat) + 140 (max gain from weapon + shield) = 250 < 255
                                    level += 1;
                                    stats[0] += 5;
                                    stats[1] += 5;
                                    stats[2] += 5;
                                    stats[3] += 5;
                                    xp = 0;


                                    max_hp = stats[1] * 2;

                                    level_up_timer = 200;
                                }
                            } else {
                                xp += xp_gained;
                            }

                        } else {

                            target->hp -= damage;
                        }
                    }
                }
            }
        } else {
            return;
        }
    }

    y_axis = (arduboy.pressed(DOWN_BUTTON) - arduboy.pressed(UP_BUTTON));

    // stop diagonal movement (cannot account for speed boost with fractions)
    if (y_axis == 0) {
        x_axis = (arduboy.pressed(RIGHT_BUTTON) - arduboy.pressed(LEFT_BUTTON));
    } else { 
        x_axis = 0;
    }

    // collide on x axis
    x += x_axis;

    uint8_t tile_x = (x + PLAYER_COLLISION_OFFSET_X) / 16;
    uint8_t tile_y = (y + PLAYER_COLLISION_OFFSET_Y) / 16;
    
    if (is_collidable(tile_x, tile_y)) {
        x -= x_axis;
    }

    if (tile_map->dungeon) { // collide with enemies
        for (uint8_t i = 0; i < NUM_ENEMIES; i++) { 
            if (enemies[i].active && !enemies[i].dead) {
                uint16_t x_dist = abs((enemies[i].x + 8) - (x + PLAYER_COLLISION_OFFSET_X));
                uint16_t y_dist = abs((enemies[i].y + 8) - (y + PLAYER_COLLISION_OFFSET_Y));
                uint24_t distsq = x_dist * x_dist + y_dist * y_dist;
                uint8_t boss_offset = (enemies[i].type == BOSS) ? 8 : 0;

                if (distsq < (7 + boss_offset) * (7 + boss_offset)) {
                    x -= x_axis;
                }
            }
        }
    }

    // collide on y axis
    y += y_axis;

    tile_x = (x + PLAYER_COLLISION_OFFSET_X) / 16;
    tile_y = (y + PLAYER_COLLISION_OFFSET_Y) / 16;
    
    if (is_collidable(tile_x, tile_y)) {
        y -= y_axis;
    }

    if (tile_map->dungeon) { // collide with enemies
        for (uint8_t i = 0; i < NUM_ENEMIES; i++) { 
            if (enemies[i].active && !enemies[i].dead) {
                uint16_t x_dist = abs((enemies[i].x + 8) - (x + PLAYER_COLLISION_OFFSET_X));
                uint16_t y_dist = abs((enemies[i].y + 8) - (y + PLAYER_COLLISION_OFFSET_Y));
                uint24_t distsq = x_dist * x_dist + y_dist * y_dist;
                uint8_t boss_offset = (enemies[i].type == BOSS) ? 8 : 0;

                if (distsq < (7 + boss_offset) * (7 + boss_offset)) {
                    y -= y_axis;
                }
            }
        }
    }

    on_stairs_down = false;
    on_stairs_up = false;

    tile_x = (x + PLAYER_COLLISION_OFFSET_X) / 16;
    tile_y = (y + PLAYER_COLLISION_OFFSET_Y) / 16;
    if (tile_map->dungeon) {
        if (tile_x == tile_map->start_tile_x && tile_y == tile_map->start_tile_y) {
            on_stairs_up = true;
        }
    }
    if (dungeon_level < NUM_LEVELS - 1 || !tile_map->dungeon) { // make sure its not last level (not applicable in town)
        if (tile_x == tile_map->exit_tile_x && tile_y == tile_map->exit_tile_y) {
            on_stairs_down = true;
        }
    }
    if (!tile_map->dungeon) {
        npc_target = check_npcs(tile_x, tile_y);
    }
    // set frame and direction
    if (x_axis != 0 || y_axis != 0) {
        frame_speed = 8;
        if (y_axis > 0) dir = 0;
        if (y_axis < 0) dir = 3;
        if (x_axis > 0) dir = 6;
        if (x_axis < 0) dir = 9;

        frame++;
        frame &= 0x1F; // mod 32

    } else {
        frame = 1 * frame_speed;
    }

    if (x < -3) { // player image starts 3 pixels in
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    if (y > 32 * 15) {
        y = 32 * 15;
    }
    if (x > 32 * 15 + 4) {
        x = 32 * 15 + 4;
    }
    


    set_camera();
}

void Player::set_camera() {
    // set camera
    scrollx = x - PLAYER_OFFSET_X;
    scrolly = y - PLAYER_OFFSET_Y;

    // fence camera
    if (scrollx < 0) scrollx = 0;
    if (scrolly < 0) scrolly = 0;
    if (scrollx > (tile_map->width - 1) * 16 - WIDTH) scrollx = (tile_map->width - 1) * 16 - WIDTH;
    if (scrolly > (tile_map->height - 1) * 16 - HEIGHT) scrolly = (tile_map->height - 1) * 16 - HEIGHT;
}

void Player::draw() {

    uint8_t div_frame = frame / frame_speed;

    if (attacked_timer) {
        attacked_timer -= 1;
    }

    Sprites::drawPlusMask(x - scrollx /*+ PLAYER_OFFSET_X*/, y - scrolly /*+ PLAYER_OFFSET_Y*/,
                          player_images, dir + pgm_read_byte(walk_lut + div_frame));
    if (wizard_hat) {
        const uint8_t* hat = wizard_hat_image;
        uint8_t offset = 0;
        if (dir == 9 || dir == 21) {
            hat = wizard_hat_flipped_image;
            offset = 5;
        } else {
            hat = wizard_hat_image;
            offset = 3;
        }
        arduboy.drawBitmap(x - scrollx + offset, y - scrolly - 7, hat, 8, 8);
    }
    if (attacking) {

        const uint8_t *weapon_images = (const uint8_t PROGMEM *)nullptr;
        uint8_t weapon_offset_idx_offset = 0;
        switch (weapon.type) {
            case SWORD:
                weapon_images = sword_anim_images;
                weapon_offset_idx_offset = 0;
                break;
            case AXE:
                weapon_images = axe_anim_images;
                weapon_offset_idx_offset = 24;
                break;
        }
        if (weapon_images != nullptr) {
            int8_t weapon_offset_x = (int8_t)pgm_read_byte(weapon_offsets + (dir - 12) * 2 + div_frame * 2 + weapon_offset_idx_offset + 0);
            int8_t weapon_offset_y = (int8_t)pgm_read_byte(weapon_offsets + (dir - 12) * 2 + div_frame * 2 + weapon_offset_idx_offset + 1);

            if (weapon_offset_x != DONOTDRAW) { 
                uint8_t idx = dir - 12 + div_frame;
                idx -= (idx > 6) ? 2 : 0; // <- handle empty frames

                Sprites::drawSelfMasked(
                    x + weapon_offset_x - scrollx /*+ PLAYER_OFFSET_X*/,
                    y + weapon_offset_y - scrolly /*+ PLAYER_OFFSET_Y*/,
                    weapon_images, idx
                );
            }
        }
    }

    // A Button Hovering over item pickup
    for (uint8_t i = 0; i < num_items; i++) {
        uint8_t image_offset = (items[i].type == GOLD) ? 8 : 4; // type 0 is 16x16, rest are 8x8

        uint16_t x_dist = abs((items[i].x + image_offset) - (x + 7));
        uint16_t y_dist = abs((items[i].y + image_offset) - (y + 7));
        uint24_t distsq = (uint24_t)x_dist * (uint24_t)x_dist + (uint24_t)y_dist * (uint24_t)y_dist;
        
        if (distsq < (image_offset + 3) * (image_offset + 3)) {
            
            bool break_flag = false;
            if (items[i].type == GOLD) {
                for (uint8_t h = 0; h < num_inv; h++) {
                    if (inventory[h].type == GOLD) {
                        if (inventory[h].pwramt + items[i].pwramt > 255) {
                            break_flag = true;
                            break;
                        } else {
                            break;
                        }
                    }
                }
                if (break_flag) {
                    break;
                }
            }
            if (num_inv < MAX_INV || items[i].type == GOLD) {
                
                font3x5.setCursor(x - scrollx + /*PLAYER_OFFSET_X*/ - 1, y - scrolly + /*PLAYER_OFFSET_Y*/ - 18);
                font3x5.print("HOLD");
                arduboy.drawBitmap(x - scrollx + /*PLAYER_OFFSET_X*/ + 3, y - scrolly + /*PLAYER_OFFSET_Y*/ - 10, abutton_image, 8, 8);

                if (hold_timer > 0) { // progress bar
                    arduboy.drawFastHLine(x - scrollx + /*PLAYER_OFFSET_X*/ + 2, y - scrolly + /*PLAYER_OFFSET_Y*/ - 2, min(hold_timer, 40) / 4);
                }
                break;
            }
        }
    }
    

    if (level_up_timer > 0) { // level up text
        font3x5.setCursor(0, 50);
        font3x5.print("LEVEL UP!");
        level_up_timer -= 1;
    }
}

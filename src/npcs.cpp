#include "npcs.h"

#include "npcs_images.h"
#include "npcs_text.h"
#include "ui_images.h"
#include "player.h"
#include "tools.h"

#include "Font3x5.h"
#include "FlashStringHelper.h"

extern Arduboy2 arduboy;
extern Player player;
extern Font3x5 font3x5;

static int8_t npc_menu_pointer = 0;
static int8_t y_axis = 0;

static uint8_t npc_text_selection = 0;

static void purchase(uint8_t num_gold, uint8_t item_type, uint8_t item_pwramt) {
    font3x5.setCursor(NPC_TEXT_X + 1, NPC_TEXT_Y + 64 - NPC_TEXT_Y * 2 - 8);
    if (player.num_inv + 1 <= MAX_INV) {
        for (uint8_t i = 0; i < player.num_inv; i++) {
            if (player.inventory[i].type == GOLD) {
                if (player.inventory[i].pwramt >= num_gold) {
                    player.inventory[i].pwramt -= num_gold;
                    if (player.inventory[i].pwramt == 0) {
                        player.num_inv -= 1;
                        for (uint8_t h = i; h < player.num_inv; h++) {
                            player.inventory[h] = player.inventory[h + 1];
                        }
                    }
                    
                    uint8_t magic_stat = NOSTAT;
                    uint8_t magic_bonus = 0;

                    if (item_type != POTION) {
                        magic_stat = random(0, 4);
                        magic_bonus = random(12, 15) * 5; // 60 to 70    
                    }
                    player.inventory[player.num_inv].init(0, 0, item_type, item_pwramt, magic_stat, magic_bonus); 
                    player.num_inv += 1;
                    sound.tone(NOTE_C4, 50, NOTE_A4, 100);
                    font3x5.print(F("BOUGHT!"));
                    return;

                } else {
                    break;
                }
            }
        }
    }
    // reached only on unsucessful buys
    font3x5.print(F("CANNOT BUY!"));
}

void update_npc_menu() {
    y_axis = (arduboy.justPressed(DOWN_BUTTON) - arduboy.justPressed(UP_BUTTON));

    npc_menu_pointer += y_axis;

    if (npc_menu_pointer < 0) {
        npc_menu_pointer = 0;
    }
    if (npc_menu_pointer > 2) {
        npc_menu_pointer = 2;
    }
    if (arduboy.justReleased(B_BUTTON)) { // buy item
        if (player.npc_target == HEALER) {
            switch (npc_menu_pointer) {
                case 0: // BUY A HEALTH POTION 5G
                    purchase(5, POTION, 0);
                    break;
                case 1: // BUY A LARGE HLTH POT. 10G
                    purchase(10, POTION, 1);
                    break;
                case 2: // RECIEVE HEALING
                    player.hp = player.max_hp; 
                    break;
            }
        } else if (player.npc_target == BLACKSMITH) {
            switch (npc_menu_pointer) {
                case 0: // BUY AN AXE 75G
                    purchase(75, AXE, 3);
                    break;
                case 1: // BUY A SWORD 75G
                    purchase(75, SWORD, 3);
                    break;
                case 2: // BUY A SHIELD 50G
                    purchase(50, SHIELD, 3);
                    break;
            }
        }
    }

}

void draw_npc_menu() {
    
    arduboy.fillRect(112, NPC_TEXT_Y + 13, 4, 19, BLACK);
    arduboy.drawBitmap(112, NPC_TEXT_Y + 13 + 6 * npc_menu_pointer, menu_pointer_image, 4, 8);
    if (y_axis != 0) {
        arduboy.fillRect(NPC_TEXT_X + 1, NPC_TEXT_Y + 64 - NPC_TEXT_Y * 2 - 7, 12 * 4 - 6, 5, BLACK);
    }
}

void draw_overlay() {
    arduboy.fillRect(NPC_TEXT_X, NPC_TEXT_Y, 128 - NPC_TEXT_X * 2, 64 - NPC_TEXT_Y * 2, BLACK);

    arduboy.drawRect(NPC_TEXT_X, NPC_TEXT_Y, 128 - NPC_TEXT_X * 2, 64 - NPC_TEXT_Y * 2, WHITE);
    
    font3x5.setCursor(NPC_TEXT_X + 2, NPC_TEXT_Y + 1);

    #ifdef CATHY_3K
        npc_text_selection = random(7);
    #else
        npc_text_selection += 1;
        if (npc_text_selection > 1) {
            npc_text_selection = 0;
        }
    #endif

    switch (player.npc_target) {
        case ELDER:
            font3x5.print(F("HELLO MY FRIEND\nSTAY A WHILE AND LISTEN!\n\n"));
            font3x5.print(readFlashStringPointer(&npcs_text[npc_text_selection]));
            break;
        case BLACKSMITH:
            font3x5.print(F(
	            "WELL WHAT CAN I DO FOR YA?\n\n"
	            "BUY AN GREATAXE 75G\n"
	            "BUY A GREATSWORD 75G\n"
	            "BUY A ROYAL SHIELD 50G\n"
            ));
            break;
        case HEALER:
            font3x5.print(F(
                "WHAT AILS YOU MY FRIEND?\n\n"
                "BUY A HEALTH POTION 5G\n"
                "BUY A LARGE HLTH POT. 10G\n"
                "RECIEVE HEALING"));
            break;
        case INNKEEPER:
            font3x5.print(F("THANK GOODNESS YOUVE\nRETURNED MY FRIEND!\nDARK RIDERS RAVAGED\nTHE TOWN AND TOOK\nMANY TO THE CHURCH\nFOLLOW THE PATH LEADING TO\nTHE NORTH TO SAVE\nWHO YOU CAN.GOOD LUCK!"));
            break;
        case MAGE:
            font3x5.print(F("I SENSE A SOUL IN\nSEARCH OF ANSWERS\n\n"));
            font3x5.print(readFlashStringPointer(&npcs_text[npc_text_selection + MAGE_TEXT_OFFSET]));
            break;
        case SECRET:
            font3x5.print(F("WELCOME TO THE\nSECRET YODARUB FAN CLUB!\nYOU GOT YODARUBBED!"));
            arduboy.drawBitmap(30, 30, secret_image, 14, 16);
            arduboy.drawBitmap(75, 30, secret_image, 14, 16);
    }    
    
}

void draw_npcs() {
    if (on_screen(ELDER_X * 16, ELDER_Y * 16, false)) {
        arduboy.drawBitmap(ELDER_X * 16 - player.scrollx + 4, ELDER_Y * 16 - player.scrolly, elder_image, 9, 16);
    }
    if (on_screen(INNKEEPER_X * 16, INNKEEPER_Y * 16, false)) {
        arduboy.drawBitmap(INNKEEPER_X * 16 - player.scrollx + 2, INNKEEPER_Y * 16 - player.scrolly, innkeeper_image, 14, 16);
    }
    if (on_screen(BLACKSMITH_X * 16, BLACKSMITH_Y * 16, false)) {
        arduboy.drawBitmap(BLACKSMITH_X * 16 - player.scrollx + 4, BLACKSMITH_Y * 16 - player.scrolly, blacksmith_image, 12, 16);
    }
    if (on_screen(HEALER_X * 16, HEALER_Y * 16, false)) {
        arduboy.drawBitmap(HEALER_X * 16 - player.scrollx + 4, HEALER_Y * 16 - player.scrolly, healer_image, 9, 16);
    }
    if (on_screen(MAGE_X * 16, MAGE_Y * 16, false)) {
        arduboy.drawBitmap(MAGE_X * 16 - player.scrollx + 3, MAGE_Y * 16 - player.scrolly, mage_image, 13, 16);
    }
}
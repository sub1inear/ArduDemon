#include "item.h"
#include "item_images.h"
#include "tools.h"

#include "player.h"


extern Arduboy2 arduboy;
extern Player player;

void Item::init(int16_t x, int16_t y, uint8_t type, uint8_t pwramt, uint8_t magic_stat, uint8_t magic_bonus) {
    this->x = x;
    this->y = y;
    this->type = type;
    this->pwramt = pwramt;
    this->size = (type == GOLD) ? 16 : 8;
    this->magic_stat = magic_stat;
    this->magic_bonus = magic_bonus;

}

void Item::draw() {
    if (on_screen(x, y, false)) {
        arduboy.drawBitmap(x - player.scrollx, y - player.scrolly, pgm_read_ptr(&loot_lut[type]), size, size);
    }
}   
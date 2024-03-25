#include "tools.h"
#include "player.h"

extern Player player;

bool on_screen(int16_t x, int16_t y, bool boss) {
    return (x + 16 - player.scrollx > 0 && // do bounds checking
        x - player.scrollx < WIDTH &&
        y + 16 + boss * 8 - player.scrolly > 0 &&
        y - player.scrolly < HEIGHT);
};
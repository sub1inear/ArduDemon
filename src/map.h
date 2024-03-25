#pragma once

#include <Arduboy2.h>

using uint24_t = __uint24;


class TileMap {
public:
    const uint8_t * source;
    uint8_t width;
    uint8_t height;
    uint8_t dungeon_size;

    uint8_t start_tile_x;
    uint8_t start_tile_y;

    uint8_t exit_tile_x;
    uint8_t exit_tile_y;

    bool dungeon;

    void init(const uint8_t *source, uint8_t map_width, uint8_t map_height, bool dungeon, uint8_t map_start_tile_x, uint8_t map_start_tile_y, uint8_t map_exit_tile_x, uint8_t map_exit_tile_y);

    void draw();
    void draw_town();
    void draw_dungeon();
};

#include "map.h"
#include "map_images.h"
#include "tools.h"
#include "player.h"
#include "dungeon.h"
#include "Font3x5.h"

extern Arduboy2 arduboy;
extern Player player;
extern Font3x5 font3x5;
extern uint8_t dungeon_level;


void TileMap::init(const uint8_t *source, uint8_t width, uint8_t height, bool dungeon,
                    uint8_t start_tile_x, uint8_t start_tile_y,
                    uint8_t exit_tile_x, uint8_t exit_tile_y) {
    this->source = source;

    this->width = width;
    this->height = height;

    this->dungeon = dungeon;
    
    this->dungeon_size = width * height / 8 - 1;


    this->start_tile_x = start_tile_x;
    this->start_tile_y = start_tile_y;

    this->exit_tile_x = exit_tile_x;
    this->exit_tile_y = exit_tile_y;
}

void TileMap::draw() {
    if (dungeon) {
        draw_dungeon();
    } else { 
        draw_town();
    }
}

void TileMap::draw_town() {

    uint16_t tile_idx = player.scrollx / 16 + width * (player.scrolly / 16);

    for (uint8_t y = 0; y < 64 + 16; y += 16) {
        for (uint8_t x = 0; x < 128 + 16; x += 16) {           
            Sprites::drawOverwrite(x - (player.scrollx & 0xf), y - (player.scrolly & 0xf), tiles, pgm_read_byte(source + tile_idx));
            tile_idx += 1;
        }
        tile_idx += width - (128 + 16) / 16;
    }
    if (on_screen(exit_tile_x * 16, exit_tile_y * 16, false)) { // draw exit tile
        arduboy.drawBitmap(exit_tile_x * 16 - player.scrollx, exit_tile_y * 16 - player.scrolly, stairsdown_image, 16, 16);
    }
}

void TileMap::draw_dungeon() {

    // renders a bit-packed dungeon and autotiles on the fly

    uint8_t byte = player.scrollx / 16 + (player.scrolly / 16 / 8) * width;
    uint8_t bit = (player.scrolly / 16) & 0x7;

    bool surrounding_bits[8];
    for (uint8_t y = 0; y < 64 + 16; y += 16) {
        for (uint8_t x = 0; x < 128 + 16; x += 16) {
            if (bitRead(source[byte], bit)) {
                // read the tiles next to this one
                // crossovers to the next row are allowed in right & left
                // but because nothing will be on the edges they're always 1
                // surrounding bits:
                /* 7    2     6
                      ------
                   3  |Tile|  1
                      ------
                   4    0     5 */

                // down
                if (bit == 7) {
                    if (byte + width < dungeon_size) {
                        surrounding_bits[0] = bitRead(source[byte + width], 0);
                    } else {
                        surrounding_bits[0] = true;
                    }
                } else {
                    surrounding_bits[0] = bitRead(source[byte], bit + 1);    
                }
                // right
               surrounding_bits[1] = bitRead(source[min(byte + 1, dungeon_size)], bit);
                // up
                if (bit > 0) {
                    surrounding_bits[2] = bitRead(source[byte], bit - 1); 
                } else {
                    surrounding_bits[2] = bitRead(source[max(byte - width, 0)], 7);
                }
                
                // left
                surrounding_bits[3] = bitRead(source[max(byte - 1, 0)], bit); 
                
                // down-left
                if (bit == 7) {
                    if (byte - 1 + width < dungeon_size) {
                        surrounding_bits[4] = bitRead(source[max(byte - 1, 0) + width], 0);
                    } else {
                        surrounding_bits[4] = true;
                    }
                } else {
                    surrounding_bits[4] = bitRead(source[max(byte - 1, 0)], bit + 1);    
                }

                // down-right
                if (bit == 7) {
                    if (byte + 1 + width < dungeon_size) {
                        surrounding_bits[5] = bitRead(source[byte + 1 + width], 0);
                    } else {
                        surrounding_bits[5] = true;
                    }
                } else {
                    surrounding_bits[5] = bitRead(source[min(byte + 1, dungeon_size)], bit + 1);    
                }

                // up-right
                if (bit > 0) {
                    surrounding_bits[6] = bitRead(source[min(byte + 1, dungeon_size)], bit - 1); 
                } else {
                    surrounding_bits[6] = bitRead(source[max(min(byte + 1, dungeon_size) - width, 0)], 7);
                }
                // up-left
                if (bit > 0) {
                    surrounding_bits[7] = bitRead(source[max(byte - 1, 0)], bit - 1); 
                } else {
                    surrounding_bits[7] = bitRead(source[max(byte - 1 - width, 0)], 7);
                }

                // draw with autotileing
                if (!(surrounding_bits[0] & // use bitwise and instead of logical and for speed and smaller code
                    surrounding_bits[1] &
                    surrounding_bits[2] &
                    surrounding_bits[3] &
                    surrounding_bits[4] &
                    surrounding_bits[5] &
                    surrounding_bits[6] &
                    surrounding_bits[7])) { // not fully surrounded and not on very edge
                    
                    if (surrounding_bits[2] &
                        surrounding_bits[0]) { // side tile
                        if (!surrounding_bits[3] & !surrounding_bits[1]) { // if both sides are clear, just make it a top-down tile
                            Sprites::drawOverwrite(x - (player.scrollx & 0xf), y - (player.scrolly & 0xf), tiles, 6);
                        } else if (!surrounding_bits[3] | !surrounding_bits[4] | !surrounding_bits[7]) { // left
                            arduboy.fillRect(x - (player.scrollx & 0xf), y - (player.scrolly & 0xf), 3, 16, WHITE);
                        } else if (!surrounding_bits[1] | !surrounding_bits[5] | !surrounding_bits[6]) { // right
                            arduboy.fillRect(x - (player.scrollx & 0xf) + 16 - 3, y - (player.scrolly & 0xf), 3, 16, WHITE);
                        } 
                    
                    } else if (surrounding_bits[0] & (!surrounding_bits[7] |
                              !surrounding_bits[6]) & (!surrounding_bits[3] ^
                              !surrounding_bits[1]) & (surrounding_bits[4] |
                               surrounding_bits[5])) { // corner tile (top)
                        Sprites::drawOverwrite(x - (player.scrollx & 0xf), y - (player.scrolly & 0xf), tiles, 6);
                        if (surrounding_bits[3]) {
                            arduboy.fillRect(x - (player.scrollx & 0xf) + 16 - 3, y - (player.scrolly & 0xf), 3, 16, WHITE);
                        } else {
                            arduboy.fillRect(x - (player.scrollx & 0xf), y - (player.scrolly & 0xf), 3, 16, WHITE);

                        }
                    } else { // top down tile
                        Sprites::drawOverwrite(x - (player.scrollx & 0xf), y - (player.scrolly & 0xf), tiles, 6);
                    }

                    
                }

            } 
            byte++;
        }
        bit++;
        byte -= (128 + 16) / 16;

        if (bit > 7)  {
            bit = 0;
            byte += width;
        }
    
    }
    if (dungeon_level < NUM_LEVELS - 1) {
        if (on_screen(exit_tile_x * 16, exit_tile_y * 16, false)) { // draw exit tile
            arduboy.drawBitmap(exit_tile_x * 16 - player.scrollx, exit_tile_y * 16 - player.scrolly, stairsdown_image, 16, 16);
        }
    }
    if (on_screen(start_tile_x * 16, start_tile_y * 16, false)) { // draw back up tile
        arduboy.drawBitmap(start_tile_x * 16 - player.scrollx,
                        start_tile_y * 16 - player.scrolly,
                        stairsup_image, 16, 16); 
    }
    font3x5.setCursor(100, 0);

    font3x5.print(F("LEVEL "));

    font3x5.print(dungeon_level);
}


#pragma once

#include <Arduboy2.h>


#define DUNGEON_WIDTH 32
#define DUNGEON_HEIGHT 32 
#define DUNGEON_TILE_WIDTH (DUNGEON_WIDTH / 8)
#define DUNGEON_TILE_HEIGHT (DUNGEON_HEIGHT / 8)

#define ROOMS_MAX 6
#define CORRIDORS_MAX ROOMS_MAX - 1

#define NUM_ENEMIES 20

#define NUM_LEVELS 7

void gen_dungeon();

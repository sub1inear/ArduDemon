#include "dungeon.h"
#include "enemy.h"
#include "enemy_images.h"
#include "map.h"

extern Arduboy2 arduboy;
extern Enemy enemies[NUM_ENEMIES];
extern Fireball boss_fireball;
extern TileMap dungeon_map; 
extern uint32_t seed;
extern uint8_t dungeon_level;

const uint8_t PROGMEM level_enemy_lut[] = {
    0,
    0,
    2,
    2,
    4,
    4
};


typedef struct {
    Point start;
    Point end;
    bool middle; // set for L-shapes
} Corridor;

static uint8_t rooms_len = 0;
static Rect rooms[ROOMS_MAX] {};

static Corridor corridors[CORRIDORS_MAX] {};

static Rect new_room {};
static Rect larger_room {};

uint8_t dungeon_array[DUNGEON_WIDTH * DUNGEON_TILE_HEIGHT] {};

static bool rooms_overlap(Rect *room, Rect *rooms) {
    // prevent flush rooms
    larger_room = Rect(room->x - 1, room->y - 1, room->width + 2, room->height + 2);

    for (uint8_t i = 0; i < rooms_len; i++) {
        if (Arduboy2::collide(larger_room, rooms[i])) {
            return true;
        }
    }

    return false;
}

static Rect gen_room() {
    // note: -1 and 1s to prevent rooms on the edge (rooms need tiles surrounding them)

    new_room.width = random(5, 8);
    new_room.height = random(5, 8);

    new_room.x = random(1, DUNGEON_WIDTH - new_room.width - 2);
    new_room.y = random(1, DUNGEON_HEIGHT - new_room.height - 2);
    
    return new_room;
}


static Corridor gen_corridor(Rect *room1, Rect *room2) {
    Corridor corridor;
    corridor.middle = false;

    // note: +1 and -1 to prevent corridors on the very edge of a room
    if (room1->y < (room2->y + room2->height) && room2->y < (room1->y + room1->height)) { // horizontal joins
        corridor.start.x = (room1->x < room2->x) ? room1->x + room1->width : room1->x;
        corridor.start.y = random(max(room1->y + 1, room2->y), min(room2->y + room2->height, room1->y + room1->height - 1));

        corridor.end.x = (room1->x < room2->x) ? room2->x : room2->x + room2->width;
        corridor.end.y = corridor.start.y;

    } else if (room1->x < (room2->x + room2->width) && room2->x < (room1->x + room1->width)) { // vertical joins
        corridor.start.x = random(max(room1->x + 1, room2->x), min(room2->x + room2->width, room1->x + room1->width - 1));
        corridor.start.y = (room1->y < room2->y) ? room1->y + room1->height : room1->y;

        corridor.end.x = corridor.start.x;
        corridor.end.y = (room1->y < room2->y) ? room2->y : room2->y + room2->height;

    } else { // L-joins
        corridor.start.x = random(room1->x + 1, room1->x + room1->width - 1);
        corridor.start.y = (room1->y < room2->y) ? room1->y + room1->height : room1->y;
        
        corridor.end.x = (room1->x < room2->x) ? room2->x : room2->x + room2->width;
        corridor.end.y = random(room2->y + 1, room2->y + room2->height - 1);
        
        corridor.middle = true;
    }
    
    return corridor;
}


static void draw_hline_to_array(uint8_t x, uint8_t y, uint8_t w) { // ported from Arduboy2 lib

    // buffer pointer plus row offset + x offset
   register uint8_t *pBuf = dungeon_array + ((y / 8) * DUNGEON_WIDTH) + x;
  
   // pixel mask
   register uint8_t mask = ~(1 << (y & 7));
    while (w--) {
        *pBuf++ &= mask;
    }
}

static void draw_vline_to_array(uint8_t x, uint8_t y, uint8_t h) {
    
    for (int i = y; i <= y + h; i++) {
        bitClear(dungeon_array[x + (i / 8) * DUNGEON_WIDTH], i & 0x7);
    }
}

static void draw_room_to_array(Rect *room) {
    for (uint8_t i = room->x; i < room->x + room->width; i++) {
        draw_vline_to_array(i, room->y, room->height);
    }
};

static void draw_straight_line_to_array(Point *start, Point *end) {
    if (start->y == end->y) { // horizontal line
        if (start->x < end->x) {
            draw_hline_to_array(start->x, start->y, end->x - start->x);
        } else {
            draw_hline_to_array(end->x, end->y, start->x - end->x);
        }
    } else { // vertical line
        if (start->y < end->y) {
            draw_vline_to_array(start->x, start->y, end->y - start->y);
        } else {
            draw_vline_to_array(end->x, end->y, start->y - end->y);
        }        
    }
}


static void draw_corridor_to_array(Corridor *corridor) {
    if (corridor->middle) {
        Point middle = {corridor->start.x, corridor->end.y};
        draw_straight_line_to_array(&corridor->start, &middle);
        draw_straight_line_to_array(&middle, &corridor->end);

    } else {
        draw_straight_line_to_array(&corridor->start, &corridor->end);
    }
}

static void draw_dungeon_to_array() {

    memset(dungeon_array, 0b11111111, DUNGEON_WIDTH * DUNGEON_TILE_HEIGHT);

    for (uint8_t i = 0; i < ROOMS_MAX; i++) {
        draw_room_to_array(&rooms[i]);
    }
    for (uint8_t i = 0; i < CORRIDORS_MAX; i++) {
        draw_corridor_to_array(&corridors[i]);
    }
}

void gen_dungeon() { 
    
    randomSeed(seed + dungeon_level);
    rooms_len = 0;

    for (uint8_t i = 0; i < ROOMS_MAX; i++) {
        new_room = gen_room();

        if (rooms_overlap(&new_room, rooms)) {
            uint8_t h = 0; // needs to be in scope for check later
            for (; h < 20; h++) {
                new_room = gen_room();
                if (!rooms_overlap(&new_room, rooms)) break;

            }
            // if (h == 20) {
            //     gen_dungeon();
            //     return;
            // }
            
        }
        
        rooms[i] = new_room;
        rooms_len++;

    }
    for (uint8_t i = 0; i < CORRIDORS_MAX; i++) {
        corridors[i] = gen_corridor(&rooms[i], &rooms[i + 1]);
    }

    draw_dungeon_to_array();
    
    uint8_t dungeon_start_tile_x = rooms[0].x;
    uint8_t dungeon_start_tile_y = rooms[0].y;

    uint8_t dungeon_exit_tile_x = rooms[1].x + random(rooms[1].width); // even if level is last level, it won't register exiting so this is safe
    uint8_t dungeon_exit_tile_y = rooms[1].y + random(rooms[1].height);

    dungeon_map.init(dungeon_array, DUNGEON_WIDTH, DUNGEON_HEIGHT, true, dungeon_start_tile_x, dungeon_start_tile_y, dungeon_exit_tile_x, dungeon_exit_tile_y);
    if (dungeon_level < NUM_LEVELS - 1) {
        for (uint8_t i = 0; i < NUM_ENEMIES; i++) {
            new_room = rooms[random(ROOMS_MAX)];

            enemy_t enemy_type = (enemy_t) (pgm_read_byte(level_enemy_lut + dungeon_level) + random(0, 2)); 
            enemies[i].init(new_room.x + random(new_room.width), new_room.y + random(new_room.height), pgm_read_ptr(&enemy_lut[enemy_type]), enemy_type);
        }
    } else { // boss level
        new_room = rooms[random(ROOMS_MAX)];
        
        enemies[0].init(new_room.x + new_room.width / 2, new_room.y + new_room.height / 2, boss_image, BOSS);
        // init fireball
        boss_fireball.x = enemies[0].x;
        boss_fireball.y = enemies[0].y;
        
        for (uint8_t i = 1; i < NUM_ENEMIES; i++) {  // fill up enemy array with dead monsters
            enemies[i].init(0, 0, (const uint8_t *)nullptr, SKELETON); 
            enemies[i].dead = true; 
        }
    }

    arduboy.initRandomSeed();

}

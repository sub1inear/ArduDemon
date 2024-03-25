#include "saveload.h"

extern Player player;
extern Item items[MAX_ITEMS];
extern uint8_t num_items;
extern uint8_t reached_dungeon_level;
extern uint32_t seed;


// CRC-16 with poly=0x8005 init=0
static uint16_t eeprom_crc16(const uint8_t *addr, uint8_t size) {
    uint16_t crc = 0;
    for (uint8_t i = 0; i < size; i++) {
        crc ^= eeprom_read_byte(&addr[i]);
        for (uint8_t j = 8; j != 0; j--)
            crc = crc & 0x01 ? (crc >> 1) ^ 0xa001 : crc >> 1;
    }
    return crc;
}

static uint8_t pack_nibbles(uint8_t a, uint8_t b) {
    return (a << 0) | (b << 4);
}
static void unpack_nibbles(uint8_t value, uint8_t &a, uint8_t &b) {
    a = (value >> 0) & 0xf;
    b = (value >> 4) & 0xf;
}

void save_game() {
    // store the header
    eeprom_update_byte((uint8_t *)EEPROM_HEADER, HEADER_VALUE);

    // store the data
    uint8_t *addr = EEPROM_DATA;
    eeprom_update_block(player.stats, addr, sizeof(player.stats)); addr += sizeof(player.stats);

    eeprom_update_dword((uint32_t *)addr, seed); addr += sizeof(uint32_t);

    for (uint8_t i = 0; i < MAX_INV; i++) {
        eeprom_update_byte(addr, pack_nibbles(player.inventory[i].type, player.inventory[i].magic_stat)); addr += sizeof(uint8_t);
        eeprom_update_byte(addr, player.inventory[i].magic_bonus); addr += sizeof(uint8_t);
        eeprom_update_byte(addr, player.inventory[i].pwramt); addr += sizeof(uint8_t);
    }
    eeprom_update_byte(addr, pack_nibbles(player.num_inv, reached_dungeon_level)); addr += sizeof(uint8_t);

    eeprom_update_byte(addr, pack_nibbles(player.weapon.type, player.weapon.magic_stat)); addr += sizeof(uint8_t);
    eeprom_update_byte(addr, player.weapon.magic_bonus); addr += sizeof(uint8_t);
    eeprom_update_byte(addr, player.weapon.pwramt); addr += sizeof(uint8_t);

    eeprom_update_byte(addr, pack_nibbles(player.shield.type, player.shield.magic_stat)); addr += sizeof(uint8_t);
    eeprom_update_byte(addr, player.shield.magic_bonus); addr += sizeof(uint8_t);
    eeprom_update_byte(addr, player.shield.pwramt); addr += sizeof(uint8_t);

    if (addr != EEPROM_CHECKSUM) {
        // breakpoint here
    }
    // update the checksum
    uint16_t checksum = eeprom_crc16(EEPROM_DATA, EEPROM_DATA_SIZE);
    eeprom_update_word((uint16_t *)EEPROM_CHECKSUM, checksum);
}

bool load_game() {

    // verify the header
    if (eeprom_read_byte((uint8_t *)EEPROM_HEADER) != HEADER_VALUE)
        return false;

    // verify the checksum
    uint16_t checksum = eeprom_read_word((uint16_t *)EEPROM_CHECKSUM);
    if (eeprom_crc16(EEPROM_DATA, EEPROM_DATA_SIZE) != checksum)
        return false;

    // load the data
    uint8_t *addr = EEPROM_DATA;
    eeprom_read_block(player.stats, addr, sizeof(player.stats)); addr += sizeof(player.stats);

    player.max_hp = player.stats[CON] * 2;
    player.hp = player.max_hp;

    seed = eeprom_read_dword((uint32_t *)addr); addr += sizeof(uint32_t);

    for (uint8_t i = 0; i < MAX_INV; i++) {
        unpack_nibbles(eeprom_read_byte(addr), player.inventory[i].type, player.inventory[i].magic_stat); addr += sizeof(uint8_t);
        player.inventory[i].magic_bonus = eeprom_read_byte(addr); addr += sizeof(uint8_t);
        player.inventory[i].pwramt = eeprom_read_byte(addr); addr += sizeof(uint8_t);
        player.inventory[i].size = (player.inventory[i].type == GOLD) ? 16 : 8;
    }
    unpack_nibbles(eeprom_read_byte(addr), player.num_inv, reached_dungeon_level); addr += sizeof(uint8_t);

    unpack_nibbles(eeprom_read_byte(addr), player.weapon.type, player.weapon.magic_stat); addr += sizeof(uint8_t);
    player.weapon.magic_bonus = eeprom_read_byte(addr); addr += sizeof(uint8_t);
    player.weapon.pwramt = eeprom_read_byte(addr); addr += sizeof(uint8_t);

    unpack_nibbles(eeprom_read_byte(addr), player.shield.type, player.shield.magic_stat); addr += sizeof(uint8_t);
    player.shield.magic_bonus = eeprom_read_byte(addr); addr += sizeof(uint8_t);
    player.shield.pwramt = eeprom_read_byte(addr); addr += sizeof(uint8_t);

    if (addr != EEPROM_CHECKSUM) {
        // breakpoint here
    }
    
    // calculate level
    uint8_t level_stat = 10;
    if (player.weapon.magic_stat != NOSTAT) {
        level_stat = player.stats[player.weapon.magic_stat] - player.weapon.magic_bonus;
        if (player.weapon.magic_stat == player.shield.magic_stat) { // weapon cannot be NOSTAT so shield cannot be either
            level_stat -= player.shield.magic_bonus;
        }
    } else if (player.shield.magic_stat != NOSTAT) {
        level_stat = player.stats[player.shield.magic_stat] - player.shield.magic_bonus; 
    } else {
        level_stat = player.stats[0];
    }
    level_stat -= 10;
    player.level = level_stat / 5;
    return true;
    
    

}
void clear_save() {
    eeprom_update_byte(EEPROM_HEADER, 0xFF);
}
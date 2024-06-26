#pragma once
#include <Arduboy2.h>

#define NUM_ENEMY_TYPES 6 // Not including Boss

const unsigned char PROGMEM skeleton_images[] =
{
// width, height,
16, 16,
// FRAME 00
0x00, 0xc0, 0xe0, 0xe0, 0xc0, 0x8c, 0xb2, 0xfe, 0xb2, 0x8c, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 
0x00, 0x01, 0x03, 0x03, 0x01, 0x78, 0x15, 0x1f, 0x15, 0xf8, 0x81, 0x02, 0x07, 0x02, 0x00, 0x00, 
// FRAME 01
0x00, 0xc0, 0xe0, 0xe0, 0xc0, 0x8c, 0xb2, 0xfe, 0xb2, 0x8c, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 
0x00, 0x01, 0x03, 0x03, 0x81, 0xf8, 0x15, 0x1f, 0x15, 0x78, 0x01, 0x02, 0x07, 0x02, 0x00, 0x00, 
// FRAME 02
0x00, 0x00, 0x00, 0x00, 0x00, 0x8c, 0x9e, 0xfe, 0xb2, 0xbc, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1b, 0xff, 0x8a, 0x02, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 
// FRAME 03
0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xcf, 0xff, 0x59, 0x5e, 0x80, 0xf8, 0x80, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xc0, 0x60, 0x1d, 0x0f, 0x15, 0x61, 0x81, 0x41, 0x00, 0x00, 0x00, 0x00, 
// FRAME 04
0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x8c, 0xbe, 0xfe, 0xbe, 0x8c, 0xc0, 0xe0, 0xe0, 0xc0, 0x00, 
0x00, 0x00, 0x02, 0x07, 0x02, 0x01, 0x78, 0x15, 0x1f, 0x15, 0xf8, 0x81, 0x03, 0x03, 0x01, 0x00, 
// FRAME 05
0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x8c, 0xbe, 0xfe, 0xbe, 0x8c, 0xc0, 0xe0, 0xe0, 0xc0, 0x00, 
0x00, 0x00, 0x02, 0x07, 0x02, 0x81, 0xf8, 0x15, 0x1f, 0x15, 0x78, 0x01, 0x03, 0x03, 0x01, 0x00, 
// FRAME 06
0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xbc, 0xb2, 0xfe, 0x9e, 0x8c, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1f, 0x06, 0x02, 0x8a, 0xff, 0x1b, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 07
0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x5e, 0x59, 0xff, 0xcf, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x4f, 0x83, 0x61, 0x15, 0x0f, 0x1d, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char PROGMEM zombie_images[] =
{
// width, height,
16, 16,
// FRAME 00
0x00, 0x00, 0x00, 0x00, 0xc0, 0xae, 0xb1, 0x91, 0x51, 0x2e, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x02, 0x87, 0xfa, 0x00, 0x00, 0x01, 0x3a, 0x07, 0x02, 0x00, 0x00, 0x00, 0x00, 
// FRAME 01
0x00, 0x00, 0x00, 0x00, 0xc0, 0xae, 0xb1, 0x91, 0x51, 0x2e, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x02, 0x07, 0x3a, 0x00, 0x00, 0x01, 0xfa, 0x87, 0x02, 0x00, 0x00, 0x00, 0x00, 
// FRAME 02
0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0x11, 0x91, 0x91, 0xee, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x11, 0xf1, 0x91, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 03
0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0x11, 0x91, 0x91, 0xee, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xc0, 0x6f, 0x19, 0x19, 0x19, 0x6e, 0x80, 0x40, 0x00, 0x00, 0x00, 0x00, 
// FRAME 04
0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x2e, 0x5f, 0x9f, 0xbf, 0xae, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x02, 0x07, 0x3a, 0x01, 0x00, 0x00, 0xfa, 0x87, 0x02, 0x00, 0x00, 0x00, 
// FRAME 05
0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x2e, 0x5f, 0x9f, 0xbf, 0xae, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x02, 0x07, 0x3a, 0x01, 0x00, 0x00, 0xfa, 0x87, 0x02, 0x00, 0x00, 0x00, 
// FRAME 06
0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xee, 0x91, 0x91, 0x11, 0xee, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x91, 0xf1, 0x11, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 07
0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xee, 0x91, 0x91, 0x11, 0xee, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x40, 0x80, 0x6e, 0x19, 0x19, 0x19, 0x6f, 0xc0, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char PROGMEM orc_images[] =
{
// width, height,
16, 16,
// FRAME 00
0x00, 0x00, 0x02, 0x81, 0xfa, 0x64, 0xa4, 0x7a, 0xc1, 0xc2, 0xa0, 0x70, 0xf8, 0x70, 0xa0, 0x00, 
0x04, 0x02, 0x03, 0x81, 0xff, 0x35, 0x0a, 0x75, 0x1f, 0x01, 0x02, 0x0c, 0x0f, 0x00, 0x00, 0x00, 
// FRAME 01
0x00, 0x00, 0x02, 0x81, 0xfa, 0x64, 0xa4, 0x7a, 0xc1, 0xc2, 0xa0, 0x70, 0xf8, 0x70, 0xa0, 0x00, 
0x04, 0x02, 0x03, 0x01, 0x7f, 0x15, 0x3a, 0xf5, 0x9f, 0x01, 0x02, 0x0c, 0x0f, 0x00, 0x00, 0x00, 
// FRAME 02
0x00, 0x00, 0x00, 0xc0, 0x38, 0xe5, 0xa6, 0x38, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0x08, 0xf7, 0x97, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 03
0x00, 0x00, 0x00, 0xc0, 0xba, 0xe5, 0xa6, 0x38, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xc0, 0x67, 0x18, 0x11, 0x13, 0x6f, 0x87, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 04
0x00, 0xa0, 0x70, 0xf8, 0x70, 0xa0, 0xc2, 0xc1, 0x7a, 0xbc, 0x7c, 0xfa, 0x81, 0x02, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0x0c, 0x02, 0x01, 0x1f, 0x75, 0x0a, 0x35, 0xff, 0x81, 0x03, 0x02, 0x04, 
// FRAME 05
0x00, 0xa0, 0x70, 0xf8, 0x70, 0xa0, 0xc2, 0xc1, 0x7a, 0xbc, 0x7c, 0xfa, 0x81, 0x02, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0x0c, 0x02, 0x01, 0x9f, 0xf5, 0x3a, 0x15, 0x7f, 0x01, 0x03, 0x02, 0x04, 
// FRAME 06
0x00, 0x00, 0x00, 0x50, 0x38, 0xfc, 0x38, 0xd0, 0x38, 0xa6, 0xe5, 0xb8, 0xc0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 0x07, 0x0f, 0x93, 0xf3, 0x09, 0x07, 0x00, 0x00, 0x00, 
// FRAME 07
0x00, 0x00, 0x00, 0x50, 0x38, 0xfc, 0x38, 0xd0, 0x38, 0xa6, 0xe5, 0xba, 0xc0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x46, 0x87, 0x6f, 0x13, 0x13, 0x19, 0x67, 0xc0, 0x00, 0x00,
};

const unsigned char PROGMEM warwolf_images[] =
{
// width, height,
16, 16,
// FRAME 00
0x00, 0x00, 0x00, 0x00, 0xb8, 0x74, 0x04, 0x8e, 0x05, 0x74, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1f, 0x66, 0x19, 0x20, 0x19, 0x66, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 01
0x00, 0x00, 0x00, 0x00, 0xfc, 0x3a, 0x82, 0x47, 0x82, 0x3a, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0c, 0x10, 0x0c, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 02
0x00, 0x00, 0x00, 0x80, 0xc0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x48, 0xc8, 0x50, 0x20, 0x00, 
0x01, 0x03, 0x03, 0x03, 0x01, 0x1a, 0x0e, 0x04, 0x04, 0x02, 0x02, 0x1d, 0x00, 0x00, 0x00, 0x00, 
// FRAME 03
0x80, 0x80, 0x80, 0xc0, 0xe0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0xa4, 0x64, 0x28, 0x10, 0x00, 
0x00, 0x01, 0x01, 0x09, 0x0c, 0x07, 0x03, 0x02, 0x0a, 0x05, 0x0b, 0x07, 0x00, 0x00, 0x00, 0x00, 
// FRAME 04
0x00, 0x00, 0x00, 0x00, 0xe0, 0x70, 0x98, 0x04, 0x98, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x80, 0x7d, 0x2e, 0x20, 0x71, 0xa0, 0x2e, 0x7d, 0x80, 0x00, 0x00, 0x00, 0x00, 
// FRAME 05
0x00, 0x00, 0x00, 0x00, 0xe0, 0x38, 0x4c, 0x82, 0x4c, 0x38, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x3f, 0x37, 0x10, 0x38, 0x40, 0x37, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 06
0x00, 0x20, 0x50, 0xc8, 0x48, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0xc0, 0x80, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1d, 0x02, 0x02, 0x04, 0x04, 0x0e, 0x1a, 0x01, 0x03, 0x03, 0x03, 0x01, 
// FRAME 07
0x00, 0x10, 0x28, 0x64, 0xa4, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0xe0, 0xc0, 0x80, 0x80, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x07, 0x0b, 0x05, 0x0a, 0x02, 0x03, 0x07, 0x0c, 0x09, 0x01, 0x01, 0x00,
};


const unsigned char PROGMEM fallenone_images[] =
{
// width, height,
16, 16,
// FRAME 00
0x00, 0x00, 0x00, 0x00, 0x80, 0xd8, 0x24, 0x24, 0x58, 0x80, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x00, 
0x00, 0x00, 0x06, 0x07, 0x09, 0x36, 0x00, 0x00, 0x76, 0x89, 0x03, 0x06, 0x06, 0x0f, 0x04, 0x00, 
// FRAME 01
0x00, 0x00, 0x00, 0x00, 0x80, 0xd8, 0x24, 0x24, 0x58, 0x80, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x00, 
0x00, 0x00, 0x06, 0x07, 0x89, 0x76, 0x00, 0x00, 0x36, 0x09, 0x03, 0x06, 0x06, 0x0f, 0x04, 0x00, 
// FRAME 02
0x00, 0x00, 0x00, 0x00, 0x00, 0xec, 0x32, 0x92, 0xec, 0x40, 0x00, 0xf0, 0x08, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf1, 0x88, 0x07, 0x01, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 
// FRAME 03
0x00, 0x00, 0x00, 0x00, 0x00, 0xf6, 0x99, 0x49, 0xf6, 0xa0, 0x80, 0xf8, 0x84, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xc0, 0x67, 0x18, 0x08, 0x17, 0x60, 0x80, 0x41, 0x00, 0x00, 0x00, 0x00, 
// FRAME 04
0x00, 0x10, 0xe0, 0x00, 0x00, 0x00, 0x80, 0x58, 0xbc, 0xbc, 0xd8, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x04, 0x0f, 0x06, 0x06, 0x03, 0x89, 0x76, 0x02, 0x01, 0x36, 0x09, 0x07, 0x06, 0x00, 0x00, 
// FRAME 05
0x00, 0x10, 0xe0, 0x00, 0x00, 0x00, 0x80, 0x58, 0xbc, 0xbc, 0xd8, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x04, 0x0f, 0x06, 0x06, 0x03, 0x09, 0x36, 0x02, 0x01, 0x76, 0x89, 0x07, 0x06, 0x00, 0x00, 
// FRAME 06
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xec, 0x92, 0xb2, 0xec, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x07, 0x89, 0xf1, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 07
0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xc0, 0xf6, 0x49, 0x99, 0xf6, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x40, 0x80, 0x60, 0x17, 0x08, 0x18, 0x67, 0xc0, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char PROGMEM gargoyle_images[] =
{
// width, height,
16, 16,
// FRAME 00
0x00, 0xa4, 0xf8, 0x54, 0xa0, 0x20, 0xd8, 0x24, 0x24, 0xd8, 0x20, 0xa0, 0x54, 0xf8, 0xa4, 0x00, 
0x00, 0x00, 0x07, 0x06, 0x09, 0x0a, 0x3f, 0x08, 0x08, 0x7f, 0x8a, 0x09, 0x06, 0x07, 0x00, 0x00, 
// FRAME 01
0x00, 0xa4, 0xf8, 0x54, 0xa0, 0x20, 0xd8, 0x24, 0x24, 0xd8, 0x20, 0xa0, 0x54, 0xf8, 0xa4, 0x00, 
0x00, 0x00, 0x07, 0x06, 0x09, 0x8a, 0x7f, 0x08, 0x08, 0x3f, 0x0a, 0x09, 0x06, 0x07, 0x00, 0x00, 
// FRAME 02
0xed, 0x12, 0xc4, 0x1a, 0x24, 0xd8, 0x38, 0xa4, 0x24, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x1f, 0x30, 0x2b, 0x20, 0x10, 0x17, 0x08, 0xf7, 0x88, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 03
0xf7, 0x09, 0xe2, 0x0d, 0x12, 0xec, 0x1c, 0x52, 0x92, 0xec, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x0f, 0x18, 0x15, 0x10, 0xc8, 0x6b, 0x1c, 0x08, 0x15, 0x67, 0x80, 0x40, 0x00, 0x00, 0x00, 0x00, 
// FRAME 04
0x00, 0x48, 0xf0, 0xa8, 0x40, 0x20, 0xd8, 0x3c, 0x3c, 0xd8, 0x20, 0x40, 0xa8, 0xf0, 0x48, 0x00, 
0x00, 0x01, 0x0f, 0x0c, 0x13, 0x0a, 0x3f, 0x08, 0x08, 0x7f, 0x8a, 0x13, 0x0c, 0x0f, 0x01, 0x00, 
// FRAME 05
0x00, 0x48, 0xf0, 0xa8, 0x40, 0x20, 0xd8, 0x3c, 0x3c, 0xd8, 0x20, 0x40, 0xa8, 0xf0, 0x48, 0x00, 
0x00, 0x01, 0x0f, 0x0c, 0x13, 0x8a, 0x7f, 0x08, 0x08, 0x3f, 0x0a, 0x13, 0x0c, 0x0f, 0x01, 0x00, 
// FRAME 06
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x24, 0xa4, 0x38, 0xd8, 0x24, 0x1a, 0xc4, 0x12, 0xed, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x88, 0xf7, 0x08, 0x17, 0x10, 0x20, 0x2b, 0x30, 0x1f, 
// FRAME 07
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xec, 0x12, 0x52, 0x9c, 0xec, 0x12, 0x0d, 0xe2, 0x09, 0xf7, 
0x00, 0x00, 0x00, 0x00, 0x40, 0x80, 0x67, 0x14, 0x08, 0x1d, 0x6b, 0xc8, 0x10, 0x15, 0x18, 0x0f,
};

const uint8_t PROGMEM boss_image[] = {
0x70, 0x80, 0x00, 0x9e, 0x3b, 0xf0, 0x08, 0x08, 0x08, 0x08, 0xf0, 0x37, 0x9e, 0x00, 0x80, 0x70, 
0xf0, 0x89, 0xd7, 0x54, 0xa3, 0x12, 0x71, 0x89, 0xad, 0x71, 0x12, 0xa3, 0x54, 0xd7, 0x09, 0xf0, 
0x03, 0x04, 0x83, 0xc0, 0xff, 0xe4, 0x7e, 0x06, 0x06, 0x7e, 0xe4, 0xff, 0xc0, 0x81, 0x02, 0x03, 
};

const uint8_t *const PROGMEM enemy_lut[NUM_ENEMY_TYPES] = {
    skeleton_images,
    zombie_images,
    orc_images,
    warwolf_images,
    fallenone_images,
    gargoyle_images,
};

const uint8_t PROGMEM fireball_image[] = {
    0x08, 0x14, 0x12, 0x29, 0x2d, 0x2d, 0x12, 0x0c, 
};
const uint8_t PROGMEM fireball_flipped_image[] = {
    0x0c, 0x12, 0x2d, 0x2d, 0x29, 0x12, 0x14, 0x08, 
};
const uint8_t PROGMEM blip_image[] = {
    0x89, 0x4a, 0x24, 0xd8, 0x1b, 0x24, 0x52, 0x91, 
};

/*const uint8_t PROGMEM dead_image[] = {
16, 16,
0x00, 0x10, 0x24, 0x28, 0x44, 0x54, 0xc8, 0xc0, 0x34, 0x88, 0xb2, 0x84, 0xd2, 0x4c, 0x80, 0x00, 
0x00, 0x00, 0x19, 0x1e, 0x28, 0x28, 0x16, 0x13, 0x27, 0x1a, 0x2f, 0x0c, 0x15, 0x08, 0x0b, 0x00, 
};*/
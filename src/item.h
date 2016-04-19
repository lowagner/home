#ifndef _item_h_
#define _item_h_

#define EMPTY 0

// shapes:
// simple shapes
#define S_CUBE 1
#define S_HALF_PY 2 // Positive Y
#define S_HALF_NY 3 // Negative Y
// thin shapes
#define S_CENTER_X 4 // half a cube centered going along x
#define S_CENTER_Z 5
#define S_HALF_PX 6
#define S_HALF_PZ 7
#define S_HALF_NX 8
#define S_HALF_NZ 9
// slats
#define S_FENCEY_X 10 // slats going in y dir, along x
#define S_FENCEY_Z 11
#define S_FENCE_X 12 // slats going along x dir
#define S_FENCE_Z 13 // slats going along x dir
// can add a few here before the plants at 127:
#define S_PLANT 127
// don't add any other shapes after 127.


// materials:
#define M_T 0 // pun on EM-PTY!
#define M_GRASS 1
#define M_SAND 2
#define M_STONE 3
#define M_BRICK 4
#define M_WOOD 5
#define M_CEMENT 6
#define M_DIRT 7
#define M_PLANK 8
#define M_SNOW 9
#define M_GLASS 10
#define M_COBBLE 11
#define M_LIGHT_STONE 12
#define M_DARK_STONE 13
#define M_CHEST 14
#define M_LEAVES 15
#define M_CLOUD 16
#define M_PLANT_GRASS 17
#define M_YELLOW_FLOWER 18
#define M_RED_FLOWER 19
#define M_PURPLE_FLOWER 20
#define M_SUN_FLOWER 21
#define M_WHITE_FLOWER 22
#define M_BLUE_FLOWER 23
#define M_FLOWER_COUNT 6 

#define A_CLOUD 127

#include <stdint.h>

typedef union {
    int32_t value;
    struct {
        int8_t shape;
        uint8_t material;
        uint8_t color;
        int8_t action;
    };
} W;

extern const int shapes[];
extern const int shape_count;
extern const int block_textures[256][6];
extern const int plant_textures[256];

int is_plant(W w);
int is_obstacle(W w);
int is_transparent(W w);
int is_destructable(W w);

extern float color_palette[256][3];

#endif

#include "item.h"
#include "util.h"

const int shapes[] = {
    // items the user can build
    S_CUBE,
    S_HALF_PY, // Positive Y
    S_HALF_NY, // Negative Y
    S_CENTER_X,
    S_CENTER_Z,
    S_HALF_PX,
    S_HALF_PZ,
    S_HALF_NX,
    S_HALF_NZ,
};

const int shape_count = sizeof(shapes) / sizeof(int);

const int block_textures[256][6] = {
    // w => (left, right, top, bottom, front, back) tiles
    {0,0,0,0,0,0},
    {16, 16, 32, 0, 16, 16}, // 1 - grass
    {1, 1, 1, 1, 1, 1}, // 2 - sand
    {2, 2, 2, 2, 2, 2}, // 3 - stone
    {3, 3, 3, 3, 3, 3}, // 4 - brick
    {20, 20, 36, 4, 20, 20}, // 5 - wood
    {5, 5, 5, 5, 5, 5}, // 6 - cement
    {6, 6, 6, 6, 6, 6}, // 7 - dirt
    {7, 7, 7, 7, 7, 7}, // 8 - plank
    {24, 24, 40, 8, 24, 24}, // 9 - snow
    {9, 9, 9, 9, 9, 9}, // 10 - glass
    {10, 10, 10, 10, 10, 10}, // 11 - cobble
    {11, 11, 11, 11, 11, 11}, // 12 - light stone
    {12, 12, 12, 12, 12, 12}, // 13 - dark stone
    {13, 13, 13, 13, 13, 13}, // 14 - chest
    {14, 14, 14, 14, 14, 14}, // 15 - leaves
    {15, 15, 15, 15, 15, 15}, // 16 - cloud
    {48, 48, 48, 48, 48, 48}, // 17 - plants 
    {49, 49, 49, 49, 49, 49}, // 18 - yellow flower 
    {50, 50, 50, 50, 50, 50}, // 19 - red flower 
    {51, 51, 51, 51, 51, 51}, // 20 - purple flower
    {51, 51, 51, 51, 51, 51}, // 20 - purple flower
    [0xff]={}, // 255 - water
};

const int plant_textures[256] = {
    // w => tile
    32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0 - 16
    48, // 17 - tall grass
    49, // 18 - yellow flower
    50, // 19 - red flower
    51, // 20 - purple flower
    52, // 21 - sun flower
    53, // 22 - white flower
    54, // 23 - blue flower
};

float color_palette[256][3] = {
    {1.0, 1.0, 1.0},
    {1.0, 0, 0},
    {0, 1.0, 0},
    {0, 0, 1.0},
};

int is_plant(W w) {
    if (w.shape == S_PLANT || -w.shape == S_PLANT)
        return 1;
    return 0;
}

int is_obstacle(W w) {
    if (is_plant(w)) {
        return 0;
    }
    if (w.value == 0) // || (w.color == M_CLOUD)) // allow/do not allow walking on clouds
        return 0;
    return 1;
}

int is_transparent(W w) {
    if (w.shape == S_CUBE) {
        switch (w.material) {
            case M_GLASS:
            case M_LEAVES:
                return 1;
            default:
                return 0;
        }
    }
    // everything else needs to be treated as see through
    return 1;
}

int is_destructable(W w) {
    if (w.value == 0) // || (w.color == M_CLOUD)) 
        return 0;

    return 1;
}

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
    {255,255,255,255,255,255},
    [M_GRASS]={16, 16, 32, 0, 16, 16}, 
    [M_SAND]={80, 80, 80, 80, 80, 80},
    [M_STONE]={16+1, 16+2, 32+2, 2, 16+2, 16+1},
    [M_BRICK]={16+3, 16+3, 32+3, 3, 16+3, 16+3},
    [M_WOOD]={20, 20, 36, 4, 20, 20},
    [M_CEMENT]={82, 82, 82, 82, 82, 82}, 
    [M_DIRT]={83, 83, 83, 83, 83, 83},
    [M_PLANK]={7, 7, 7, 7, 7, 7}, 
    [M_SNOW]={24, 24, 40, 8, 24, 24}, 
    [M_GLASS]={84, 84, 84, 84, 84, 84}, 
    [M_COBBLE]={85, 85, 85, 85, 85, 85}, 
    [M_LIGHT_STONE]={86, 86, 86, 86, 86, 86}, 
    [M_DARK_STONE]={87, 87, 87, 87, 87, 87}, 
    [M_CHEST]={88, 88, 88, 88, 88, 88}, 
    [M_LEAVES]={81, 81, 81, 81, 81, 81}, 
    [M_CLOUD]={255, 255, 255, 255, 255, 255},
    [M_PLANT_GRASS]={244, 244, 244, 244, 244, 244}, 
    [M_YELLOW_FLOWER]={245, 245, 245, 245, 245, 245}, 
    [M_RED_FLOWER]={246, 246, 246, 246, 246, 246}, 
    [M_PURPLE_FLOWER]={247, 247, 247, 247, 247, 247},
    [M_SUN_FLOWER]={248, 248, 248, 248, 248, 248}, 
    [M_WHITE_FLOWER]={249, 249, 249, 249, 249, 249},
    [M_BLUE_FLOWER]={250, 250, 250, 250, 250, 250}, 
    [M_WATER]={14+16, 14+32, 14, 14, 14+16, 14+32}, 
};

const int plant_textures[256] = {
    // w => tile
    [M_PLANT_GRASS]=244,
    [M_YELLOW_FLOWER]=245,
    [M_RED_FLOWER]=246,
    [M_PURPLE_FLOWER]=247,
    [M_SUN_FLOWER]=248,
    [M_WHITE_FLOWER]=249,
    [M_BLUE_FLOWER]=250
};

float color_palette[256][3] = {
    {1.0, 1.0, 1.0},
    {1.0, 0, 0},
    {0, 1.0, 0},
    {0, 0, 1.0},
    [C_WATER]={0.99, 0.99, 1.0},
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
    if (w.value == 0 || w.material == M_WATER) // || (w.color == M_CLOUD)) // allow/do not allow walking on clouds
        return 0;
    return 1;
}

int is_transparent(W w) {
    int shape = w.shape < 0 ? -w.shape : w.shape;
    if (shape == S_CUBE) {
        switch (w.material) {
            case M_GLASS:
            case M_LEAVES:
            case M_CLOUD:
            case M_WATER:
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


int number_faces(W w, int f1, int f2, int f3, int f4, int f5, int f6) {
    // you can assume at least one of f1-6 are nonzero.
    int shape = w.shape < 0 ? -w.shape : w.shape;
    switch (shape) {
        case S_PLANT:
            return 4;
    }
    return f1+f2+f3+f4+f5+f6;
}

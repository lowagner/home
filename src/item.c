#include "item.h"
#include "util.h"

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
    [M_PLANK]={21, 21, 21, 21, 21, 21}, 
    [M_VERTICAL_PLANK]={37, 37, 37, 37, 37, 37}, 
    [M_SNOW]={22, 22, 38, 6, 22, 22}, 
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
    [M_SIDING]={23, 23, 23, 23, 23, 23}, 
    [M_VERTICAL_SIDING]={39, 39, 39, 39, 39, 39}, 
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

float hsv_palette[256][3] = {
    {0.000, 1.2, 1.0},
    {0.000, 0.9, 1.0},
    {0.000, 0.5, 1.0},
    {0.000, 1.2, 0.5},
    {0.000, 0.9, 0.5},
    {0.000, 0.5, 0.5},
    {0.050, 1.2, 1.0},
    {0.050, 0.9, 1.0},
    {0.050, 0.5, 1.0},
    {0.050, 1.2, 0.5},
    {0.050, 0.9, 0.5},
    {0.050, 0.5, 0.5},
    {0.100, 1.2, 1.0},
    {0.100, 0.9, 1.0},
    {0.100, 0.5, 1.0},
    {0.100, 1.2, 0.5},
    {0.100, 0.9, 0.5},
    {0.100, 0.5, 0.5},
    {0.150, 1.2, 1.0},
    {0.150, 0.9, 1.0},
    {0.150, 0.5, 1.0},
    {0.150, 1.2, 0.5},
    {0.150, 0.9, 0.5},
    {0.150, 0.5, 0.5},
    {0.200, 1.2, 1.0},
    {0.200, 0.9, 1.0},
    {0.200, 0.5, 1.0},
    {0.200, 1.2, 0.5},
    {0.200, 0.9, 0.5},
    {0.200, 0.5, 0.5},
    {0.250, 1.2, 1.0},
    {0.250, 0.9, 1.0},
    {0.250, 0.5, 1.0},
    {0.250, 1.2, 0.5},
    {0.250, 0.9, 0.5},
    {0.250, 0.5, 0.5},
    {0.300, 1.2, 1.0},
    {0.300, 0.9, 1.0},
    {0.300, 0.5, 1.0},
    {0.300, 1.2, 0.5},
    {0.300, 0.9, 0.5},
    {0.300, 0.5, 0.5},
    {0.350, 1.2, 1.0},
    {0.350, 0.9, 1.0},
    {0.350, 0.5, 1.0},
    {0.350, 1.2, 0.5},
    {0.350, 0.9, 0.5},
    {0.350, 0.5, 0.5},
    {0.400, 1.2, 1.0},
    {0.400, 0.9, 1.0},
    {0.400, 0.5, 1.0},
    {0.400, 1.2, 0.5},
    {0.400, 0.9, 0.5},
    {0.400, 0.5, 0.5},
    {0.450, 1.2, 1.0},
    {0.450, 0.9, 1.0},
    {0.450, 0.5, 1.0},
    {0.450, 1.2, 0.5},
    {0.450, 0.9, 0.5},
    {0.450, 0.5, 0.5},
    {0.500, 1.2, 1.0},
    {0.500, 0.9, 1.0},
    {0.500, 0.5, 1.0},
    {0.500, 1.2, 0.5},
    {0.500, 0.9, 0.5},
    {0.500, 0.5, 0.5},
    {0.550, 1.2, 1.0},
    {0.550, 0.9, 1.0},
    {0.550, 0.5, 1.0},
    {0.550, 1.2, 0.5},
    {0.550, 0.9, 0.5},
    {0.550, 0.5, 0.5},
    {0.600, 1.2, 1.0},
    {0.600, 0.9, 1.0},
    {0.600, 0.5, 1.0},
    {0.600, 1.2, 0.5},
    {0.600, 0.9, 0.5},
    {0.600, 0.5, 0.5},
    {0.650, 1.2, 1.0},
    {0.650, 0.9, 1.0},
    {0.650, 0.5, 1.0},
    {0.650, 1.2, 0.5},
    {0.650, 0.9, 0.5},
    {0.650, 0.5, 0.5},
    {0.700, 1.2, 1.0},
    {0.700, 0.9, 1.0},
    {0.700, 0.5, 1.0},
    {0.700, 1.2, 0.5},
    {0.700, 0.9, 0.5},
    {0.700, 0.5, 0.5},
    {0.750, 1.2, 1.0},
    {0.750, 0.9, 1.0},
    {0.750, 0.5, 1.0},
    {0.750, 1.2, 0.5},
    {0.750, 0.9, 0.5},
    {0.750, 0.5, 0.5},
    {0.800, 1.2, 1.0},
    {0.800, 0.9, 1.0},
    {0.800, 0.5, 1.0},
    {0.800, 1.2, 0.5},
    {0.800, 0.9, 0.5},
    {0.800, 0.5, 0.5},
    {0.850, 1.2, 1.0},
    {0.850, 0.9, 1.0},
    {0.850, 0.5, 1.0},
    {0.850, 1.2, 0.5},
    {0.850, 0.9, 0.5},
    {0.850, 0.5, 0.5},
    {0.900, 1.2, 1.0},
    {0.900, 0.9, 1.0},
    {0.900, 0.5, 1.0},
    {0.900, 1.2, 0.5},
    {0.900, 0.9, 0.5},
    {0.900, 0.5, 0.5},
    {0.950, 1.2, 1.0},
    {0.950, 0.9, 1.0},
    {0.950, 0.5, 1.0},
    {0.950, 1.2, 0.5},
    {0.950, 0.9, 0.5},
    {0.950, 0.5, 0.5},
    // set -1:  white-gray
    {0.0, 0.0, 0.0625},  // gray
    {0.0, 0.0, 0.125}, 
    {0.0, 0.0, 0.25}, 
    {0.0, 0.0, 0.5}, 
    {0.0, 0.0, 0.75}, 
    {0.0, 0.0, 1.0}, 
    {0.0, 0.0, 1.25}, 
    {0.0, 0.0, 1.5},  // 127 - white
    [C_WATER]=
    {0.000, 1.2, -1.0},
    {0.000, 0.9, -1.0},
    {0.000, 0.5, -1.0},
    {0.000, 1.2, -0.5},
    {0.000, 0.9, -0.5},
    {0.000, 0.5, -0.5},
    {0.050, 1.2, -1.0},
    {0.050, 0.9, -1.0},
    {0.050, 0.5, -1.0},
    {0.050, 1.2, -0.5},
    {0.050, 0.9, -0.5},
    {0.050, 0.5, -0.5},
    {0.100, 1.2, -1.0},
    {0.100, 0.9, -1.0},
    {0.100, 0.5, -1.0},
    {0.100, 1.2, -0.5},
    {0.100, 0.9, -0.5},
    {0.100, 0.5, -0.5},
    {0.150, 1.2, -1.0},
    {0.150, 0.9, -1.0},
    {0.150, 0.5, -1.0},
    {0.150, 1.2, -0.5},
    {0.150, 0.9, -0.5},
    {0.150, 0.5, -0.5},
    {0.200, 1.2, -1.0},
    {0.200, 0.9, -1.0},
    {0.200, 0.5, -1.0},
    {0.200, 1.2, -0.5},
    {0.200, 0.9, -0.5},
    {0.200, 0.5, -0.5},
    {0.250, 1.2, -1.0},
    {0.250, 0.9, -1.0},
    {0.250, 0.5, -1.0},
    {0.250, 1.2, -0.5},
    {0.250, 0.9, -0.5},
    {0.250, 0.5, -0.5},
    {0.300, 1.2, -1.0},
    {0.300, 0.9, -1.0},
    {0.300, 0.5, -1.0},
    {0.300, 1.2, -0.5},
    {0.300, 0.9, -0.5},
    {0.300, 0.5, -0.5},
    {0.350, 1.2, -1.0},
    {0.350, 0.9, -1.0},
    {0.350, 0.5, -1.0},
    {0.350, 1.2, -0.5},
    {0.350, 0.9, -0.5},
    {0.350, 0.5, -0.5},
    {0.400, 1.2, -1.0},
    {0.400, 0.9, -1.0},
    {0.400, 0.5, -1.0},
    {0.400, 1.2, -0.5},
    {0.400, 0.9, -0.5},
    {0.400, 0.5, -0.5},
    {0.450, 1.2, -1.0},
    {0.450, 0.9, -1.0},
    {0.450, 0.5, -1.0},
    {0.450, 1.2, -0.5},
    {0.450, 0.9, -0.5},
    {0.450, 0.5, -0.5},
    {0.500, 1.2, -1.0},
    {0.500, 0.9, -1.0},
    {0.500, 0.5, -1.0},
    {0.500, 1.2, -0.5},
    {0.500, 0.9, -0.5},
    {0.500, 0.5, -0.5},
    {0.550, 1.2, -1.0},
    {0.550, 0.9, -1.0},
    {0.550, 0.5, -1.0},
    {0.550, 1.2, -0.5},
    {0.550, 0.9, -0.5},
    {0.550, 0.5, -0.5},
    {0.600, 1.2, -1.0},
    {0.600, 0.9, -1.0},
    {0.600, 0.5, -1.0},
    {0.600, 1.2, -0.5},
    {0.600, 0.9, -0.5},
    {0.600, 0.5, -0.5},
    {0.650, 1.2, -1.0},
    {0.650, 0.9, -1.0},
    {0.650, 0.5, -1.0},
    {0.650, 1.2, -0.5},
    {0.650, 0.9, -0.5},
    {0.650, 0.5, -0.5},
    {0.700, 1.2, -1.0},
    {0.700, 0.9, -1.0},
    {0.700, 0.5, -1.0},
    {0.700, 1.2, -0.5},
    {0.700, 0.9, -0.5},
    {0.700, 0.5, -0.5},
    {0.750, 1.2, -1.0},
    {0.750, 0.9, -1.0},
    {0.750, 0.5, -1.0},
    {0.750, 1.2, -0.5},
    {0.750, 0.9, -0.5},
    {0.750, 0.5, -0.5},
    {0.800, 1.2, -1.0},
    {0.800, 0.9, -1.0},
    {0.800, 0.5, -1.0},
    {0.800, 1.2, -0.5},
    {0.800, 0.9, -0.5},
    {0.800, 0.5, -0.5},
    {0.850, 1.2, -1.0},
    {0.850, 0.9, -1.0},
    {0.850, 0.5, -1.0},
    {0.850, 1.2, -0.5},
    {0.850, 0.9, -0.5},
    {0.850, 0.5, -0.5},
    {0.900, 1.2, -1.0},
    {0.900, 0.9, -1.0},
    {0.900, 0.5, -1.0},
    {0.900, 1.2, -0.5},
    {0.900, 0.9, -0.5},
    {0.900, 0.5, -0.5},
    {0.950, 1.2, -1.0},
    {0.950, 0.9, -1.0},
    {0.950, 0.5, -1.0},
    {0.950, 1.2, -0.5},
    {0.950, 0.9, -0.5},
    {0.950, 0.5, -0.5},
    // set -1:  white-gray
    {0.0, 0.0, -0.0625},  // gray
    {0.0, 0.0, -0.125}, 
    {0.0, 0.0, -0.25}, 
    {0.0, 0.0, -0.5}, 
    {0.0, 0.0, -0.75}, 
    {0.0, 0.0, -1.0}, 
    {0.0, 0.0, -1.25}, 
    {0.0, 0.0, -1.5},  // 255 - white
};

int is_plant(W w) {
    if (w.shape == S_PLANT || -w.shape == S_PLANT)
        return 1;
    return 0;
}

int is_obstacle(W w, int dir) {
    if (w.value == 1 || is_plant(w)) // || w.material == M_WATER) // include WATER to make building on water impossible
        return 0;
    if (w.material == M_CLOUD) {
        if (dir & D_PY)
            return 1;
        return 0;
    }
    return 1;
}

int is_transparent(W w) {
    int shape = w.shape < 0 ? -w.shape : w.shape;
    if (shape == S_CUBE) {
        switch (w.material) {
            case M_GLASS:
            case M_LEAVES:
            //case M_CLOUD:
            //case M_WATER: // this really slows down calculations in spots
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


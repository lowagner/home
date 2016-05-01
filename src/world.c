#include "config.h"
#include "noise.h"
#include "world.h"
#include "item.h" // for converting W's into int's

void create_world1(int p, int q, world_func func, void *arg) {
    for (int dx = -CHUNK_PAD; dx < CHUNK_SIZE + CHUNK_PAD; dx++) {
        for (int dz = -CHUNK_PAD; dz < CHUNK_SIZE + CHUNK_PAD; dz++) {
            int flag = 1;
            if (dx < 0 || dz < 0 || dx >= CHUNK_SIZE || dz >= CHUNK_SIZE) {
                flag = -1;
            }
            int x = p * CHUNK_SIZE + dx;
            int z = q * CHUNK_SIZE + dz;
            float f = simplex2(x * 0.01, z * 0.01, 4, 0.5, 2);
            float g = simplex2(-x * 0.01, -z * 0.01, 2, 0.9, 2);
            int mh = g * 32 + 16;
            int h = f * mh;
            unsigned int material = M_GRASS;
            int t = 12;
            if (h <= t) {
                h = t;
                material = M_SAND;
            }
            int w = (W){.shape=flag*S_CUBE, .material=material, .color=0}.value;
            // sand and grass terrain
            for (int y = 0; y < h; y++) {
                func(x, y, z, w, arg);
            }
            if (material == M_GRASS) {
                if (SHOW_PLANTS) {
                    // grass
                    if (simplex2(-x * 0.1, z * 0.1, 4, 0.8, 2) > 0.6) {
                        func(x, h, z, (W){.shape=flag*S_PLANT, 
                                .material=M_PLANT_GRASS, .color=0, .action=0}.value,
                            arg);
                    }
                    // flowers
                    if (simplex2(x * 0.05, -z * 0.05, 4, 0.8, 2) > 0.7) {
                        func(x, h, z, (W){.shape=flag*S_PLANT, 
                            .material=(M_RED_FLOWER
                                + M_FLOWER_COUNT * simplex2(x * 0.1, z * 0.1, 4, 0.8, 2)), 
                            .color=0, .action=0}.value, arg);
                    }
                }
                // trees
                int ok = SHOW_TREES;
                if (dx - 4 < 0 || dz - 4 < 0 ||
                    dx + 4 >= CHUNK_SIZE || dz + 4 >= CHUNK_SIZE)
                {
                    ok = 0;
                }
                if (ok && simplex2(x, z, 6, 0.5, 2) > 0.84) {
                    for (int y = h + 3; y < h + 8; y++) {
                        for (int ox = -3; ox <= 3; ox++) {
                            for (int oz = -3; oz <= 3; oz++) {
                                int d = (ox * ox) + (oz * oz) +
                                    (y - (h + 4)) * (y - (h + 4));
                                if (d < 11) {
                                    func(x + ox, y, z + oz, 
                                        (W) {.shape=S_CUBE, .material=M_LEAVES, .color=0, .action=0}.value, 
                                        arg);
                                }
                            }
                        }
                    }
                    for (int y = h; y < h + 7; y++) {
                        func(x, y, z, (W) {.shape=S_CUBE, .material=M_WOOD, .color=0, .action=0}.value, arg);
                    }
                }
            }
            // clouds
            if (SHOW_CLOUDS) {
                for (int y = 64; y < 72; y++) {
                    if (simplex3(
                        x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.75)
                    {
                        func(x, y, z, 
                            (W) {.shape=S_CUBE*flag, .material=M_CLOUD, .color=0, .action=A_CLOUD}.value, arg);
                    }
                }
            }
        }
    }
}

void biome0(int x, int z, int flag, world_func func, void *arg) {
    int lo = simplex2(x * 0.01, z * 0.01, 4, 0.5, 2) * 2 + 10;
    int hi = simplex2(-x * 0.01, -z * 0.01, 4, 0.5, 2) * 2 + 12;
    for (int y = 0; y < lo; y++) {
        func(x, y, z, (W) {.shape=S_CUBE*flag, .material=M_DIRT, .color=0, .action=0}.value, arg);
    }
    for (int y = lo; y < hi; y++) {
        func(x, y, z, (W) {.shape=S_CUBE*flag, .material=M_GRASS, .color=0, .action=0}.value, arg);
    }
    float f = simplex2(-x * 0.1, z * 0.1, 4, 0.8, 2);
    if (f < 0.2) {
        uint8_t lookup[] = {M_BRICK, M_YELLOW_FLOWER, 
            M_RED_FLOWER, M_PURPLE_FLOWER, M_SUN_FLOWER, M_WHITE_FLOWER, M_BLUE_FLOWER};
        int i = simplex3(x * 0.5, hi * 0.5, z * 0.5, 3, 2.5, 5) * sizeof(lookup);
        if (i == 0)  {
            func(x, hi, z, (W) {.shape=S_CUBE*flag, .material=M_BRICK, .color=0, .action=0}.value, arg);
            i = 1+simplex3(-x * 0.001, -hi * 0.001, -z * 0.001, 5, 0.5, 2) * (sizeof(lookup)-1);
            func(x, hi+1, z, (W) {.shape=S_PLANT*flag, .material=lookup[i], .color=0, .action=0}.value, arg);
        } else
            func(x, hi, z, (W) {.shape=S_PLANT*flag, .material=lookup[i], .color=0, .action=0}.value, arg);
    } else if (f < 0.3) {
        func(x, hi, z, (W) {.shape=S_PLANT*flag, .material=M_PLANT_GRASS, .color=0, .action=0}.value, arg);
    }
    if (SHOW_CLOUDS) {
        for (int y = 60; y < 72; y++) {
            if (simplex3(
                x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.65)
            {
                func(x, y, z, 
                    (W) {.shape=S_CUBE*flag, .material=M_CLOUD, .color=0, .action=A_CLOUD}.value, arg);
            }
        }
    }
}

void biome1(int x, int z, int flag, world_func func, void *arg) {
    float f = simplex2(x * 0.01, z * 0.01, 4, 0.5, 2);
    float g = simplex2(-x * 0.01, -z * 0.01, 2, 0.9, 2);
    int mh = g * 32 + 16;
    int h = f * mh;
    unsigned int material = M_GRASS;
    int t = 12;
    if (h <= t) {
        h = t;
        material = M_SAND;
    }
    int w = (W) {.shape = flag*S_CUBE, .material=material, .color=0, .action=0}.value;
    // sand and grass terrain
    for (int y = 0; y < h; y++) {
        func(x, y, z, w, arg);
    }
    if (material == M_GRASS) {
        int ok = SHOW_TREES;
        if (SHOW_PLANTS) {
            // grass
            if (simplex2(-x * 0.1, z * 0.1, 4, 0.8, 2) > 0.6) {
                func(x, h, z, (W){.shape=flag*S_PLANT, .material=M_PLANT_GRASS, .color=0, .action=0}.value,
                    arg);
                ok = 0;
            }
            // flowers
            else if (simplex2(x * 0.05, -z * 0.05, 4, 0.8, 2) > 0.7) {
                func(x, h, z, 
                    (W){.shape=flag*S_PLANT, 
                        .material=(M_RED_FLOWER
                            + M_FLOWER_COUNT * simplex2(x * 0.1, z * 0.1, 4, 0.8, 2)), 
                        .color=0, .action=0}.value, arg);
                ok = 0;
            }
        }
        // trees
        if (ok && simplex2(x, z, 6, 1.5, 2) > 0.84) {
            for (int y = h + 3; y < h + 8; y++) {
                for (int ox = -3; ox <= 3; ox++) {
                    for (int oz = -3; oz <= 3; oz++) {
                        int d = (ox * ox) + (oz * oz) +
                            (y - (h + 4)) * (y - (h + 4));
                        if (d < 11) {
                            func(x + ox, y, z + oz, 
                                (W) {.shape=flag*S_CUBE, .material=M_LEAVES, .color=0, .action=0}.value, arg);
                        }
                    }
                }
            }
            for (int y = h; y < h + 7; y++) {
                func(x, y, z,
                    (W) {.shape=flag*S_CUBE, .material=M_WOOD, .color=0, .action=0}.value, arg);
            }
        }
    }
    // clouds
    if (SHOW_CLOUDS) {
        for (int y = 64; y < 72; y++) {
            if (simplex3(
                x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.75)
            {
                func(x, y, z, 
                    (W) {.shape=S_CUBE*flag, .material=M_CLOUD, .color=0, .action=A_CLOUD}.value, arg);
            }
        }
    }
}

void biome2(int x, int z, int flag, world_func func, void *arg) {
    int lo = simplex2(x * 0.01, z * 0.01, 4, 0.5, 2) * 8 + 8;
    int hi = simplex2(-x * 0.01, -z * 0.01, 4, 0.5, 2) * 32 + 32;
    int lookup[] = {
        (W) {.shape=S_CUBE*flag, .material=M_STONE, .color=103, .action=0}.value,
        (W) {.shape=S_CUBE*flag, .material=M_COBBLE, .color=108, .action=0}.value,
        (W) {.shape=S_CUBE*flag, .material=M_STONE, .color=105, .action=0}.value,
        (W) {.shape=S_CUBE*flag, .material=M_CEMENT, .color=103, .action=0}.value,
        (W) {.shape=S_CUBE*flag, .material=M_STONE, .color=107, .action=0}.value,
        (W) {.shape=S_CUBE*flag, .material=M_CEMENT, .color=107, .action=0}.value,
        (W) {.shape=S_CUBE*flag, .material=M_LIGHT_STONE, .color=107, .action=0}.value,
        (W) {.shape=S_CUBE*flag, .material=M_DARK_STONE, .color=107, .action=0}.value,
        (W) {.shape=S_CUBE*flag, .material=M_COBBLE, .color=103, .action=0}.value
    };
    for (int y = 0; y < lo; y++) {
        func(x, y, z, lookup[5], arg);
    }
    for (int y = lo; y < hi; y++) {
        if (simplex3(x * 0.01, y * 0.01, z * 0.01, 4, 0.5, 2) > 0.5) {
            int i = simplex3(-x * 0.01, -y * 0.01, -z * 0.01, 4, 0.5, 2) * 10;
            func(x, y, z, lookup[i%9], arg);
        } 
        else if (y < 11) {
            func(x, y, z, (W) {.shape=S_CUBE*flag, .material=M_WATER, .color=C_WATER, .action=A_WATER}.value, arg);
        } 
        else if (y == 11) {
            func(x, 11, z, (W) {.shape=S_HALF_NY*flag, .material=M_WATER, .color=C_WATER, .action=A_WATER}.value, arg);
        }
    }
    if (SHOW_CLOUDS) {
        for (int y = 64; y < 72; y++) {
            if (simplex3(
                x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.75)
            {
                func(x, y, z, (W){.shape=S_CUBE*flag, .material=M_CLOUD, .color=0, .action=0}.value, arg);
            }
        }
    }
}

void biome3(int x, int z, int flag, world_func func, void *arg) {
    int lo = simplex2(x * 0.01, z * 0.01, 4, 0.5, 2) * 20 + 10;
    int hi = simplex2(-x * 0.01, -z * 0.01, 4, 0.5, 2) * 40 + 28;
    for (int y = 0; y < lo; y++) {
        func(x, y, z, (W) {.shape=S_CUBE*flag, .material=M_STONE, .color=115, .action=0}.value, arg);
    }
    int i;
    int lookup[] = {
        (W) {.shape=S_CUBE*flag, .material=M_VERTICAL_PLANK, .color=103, .action=0}.value,
        (W) {.shape=S_CUBE*flag, .material=M_VERTICAL_PLANK, .color=108, .action=0}.value,
        (W) {.shape=S_CUBE*flag, .material=M_WOOD, .color=108, .action=0}.value,
        (W) {.shape=S_CUBE*flag, .material=M_WOOD, .color=105, .action=0}.value,
        (W) {.shape=S_CUBE*flag, .material=M_CHEST, .color=101, .action=0}.value,
        (W) {.shape=S_CUBE*flag, .material=M_SNOW, .color=108, .action=0}.value
    };
    for (int y = lo; y < hi; y++) {
        i = simplex3(-x * 0.01, -y * 0.01, -z * 0.01, 3, 0.5, 2) * 6;
        func(x, y, z, lookup[i], arg);
    }
    if (i < 3 && simplex3(-x*0.005, -hi*0.005, -z*0.005, 3, 0.5, 2) > 0.55) {
        // add some lichen if the top isn't snow
        int c = simplex3(-x*0.1, -hi*0.1, -z*0.1, 3, 0.5, 2) * 3; 
        func(x, hi, z, (W) {.shape=S_CUBE*flag, .material=M_LEAVES, .color=c, .action=0}.value, arg);
    }
}

void create_world2(int p, int q, world_func func, void *arg) {
    for (int dx = -CHUNK_PAD; dx < CHUNK_SIZE + CHUNK_PAD; dx++) {
        for (int dz = -CHUNK_PAD; dz < CHUNK_SIZE + CHUNK_PAD; dz++) {
            int flag = 1;
            if (dx < 0 || dz < 0 || dx >= CHUNK_SIZE || dz >= CHUNK_SIZE) {
                flag = -1;
            }
            int x = p * CHUNK_SIZE + dx;
            int z = q * CHUNK_SIZE + dz;
            float f = simplex2(-x * 0.001, -z * 0.001, 8, 0.15, 2);
            if (f < 0.25) biome0(x, z, flag, func, arg);
            else if (f < 0.45) biome1(x, z, flag, func, arg);
            else if (f < 0.7) biome2(x, z, flag, func, arg);
            else biome3(x, z, flag, func, arg);
        }
    }
}

void create_world(int p, int q, world_func func, void *arg) {
    create_world2(p, q, func, arg);
}

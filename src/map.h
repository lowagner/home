#ifndef _map_h_
#define _map_h_

#define EMPTY_ENTRY(entry) ((entry)->value == 0)

#define MAP_FOR_EACH(map, ex, ey, ez, ew) \
    for (unsigned int i = 0; i <= map->mask; i++) { \
        MapEntry *entry = map->data + i; \
        if (EMPTY_ENTRY(entry)) { \
            continue; \
        } \
        int ex = entry->e.x + map->dx; \
        int ey = entry->e.y + map->dy; \
        int ez = entry->e.z + map->dz; \
        W ew = entry->e.w;

#define END_MAP_FOR_EACH }

#include "item.h"

typedef union {
    int64_t value;
    struct {
        W w;
        uint8_t x;
        uint8_t y;
        uint8_t z;
        uint8_t unused;  // could use this for the light map and save half the memory for lights...
    } e;
} MapEntry;

typedef struct {
    int dx;
    int dy;
    int dz;
    unsigned int mask;
    unsigned int size;
    MapEntry *data;
} Map;

void map_alloc(Map *map, int dx, int dy, int dz, int mask);
void map_free(Map *map);
void map_copy(Map *dst, Map *src);
void map_grow(Map *map);
int map_set(Map *map, int x, int y, int z, W w);
W map_get(Map *map, int x, int y, int z);

#endif

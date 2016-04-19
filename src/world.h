#ifndef _world_h_
#define _world_h_

#include "item.h"

typedef void (*world_func)(int, int, int, W, void *);

void create_world(int p, int q, world_func func, void *arg);

#endif

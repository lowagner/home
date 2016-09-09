#include "chunks.h"
#include "item.h"
#include <math.h>
#include <stdio.h>

#define COLLISION_PAD 0.25f

void get_sight_vector(float rx, float ry, float *vx, float *vy, float *vz);

int foot_collide(W w, float fx, float fy, float fz, float cx, float cy, float cz, float *x, float *y, float *z)
{
    // push up if hitting something like a step, and/or push out.
    if (w.value == 0 || is_plant(w) || w.material == M_WATER)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    switch (shape) {
        case S_HALF_NY:
            if (fy < COLLISION_PAD) {
                *y = cy + COLLISION_PAD;
                return 2;
            }
            return 0;
        case S_CUBE:
        case S_HALF_PY:
            if (fy > -COLLISION_PAD) {
                *y = cy + COLLISION_PAD + 0.5;
                return 2;
            }
            return 0;
        case S_HALF_PX:
            if (fx > -COLLISION_PAD) {
                *x = cx - COLLISION_PAD;
                return 1;
            }
            return 0;
        case S_HALF_NX:
            if (fx < COLLISION_PAD) {
                *x = cx + COLLISION_PAD;
                return 1;
            }
            return 0;
        case S_HALF_PZ:
            if (fz > -COLLISION_PAD) {
                *z = cz - COLLISION_PAD;
                return 4;
            }
            return 0;
        case S_HALF_NZ:
            if (fz < COLLISION_PAD) {
                *z = cz + COLLISION_PAD;
                return 4;
            }
            return 0;
    }
    return 0;
}

int foot_collide_px(W w, float fx, float fy, float fz, float cx, float *x)
{
    if (w.value == 0 || is_plant(w) || w.material == M_WATER || w.material == M_CLOUD)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    const float x0 = cx - 1 + COLLISION_PAD + 0.5;
    switch (shape) {
        case S_CUBE:
        case S_HALF_PY:
        case S_HALF_PX:
            *x = x0;
            return 1;
        case S_HALF_PZ:
            if (fz > -COLLISION_PAD) {
                *x = x0;
                return 1;
            }
            return 0;
        case S_HALF_NZ:
            if (fz < COLLISION_PAD) {
                *x = x0;
                return 1;
            }
            return 0;
    }
    return 0;
}

int foot_collide_nx(W w, float fx, float fy, float fz, float cx, float *x)
{
    if (w.value == 0 || is_plant(w) || w.material == M_WATER || w.material == M_CLOUD)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    const float x0 = cx + 1 - COLLISION_PAD - 0.5;
    switch (shape) {
        case S_CUBE:
        case S_HALF_PY:
        case S_HALF_NX:
            *x = x0;
            return 1;
        case S_HALF_PZ:
            if (fz > -COLLISION_PAD) {
                *x = x0;
                return 1;
            }
            return 0;
        case S_HALF_NZ:
            if (fz < COLLISION_PAD) {
                *x = x0; 
                return 1;
            }
            return 0;
    }
    return 0;
}

int foot_collide_py(W w, float fx, float fy, float fz, float cy, float *y)
{
    // foot colliding downwards on the +y plane of block w below it.
    if (w.value == 0 || is_plant(w) || w.material == M_WATER)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    const float y0 =  cy - 1 + COLLISION_PAD + 0.5;
    switch (shape) {
        case S_CUBE:
        case S_HALF_PY:
            *y = y0;
            return 2;
        case S_HALF_PX:
            if (fx > -COLLISION_PAD) {
                *y = y0; 
                return 2;
            }
            return 0;
        case S_HALF_NX:
            if (fx < COLLISION_PAD) {
                *y = y0; 
                return 2;
            }
            return 0;
        case S_HALF_PZ:
            if (fz > -COLLISION_PAD) {
                *y = y0; 
                return 2;
            }
            return 0;
        case S_HALF_NZ:
            if (fz < COLLISION_PAD) {
                *y = y0; 
                return 2;
            }
            return 0;
    }
    return 0;
}

/*
// commented out because we probably shouldn't have a foot collide up (to a -y face)
int foot_collide_ny(W w, float fx, float fy, float fz, float cy, float *y)
{
    if (w.value == 0 || is_plant(w) || w.material == M_WATER || w.material == M_CLOUD)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    const float y0 = cy + 1 - COLLISION_PAD - 0.5;
    switch (shape) {
        case S_CUBE:
        case S_HALF_NY:
            *y = y0;
            return 2;
        case S_HALF_PX:
            if (fx > -COLLISION_PAD) {
                *y = y0;
                return 2;
            }
            return 0;
        case S_HALF_PZ:
            if (fz > -COLLISION_PAD) {
                *y = y0;
                return 2;
            }
            return 0;
        case S_HALF_NZ:
            if (fz < COLLISION_PAD) {
                *y = y0;
                return 2;
            }
            return 0;
    }
    return 0;
}
*/

int foot_collide_pz(W w, float fx, float fy, float fz, float cz, float *z)
{
    if (w.value == 0 || is_plant(w) || w.material == M_WATER || w.material == M_CLOUD)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    const float z0 = cz - 1 + COLLISION_PAD + 0.5;
    switch (shape) {
        case S_CUBE:
        case S_HALF_PY:
        case S_HALF_PZ:
            *z = z0;
            return 4;
        case S_HALF_PX:
            if (fx > -COLLISION_PAD) {
                *z = z0;
                return 4;
            }
            return 0;
        case S_HALF_NX:
            if (fx < COLLISION_PAD) {
                *z = z0;
                return 4;
            }
            return 0;
    }
    return 0;
}

int foot_collide_nz(W w, float fx, float fy, float fz, float cz, float *z)
{
    if (w.value == 0 || is_plant(w) || w.material == M_WATER || w.material == M_CLOUD)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    const float z0 = cz + 1 - COLLISION_PAD - 0.5;
    switch (shape) {
        case S_CUBE:
        case S_HALF_PY:
        case S_HALF_NZ:
            *z = z0;
            return 4;
        case S_HALF_PX:
            if (fx > -COLLISION_PAD) {
                *z = z0;
                return 4;
            }
            return 0;
        case S_HALF_NX:
            if (fx < COLLISION_PAD) {
                *z = z0;
                return 4;
            }
            return 0;
    }
    return 0;
}

int body_collide(W w, float fx, float fy, float fz, float cx, float cy, float cz, float *x, float *y, float *z)
{
    // push down if colliding into something
    if (w.value == 0 || is_plant(w) || w.material == M_WATER || w.material == M_CLOUD)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    switch (shape) {
        case S_CUBE:
        case S_HALF_NY:
            *y = cy - COLLISION_PAD - 0.5;
            return 2;
        case S_HALF_PY:
            if (fy > -COLLISION_PAD) {
                *y = cy - COLLISION_PAD;
                return 2;
            }
            return 0;
        case S_HALF_PX:
            if (fx > -COLLISION_PAD) {
                *x = cx - COLLISION_PAD;
                return 1;
            }
            return 0;
        case S_HALF_NX:
            if (fx < COLLISION_PAD) {
                *x = cx + COLLISION_PAD;
                return 1;
            }
            return 0;
        case S_HALF_PZ:
            if (fz > -COLLISION_PAD) {
                *z = cz - COLLISION_PAD;
                return 4;
            }
            return 0;
        case S_HALF_NZ:
            if (fz < COLLISION_PAD) {
                *z = cz + COLLISION_PAD;
                return 4;
            }
            return 0;
    }
    return 0;
}

int collide_px(W w, float fx, float fy, float fz, float cx, float *x)
{
    if (w.value == 0 || is_plant(w) || w.material == M_WATER || w.material == M_CLOUD)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    const float x0 = cx - 1 + COLLISION_PAD + 0.5;
    switch (shape) {
        case S_CUBE:
        case S_HALF_PY:
        case S_HALF_NY:
        case S_HALF_PX:
            *x = x0;
            return 1;
        case S_HALF_PZ:
            if (fz > -COLLISION_PAD) {
                *x = x0;
                return 1;
            }
            return 0;
        case S_HALF_NZ:
            if (fz < COLLISION_PAD) {
                *x = x0;
                return 1;
            }
            return 0;
    }
    return 0;
}

int collide_nx(W w, float fx, float fy, float fz, float cx, float *x)
{
    if (w.value == 0 || is_plant(w) || w.material == M_WATER || w.material == M_CLOUD)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    const float x0 = cx + 1 - COLLISION_PAD - 0.5;
    switch (shape) {
        case S_CUBE:
        case S_HALF_PY:
        case S_HALF_NY:
            *x = x0;
            return 1;
        case S_HALF_PZ:
            if (fz > -COLLISION_PAD) {
                *x = x0;
                return 1;
            }
            return 0;
        case S_HALF_NZ:
            if (fz < COLLISION_PAD) {
                *x = x0;
                return 1;
            }
            return 0;
    }
    return 0;
}

/*
// commented out because we probably shouldn't have a body part collide down (to a +y face)
// feet will do that...
int collide_py(W w, float fx, float fy, float fz, float cy, float *y)
{
    if (w.value == 0 || is_plant(w) || w.material == M_WATER)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    switch (shape) {
        case S_CUBE:
        case S_HALF_PY:
            *y = cy - 1 - COLLISION_PAD - 0.5;
            return 2;
        case S_HALF_PX:
            if (fx > -COLLISION_PAD) {
                *y = cy - 1 - COLLISION_PAD - 0.5;
                return 2;
            }
            return 0;
    }
    return 0;
}
*/

int collide_ny(W w, float fx, float fy, float fz, float cy, float *y)
{
    // head hitting something above it, on its -y face
    if (w.value == 0 || is_plant(w) || w.material == M_WATER || w.material == M_CLOUD)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    const float y0 = cy + 1 - COLLISION_PAD - 0.5;
    switch (shape) {
        case S_CUBE:
        case S_HALF_NY:
            *y = y0; // push head down below this point
            return 2;
        case S_HALF_PY:
            return 0;
        case S_HALF_PX:
            if (fx > -COLLISION_PAD) {
                *y = y0;
                return 2;
            }
            return 0;
        case S_HALF_NX:
            if (fx < COLLISION_PAD) {
                *y = y0;
                return 2;
            }
            return 0;
        case S_HALF_PZ:
            if (fz > -COLLISION_PAD) {
                *y = y0;
                return 2;
            }
            return 0;
        case S_HALF_NZ:
            if (fz < COLLISION_PAD) {
                *y = y0;
                return 2;
            }
            return 0;
    }
    return 0;
}

int collide_pz(W w, float fx, float fy, float fz, float cz, float *z)
{
    if (w.value == 0 || is_plant(w) || w.material == M_WATER || w.material == M_CLOUD)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    const float z0 = cz - 1 + COLLISION_PAD + 0.5;
    switch (shape) {
        case S_CUBE:
        case S_HALF_PY:
        case S_HALF_NY:
        case S_HALF_PZ:
            *z = z0;
            return 4;
        case S_HALF_PX:
            if (fx > -COLLISION_PAD) {
                *z = z0;
                return 4;
            }
            return 0;
        case S_HALF_NX:
            if (fx < COLLISION_PAD) {
                *z = z0;
                return 4;
            }
            return 0;
    }
    return 0;
}

int collide_nz(W w, float fx, float fy, float fz, float cz, float *z)
{
    if (w.value == 0 || is_plant(w) || w.material == M_WATER || w.material == M_CLOUD)
        return 0;
    int shape = w.shape > 0 ? w.shape : -w.shape;
    const float z0 = cz + 1 - COLLISION_PAD - 0.5;
    switch (shape) {
        case S_CUBE:
        case S_HALF_PY:
        case S_HALF_NY:
            *z = z0;
            return 4;
        case S_HALF_PX:
            if (fx > -COLLISION_PAD) {
                *z = z0;
                return 4;
            }
            return 0;
        case S_HALF_NX:
            if (fx < COLLISION_PAD) {
                *z = z0;
                return 4;
            }
            return 0;
    }
    return 0;
}

int collide(int height, float *x, float *y, float *z) {
    int p = chunked(*x);
    int q = chunked(*z);
    Chunk *chunk = find_chunk(p, q);
    if (!chunk) {
        return 0;
    }
    int result = 0;
    Map *map = &chunk->map;
    int cx = roundf(*x); // chunk x,y,z
    int cy = roundf(*y);
    int cz = roundf(*z);
    float fx = *x - cx; // fractional part of x, y, z
    float fy = *y - cy;
    float fz = *z - cz;
    int dy = 0;
    int head_hit = 0;
    if (fy > COLLISION_PAD) {
        head_hit |= collide_ny((W){.value=map_get(map, cx, cy + 1, cz)}, fx, fy, fz, cy, y);
        result |= head_hit;
    }
    for (; dy < height-1; dy++) {
        if (dy == 0) {
            head_hit |= body_collide((W){.value=map_get(map, cx, cy, cz)}, fx, fy, fz, cx, cy, cz, x, y, z);
            result |= head_hit;
        }
        else {
            result |= body_collide((W){.value=map_get(map, cx, cy - dy, cz)}, fx, fy, fz, cx, cy, cz, x, y, z);
        }

        if (fx < -COLLISION_PAD)
            result |= collide_px((W){.value=map_get(map, cx - 1, cy - dy, cz)}, fx, fy, fz, cx, x); 
        else if (fx > COLLISION_PAD) 
            result |= collide_nx((W){.value=map_get(map, cx + 1, cy - dy, cz)}, fx, fy, fz, cx, x);
        // TODO:  do we need to check above and below if we traverse from head (above) to feet (below)?
        //if (fy < -COLLISION_PAD)
        //    result |= collide_py((W){.value=map_get(map, cx, cy - dy - 1, cz)}, fx, fy, fz, cy, y);
        //else if (fy > COLLISION_PAD)
        //    result |= collide_ny((W){.value=map_get(map, cx, cy - dy + 1, cz)}, fx, fy, fz, cy, y);
        if (fz < -COLLISION_PAD)
            result |= collide_pz((W){.value=map_get(map, cx, cy - dy, cz - 1)}, fx, fy, fz, cz, z);
        else if (fz > COLLISION_PAD)
            result |= collide_nz((W){.value=map_get(map, cx, cy - dy, cz + 1)}, fx, fy, fz, cz, z);
    }
    /*
    TODO:
    when walking up a slope into a ceiling, your head can poke through.  ouch!
    make sure this "return 8;" thing works.
    */
    if (fy < COLLISION_PAD-0.01) { 
        // don't collide feet with nearby objects unless we're far enough down;
        // this allows us to step over some objects without jumping.
        if (fx < -COLLISION_PAD)
            result |= foot_collide_px((W){.value=map_get(map, cx - 1, cy - dy, cz)}, fx, fy, fz, cx, x);
        else if (fx > COLLISION_PAD)
            result |= foot_collide_nx((W){.value=map_get(map, cx + 1, cy - dy, cz)}, fx, fy, fz, cx, x);
        if (fy < -COLLISION_PAD)
            result |= foot_collide_py((W){.value=map_get(map, cx, cy - dy - 1, cz)}, fx, fy, fz, cy, y);
        //else if (fy > COLLISION_PAD)
        //    result |= foot_collide_ny((W){.value=map_get(map, cx, cy - dy + 1, cz)}, fx, fy, fz, cy, y);
        if (fz < -COLLISION_PAD)
            result |= foot_collide_pz((W){.value=map_get(map, cx, cy - dy, cz - 1)}, fx, fy, fz, cz, z);
        else if (fz > COLLISION_PAD)
            result |= foot_collide_nz((W){.value=map_get(map, cx, cy - dy, cz + 1)}, fx, fy, fz, cz, z);
    }
    int foot_hit = foot_collide((W){.value=map_get(map, cx, cy - dy, cz)}, fx, fy, fz, cx, cy, cz, x, y, z);
    if ((head_hit & 2) && (foot_hit & 2)) {
        // this looks bad, may have hit our head and foot somewhere
        printf("ouch\n");
        return 8;
    }
    result |= foot_hit;
    return result;
}


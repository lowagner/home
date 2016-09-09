#include "chunks.h"
#include <math.h>
#include "util.h"

int chunked(float x) {
    return floorf(roundf(x) / CHUNK_SIZE);
}

int chunk_distance(Chunk *chunk, int p, int q) {
    int dp = ABS(chunk->p - p);
    int dq = ABS(chunk->q - q);
    return MAX(dp, dq);
}


#ifndef _chunks_h_
#define _chunks_h_

#include <GL/glew.h>
#include "map.h"
#include "sign.h"
#include "config.h"

typedef struct {
    Map map;
    Map lights;
    SignList signs;
    int p;
    int q;
    int faces;
    int sign_faces;
    int dirty;
    int miny;
    int maxy;
    GLuint buffer;
    GLuint sign_buffer;
} Chunk;

typedef struct {
    float x;
    float y;
    float z;
    float rx;
    float ry;
    float t;
} State;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    State state;
    State state1;
    State state2;
    GLuint buffer;
} Player;

Chunk *find_chunk(int p, int q);
int chunked(float x);
int chunk_distance(Chunk *chunk, int p, int q);

#endif

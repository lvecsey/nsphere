#ifndef GEN_VERTEXARRAY_H
#define GEN_VERTEXARRAY_H

#include "collection_pack.h"
#include <GL/gl.h>

int gen_vertexarray(struct collection_pack *i, GLfloat *vertices, int num_vertices, int *filled_vertices);

#endif

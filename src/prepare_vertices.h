#ifndef PREPARE_VERTICES_H
#define PREPARE_VERTICES_H

#include <GL/gl.h>

#include "transform_pack.h"

#include "read_status.h"

int prepare_vertices(char *filename, struct transform_pack *t, int num_vertices, GLfloat **vertices, int *filled_vertices, read_status_t read_status);

#endif

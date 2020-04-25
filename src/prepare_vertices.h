#ifndef PREPARE_VERTICES_H
#define PREPARE_VERTICES_H

#include <GL/gl.h>

#include "transform_pack.h"

#include "read_status.h"

int prepare_vertices(char *filename, transform_pack *tp, long int num_vertices, GLfloat **vertices, long int *filled_vertices, read_statusfunc read_status);

#endif

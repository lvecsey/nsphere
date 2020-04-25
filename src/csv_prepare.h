#ifndef CSV_PREPARE_H
#define CSV_PREPARE_H

#include <GL/gl.h>

#include "transform_pack.h"

#include "read_status.h"

int csv_prepare(char *filename, transform_pack *tp, long int num_vertices, GLfloat **vertices, long int *filled_vertices, read_statusfunc read_status);

#endif

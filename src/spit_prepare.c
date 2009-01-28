#include <stdio.h>
#include <assert.h>

#include <GL/gl.h>

#include "read_status.h"

#include "prepare_vertices.h"

#include "transform_pack.h"

int spit_prepare(char *filename, struct transform_pack *t, int num_vertices, GLfloat **vertices, int *filled_vertices, read_status_t read_status) {

  GLfloat *v;

  int retval;

  int count;

  retval = prepare_vertices(filename, t, num_vertices, vertices, filled_vertices, read_status);
  if (retval==-1) {
    fprintf(stderr, "%s: Trouble with collective calls to prepare vertices.\n", __FUNCTION__);
    return -1;    
  }

  for (v = *vertices, count = 0; count < *filled_vertices; count++, v+=3) {

    printf("[%d]: %f,%f,%f\n", count, v[0], v[1], v[2]);

  }

  return 0;

}


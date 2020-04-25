#include <stdio.h>
#include <assert.h>

#include <GL/gl.h>

#include "read_status.h"

#include "prepare_vertices.h"

#include "transform_pack.h"

int csv_prepare(char *filename, transform_pack *tp, long int num_vertices, GLfloat **vertices, long int *filled_vertices, read_statusfunc read_status) {
  
  GLfloat *v;

  int retval;

  int count;

  retval = prepare_vertices(filename, tp, num_vertices, vertices, filled_vertices, read_status);
  if (retval==-1) {
    fprintf(stderr, "%s: Trouble with collective calls to prepare vertices.\n", __FUNCTION__);
    return -1;    
  }

  printf("X, Y, Z\n");
  
  for (v = *vertices, count = 0; count < *filled_vertices; count++, v+=3) {

    printf("%f, %f, %f\n", count, v[0], v[1], v[2]);

  }

  return 0;

}



#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <GL/gl.h>

#include "collection_pack.h"

#include "polar_collect.h"

#include "transform_pack.h"

#include "read_status.h"

#include "plot_file.h"

#include "gen_vertexarray.h"

int prepare_vertices(char *filename, transform_pack *tp, long int num_vertices, GLfloat **vertices, long int *filled_vertices, read_statusfunc read_status) {

  GLfloat *v;

  int retval;

  assert(filename!=NULL && tp!=NULL && num_vertices>0 && vertices!=NULL);

  cp.set = malloc(sizeof(cartesian) * num_vertices);
  if (cp.set == NULL) {
    fprintf(stderr, "%s: Trouble with malloc for cartesian vertices.\n", __FUNCTION__);
    return -1;
  }

  cp.pointer = cp.set;
  cp.end = cp.set + num_vertices;

  retval = plot_file(filename, tp, polar_collect, read_status);
  if (retval == -1) {
    fprintf(stderr, "%s: Trouble with plot_file to polar_collect.\n", __FUNCTION__);
    return -1;
  }

  v = malloc(sizeof(GLfloat) * num_vertices * 3);
  if (v==NULL) {
    fprintf(stderr, "%s: Trouble allocating memory for num_vertices=%d\n", __FUNCTION__, num_vertices);
    return -1;
  }

  retval = gen_vertexarray(&cp, v, num_vertices, filled_vertices);
  if (retval==-1) {    
    fprintf(stderr, "%s: Trouble filling glVexterArray memory area.\n", __FUNCTION__);
    return -1;
  }

  if (vertices != NULL) {
    *vertices = v;
  }

  return 0;

}

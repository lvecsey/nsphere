#include <stdio.h>
#include <assert.h>

#include <GL/gl.h>

#include "cartesian.h"

#include "collection_pack.h"

void show_cartesian(cartesian *c) {

  printf("%s: %f %f %f\n", __FUNCTION__, c->x, c->y, c->z);

}

int gen_vertexarray(collection_pack *i, GLfloat *vertices, long int num_vertices, long int *filled_vertices) {

  cartesian *c;
  
  GLfloat *gend = vertices + 3 * num_vertices;

  assert(i!=NULL && vertices != NULL && num_vertices > 0);

  assert(vertices < gend);
  
  for (c = i->set; c < i->end && vertices + 3 <= gend; c++) {

    *vertices++ = c->x;
    *vertices++ = c->y;
    *vertices++ = c->z;

  }

  if (filled_vertices!=NULL) {
    *filled_vertices = (c - i->set);
  }

  return 0;

}

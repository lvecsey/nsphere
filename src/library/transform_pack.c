#include <stdio.h>
#include <assert.h>

#include "transform_pack.h"

void init_transform_pack(struct transform_pack *t) {

  assert(t!=NULL);

  t->samp.filled_bytes = 0;

  t->mode = 0;
  t->lag = 1;
  t->sr = 0;
  t->skip = 0;
  t->jcount = 0;

  t->x = t->X;
  t->xend = t->X + (sizeof(t->X) / sizeof(double));

}


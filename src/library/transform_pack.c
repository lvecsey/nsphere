#include <stdio.h>

#include "transform_pack.h"

void init_transform_pack(transform_pack *tp) {

  tp->samp.filled_bytes = 0;

  tp->mode = 0;
  tp->lag = 1;
  tp->sr = 0;
  tp->skip = 0;
  tp->jcount = 0;
  tp->icount = 0;

  tp->n = 0;

  tp->state = 0;

}


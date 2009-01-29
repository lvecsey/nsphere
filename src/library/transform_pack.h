#ifndef TRANSFORM_PACK_H
#define TRANSFORM_PACK_H

#include <assert.h>

#include "sample_fill.h"

#define HAVE_PREFILL 0x1

struct transform_pack {

  double X[3];
  double srange;
  int n;

  struct sample_fill samp;

  int mode;

  int sr, skip;

  int lag;

  int jcount;
  int icount;

  unsigned long state;

};

void init_transform_pack(struct transform_pack *t);

#endif


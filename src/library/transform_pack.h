#ifndef TRANSFORM_PACK_H
#define TRANSFORM_PACK_H

#include <assert.h>

#include <stdint.h>

#include "sample_fill.h"

#define HAVE_PREFILL 0x1

typedef struct {

  double X[3];
  double srange;
  int n;

  sample_fill samp;

  int mode;

  int sr, skip;

  int lag;

  int jcount;
  int icount;

  uint64_t state;

} transform_pack;

void init_transform_pack(transform_pack *tp);

#endif


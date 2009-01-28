#ifndef TRANSFORM_PACK_H
#define TRANSFORM_PACK_H

#include <assert.h>

#include "sample_fill.h"

struct transform_pack {

  double X[3];
  double srange;
  double *x, *xend;

  struct sample_fill samp;

  int mode;

  int sr, skip;

  int lag;

  int jcount;

};

void init_transform_pack(struct transform_pack *t);

#define transform_assert1(t) ( (t)->x >= t->X )
#define transform_assert2(t) ( (t)->x <= (t)->X + (sizeof((t)->X) / sizeof(double)) )
#define transform_assert3(t) ( (t)->xend == (t)->X + (sizeof((t)->X) / sizeof(double)) )

#define assert_transform_pack( t ) assert( transform_assert1(t) && transform_assert2(t) && transform_assert3(t) )

#endif


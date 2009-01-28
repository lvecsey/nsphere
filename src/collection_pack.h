#ifndef COLLECTION_PACK_H
#define COLLECTION_PACK_H

#include <stdio.h>
#include <assert.h>

#include "cartesian.h"

struct collection_pack {

  struct cartesian *set, *pointer, *end;
  
};

extern struct collection_pack cp;

#endif


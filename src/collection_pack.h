#ifndef COLLECTION_PACK_H
#define COLLECTION_PACK_H

#include <stdio.h>
#include <assert.h>

#include "cartesian.h"

typedef struct {

  cartesian *set, *pointer, *end;
  
} collection_pack;

extern collection_pack cp;

#endif


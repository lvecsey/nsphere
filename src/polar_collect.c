#include <stdio.h>
#include <assert.h>

#include "polar.h"

#include "collection_pack.h"

#include "polar_collect.h"

int polar_collect(struct polar *p) {

  assert(p!=NULL);

  assert(cp.pointer != NULL && cp.end != NULL && cp.pointer >= cp.set && cp.pointer <= cp.end);

  if (cp.pointer < cp.end) {

    polar_to_cartesian(p, cp.pointer);

    cp.pointer++;

  }

  return 0;

}

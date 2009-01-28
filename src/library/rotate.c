#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "cartesian.h"

#include "rotation_pack.h"

#include "rotate.h"

#include "rads.h"

/* Rotate C along X, Y and Z axes */
void rotate(struct rotation_pack *rp, struct cartesian *c) {
  
  double x,y,z;

  assert(rp!=NULL);

  if (rp->RZ) {
    z = rads(rp->RZ);
    x = (c->x * cos(z)) + (c->y * sin(z));
    y = (c->y * cos(z)) - (c->x * sin(z));
    c->x = x; c->y = y;
  }

  if (rp->RX) {
    x = rads(rp->RX);
    y = (c->y * cos(x)) + (c->z * sin(x));
    z = (c->z * cos(x)) - (c->y * sin(x));
    c->z = z; c->y = y;
  }

  if (rp->RY) {
    y = rads(rp->RY);
    x = (c->x * cos(y)) + (c->z * sin(y));
    z = (c->z * cos(y)) - (c->x * sin(y));
    c->z = z; c->x = x;
  }

}

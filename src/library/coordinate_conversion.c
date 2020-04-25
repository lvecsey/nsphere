#include <stdio.h>
#include <assert.h>

#include <math.h>

#include "polar.h"
#include "cartesian.h"

/* Converts 3-d polar coordinates to cartesian coordinates */
void polar_to_cartesian(polar *p, cartesian *c) {

  c->x = p->r * sin(p->phi) * cos(p->theta);
  c->y = p->r * sin(p->phi) * sin(p->theta);
  c->z = p->r * cos(p->phi);

/* We can assign colors based on C.x, C.y, C.z,
   or P.r, P.theta / pi or P.phi / (2 * pi) */
//  c->color = ScaleColor(c->z);

}

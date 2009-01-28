#ifndef NSPHERE_H
#define NSPHERE_H

#include <math.h>
#include <GL/gl.h>

#include "rotation_pack.h"

#include "transform_pack.h"

struct nsphere {

  GLfloat default_color[4];

  int triple;

  struct rotation_pack rp;

  int MidA, MidB, MidC, MidY, Scale;

  struct transform_pack t;

};

#define project(x) (ceil(Scale * x))

#endif

#ifndef NSPHERE_H
#define NSPHERE_H

#include <math.h>
#include <GL/gl.h>

#include "rotation_pack.h"

#include "transform_pack.h"

typedef struct {

  GLfloat default_color[4];

  int triple;

  rotation_pack rp;

  int MidA, MidB, MidC, MidY, Scale;

  transform_pack tp;

} nsphere;

#define project(x) (ceil(Scale * x))

#endif

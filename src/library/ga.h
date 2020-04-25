#ifndef GA_H
#define GA_H

typedef struct {

  double b01;
  double b02;
  double b12;
  
} bivec3;

typedef struct {

  double a;

  bivec3 bv;
  
} rot3;

#include <vector.h>

void normalize(rot3 *rot);

rot3 create_rotor(vec3d vFrom, vec3d vTo);

rot3 create_rotorplane(double angleRadian, bivec3 *bv);

int rotate(rot3 *rot, vec3d x, vec3d outvec);

int toMatrix3(rot3 *rot, double *matrix);

rot3 geo(vec3d a, vec3d b);

#endif

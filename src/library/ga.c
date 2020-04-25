
#include <vector.h>

#include <math.h>

#include "dot.h"

#include "ga.h"

bivec3 wedge(vec3d u, vec3d v) {

  bivec3 bv;

  bv.b01 = u[0]*v[1] - u[1]*v[0]; // XY
  bv.b02 = u[0]*v[2] - u[2]*v[0]; // XZ
  bv.b12 = u[1]*v[2] - u[2]*v[1]; // YZ

  return bv;

}

// construct the rotor that rotates one vector to another
// uses the usual trick to get the half angle
rot3 create_rotor(vec3d vFrom, vec3d vTo) {

  rot3 rot;

  bivec3 minusb;
  
  rot.a = 1 + dot(vTo, vFrom);
	// the left side of the products have b a, not a b, so flip
  minusb = wedge(vTo, vFrom);
  rot.bv.b01 = minusb.b01;
  rot.bv.b02 = minusb.b02;
  rot.bv.b12 = minusb.b12;
  normalize(&rot);

  return rot;
	
}

// angle+plane, plane must be normalized
rot3 create_rotorplane(double angleRadian, bivec3 *bv) {

  double sina;

  rot3 rot;
  
  sina = sin(angleRadian / 2.0);
  rot.a = cos(angleRadian / 2.0);

  // the left side of the products have b a, not a b
  rot.bv.b01 = -sina * bv->b01;
  rot.bv.b02 = -sina * bv->b02;
  rot.bv.b12 = -sina * bv->b12;

  return rot;
	
}

// Rotor3-Rotor3 product
// non-optimized
/*
inline Rotor3 Rotor3::operator*( const Rotor3& q ) const
{
	const Rotor3& p = *this;
	Rotor3 r;
	
	r.a = p.a * q.a 
	  - p.b01 * q.b01 - p.b02 * q.b02 - p.b12 * q.b12;

	r.b01 = p.b01 * q.a   + p.a   * q.b01
	  + p.b12 * q.b02 - p.b02 * q.b12;

	r.b02 = p.b02 * q.a   + p.a   * q.b02
	  - p.b12 * q.b01 + p.b01 * q.b12;

	r.b12 = p.b12 * q.a   + p.a   * q.b12
	  + p.b02 * q.b01 - p.b01 * q.b02;

	return r;
}
*/

/// R x R*
// non-optimized
int rotate(rot3 *rot, vec3d x, vec3d outvec) {

  vec3d q;
  
  double q012;
  
  // q = P x
  q[0] = rot->a * x[0] + x[1] * rot->bv.b01 + x[2] * rot->bv.b02;
  q[1] = rot->a * x[1] - x[0] * rot->bv.b01 + x[2] * rot->bv.b12;
  q[2] = rot->a * x[2] - x[0] * rot->bv.b02 - x[1] * rot->bv.b12;

  q012 = - x[0] * rot->bv.b12 + x[1] * rot->bv.b02 - x[2] * rot->bv.b01; // trivector

  // r = q P*
  (outvec)[0] = rot->a * q[0] + q[1] * rot->bv.b01 + q[2] * rot->bv.b02  - q012 * rot->bv.b12;
  (outvec)[1] = rot->a * q[1] - q[0] * rot->bv.b01 + q012 * rot->bv.b02  + q[2] * rot->bv.b12;
  (outvec)[2] = rot->a * q[2] - q012 * rot->bv.b01 - q[0] * rot->bv.b02  - q[1] * rot->bv.b12;

  return 0;
  
}

rot3 reverse(rot3 rot) {

  rot3 rot2;

  rot2.a = rot.a;
  rot2.bv.b01 = -(rot.bv.b01);
  rot2.bv.b02 = -(rot.bv.b02);
  rot2.bv.b12 = -(rot.bv.b12);  
  
  return rot2;
  
}

// rotate a rotor by another
/*
inline Rotor3 Rotor3::rotate( const Rotor3& r ) const
{
	// should unwrap this for efficiency
	return (*this) * r * (*this).reverse();
}
*/


double lengthsqrd(rot3 *rot) {

  return (rot->a * rot->a) + (rot->bv.b01 * rot->bv.b01) + (rot->bv.b02 * rot->bv.b02) + (rot->bv.b12 * rot->bv.b12);
  
}

double length(rot3 *rot) {
  return sqrt(lengthsqrd(rot));
}

void normalize(rot3 *rot) {
  double l;
  l = length(rot);
  rot->a /= l; rot->bv.b01 /= l; rot->bv.b02 /= l; rot->bv.b12 /= l;
}

// convert to matrix
// non-optimized
int toMatrix3(rot3 *rot, double *matrix) {

  vec3d regx = { 1.0, 0.0, 0.0 };
  vec3d regy = { 0.0, 1.0, 0.0 };
  vec3d regz = { 0.0, 0.0, 1.0 };  

  vec3d v0, v1, v2;
  
  rotate(rot, regx, v0);
  rotate(rot, regy, v1);
  rotate(rot, regz, v2);

  matrix[0] = v0[0]; matrix[1] = v1[0]; matrix[2] = v2[0]; matrix[3] = 0.0;
  matrix[4] = v0[1]; matrix[5] = v1[1]; matrix[6] = v2[1]; matrix[7] = 0.0;
  matrix[8] = v0[2]; matrix[9] = v1[2]; matrix[10] = v2[2]; matrix[11] = 0.0;
  matrix[12] = 0.0; matrix[13] = 0.0; matrix[14] = 0.0; matrix[15] = 0.0;
  
  return 0;
  
}

// geometric product (for reference), produces twice the angle, negative direction
rot3 geo(vec3d a, vec3d b) {

  bivec3 bv;

  bv = wedge(a,b);
  
  return create_rotorplane(dot(a,b), &bv);
  
}

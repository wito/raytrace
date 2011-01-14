#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"

typedef struct {
  intersectFunction intersect;
  normalFunction normal;

  void *material;

  vector centre;
  double radius;
} Sphere;

Primitive *createSphere(vector,double);
void destroySphere(Sphere *);

#endif // SPHERE_H

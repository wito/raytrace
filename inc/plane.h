#ifndef PLANE_H
#define PLANE_H

#include "primitive.h"

typedef struct {
  intersectFunction intersect;
  normalFunction normal;

  void *material;

  vector nvec;
  vector qvec;
} Plane;

Primitive *createPlane(vector, vector);
void destroyPlane(Plane *);

#endif // PLANE_H

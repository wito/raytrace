#include "plane.h"
#include <stdlib.h>

Intersection *planeIntersect(Primitive *p, Line *ray) {
  Plane *self = (Plane *)p;

  vector Q = self->qvec;
  vector N = self->nvec;

  vector E = ray->start;
  vector D = vectorSubtraction(ray->end, ray->start);

  double t = vectorDotProduct(vectorSubtraction(Q, E), N) / vectorDotProduct(N,D);

  if (t <= 0) {
    return NULL;
  }

  Intersection *retval = malloc(sizeof(Intersection));

  retval->primitive = p;
  retval->intersectionPoint = vectorAddition(E, vectorMultiply(D, t));
  retval->distance = vectorLength(retval->intersectionPoint);

  return retval;
}

vector planeNormal(Primitive *self, Intersection *ip) {
  return ((Plane*)self)->nvec;
}

Primitive *createPlane(vector N, vector Q) {
  Plane *self = malloc(sizeof(Plane));

  self->intersect = &planeIntersect;
  self->normal = &planeNormal;

  self->material = NULL;

  self->nvec = N;
  self->qvec = Q;

  return (Primitive *)self;
}

void destroyPlane(Plane *self) {
  free(self);
}

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "vector.h"
#include "material.h"

typedef struct primitive Primitive;

typedef struct {
  Primitive *primitive;
  vector intersectionPoint;

  double distance;

  int direction;  
} Intersection;

typedef Intersection *(*intersectFunction)(Primitive *, Line *);
typedef vector(*normalFunction)(Primitive *, Intersection *);

struct primitive {
  intersectFunction intersect;
  normalFunction normal;

  Material *material;
};

Intersection *primitiveIntersect(Primitive *, Line *);
vector primitiveNormal(Primitive *, Intersection *);

#endif // PRIMITIVE_H

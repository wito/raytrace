#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "vector.h"

typedef struct primitive Primitive;

typedef struct {
  Primitive *primitive;
  vector intersectionPoint;
} Intersection;

typedef Intersection *(*intersectFunction)(Primitive *, Line *);
typedef vector(*normalFunction)(Primitive *, Intersection *);

struct primitive {
  intersectFunction intersect;
  normalFunction normal;
};

Intersection *primitiveIntersect(Primitive *, Line *);
vector primitiveNormal(Primitive *, Intersection *);

#endif // PRIMITIVE_H

#include "primitive.h"

Intersection *primitiveIntersect(Primitive *self, Line *l) {
  return self->intersect(self,l);
}

vector primitiveNormal(Primitive *self, Intersection *ip) {
  return self->normal(self,ip);
}

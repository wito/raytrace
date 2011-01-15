#include "sphere.h"
#include <stdlib.h>
#include <math.h>

#define EPS 0.0005

Intersection *sphereIntersect(Primitive *s, Line *ray) {
  Sphere *self = (Sphere *)s;

  vector dp;

  dp.x = ray->end.x - ray->start.x;
  dp.y = ray->end.y - ray->start.y;
  dp.z = ray->end.z - ray->start.z;

  double a,b,c;
  double bb4ac;

  a = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z;
  b = 2 * (dp.x * (ray->start.x - self->centre.x) + dp.y * (ray->start.y - self->centre.y) + dp.z * (ray->start.z - self->centre.z));
  c = self->centre.x * self->centre.x + self->centre.y * self->centre.y + self->centre.z * self->centre.z;
  c += ray->start.x * ray->start.x + ray->start.y * ray->start.y + ray->start.z * ray->start.z;
  c -= 2 * (self->centre.x * ray->start.x + self->centre.y * ray->start.y + self->centre.z * ray->start.z);
  c -= self->radius * self->radius;
  bb4ac = b * b - 4 * a * c;

  if (fabs(a) < EPS || bb4ac < 0) {
    return NULL;
  }

  double mu1 = (-b + sqrt(bb4ac)) / (2 * a);
  double mu2 = (-b - sqrt(bb4ac)) / (2 * a);

  double mu = (mu1 >= mu2)?mu2:mu1;

  if (mu > 1.0 || mu < 0.0) {
    return NULL;
  }

  vector line = vectorMultiply(dp, mu);
  line = vectorAddition(line,ray->start);

  Intersection *retval = malloc(sizeof(Intersection));

  retval->primitive = s;
  retval->intersectionPoint = line;
  retval->distance = vectorLength(line);
  retval->direction = ((mu1 > 0.0 && mu1 < 1.0) && (mu2 > 0.0 && mu2 < 1.0))?1:-1;

  return retval;
}

vector sphereNormal(Primitive *self, Intersection *ip) {
  return vectorUnit(vectorSubtraction(ip->intersectionPoint, ((Sphere *)self)->centre));
}

Primitive *createSphere(vector l, double r) {
  Sphere *self = malloc(sizeof(Sphere));

  self->intersect = &sphereIntersect;
  self->normal = &sphereNormal;

  self->material = NULL;

  self->centre = l;
  self->radius = r;

  return (Primitive *)self;
}

void destroySphere(Sphere *self) {
  free(self);
}

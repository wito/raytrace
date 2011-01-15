#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector.h"

typedef struct {
  vector color;
  double specular;
  double diffuse;
  double reflection;
  double refraction;
  double transparency;
  double luminosity;
} Material;

#endif // MATERIAL_H

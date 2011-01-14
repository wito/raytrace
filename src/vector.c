#include "vector.h"

#include <math.h>
#include <stdlib.h>

vector vectorCreate(double x, double y, double z) {
  return (vector){ x, y, z };
}


double vectorLength(vector self) {
  return sqrt(vectorDotProduct(self,self));
}

vector vectorUnit(vector self) {
  return vectorDivide(self, vectorLength(self));
}


vector vectorAddition(vector a,vector b) {
  return vectorCreate(a.x + b.x, a.y + b.y, a.z + b.z);
}

vector vectorSubtraction(vector a, vector b) {
  return vectorCreate(a.x - b.x, a.y - b.y, a.z - b.z);
}

vector vectorMultiply(vector self, double r) {
  return vectorCreate(self.x * r, self.y * r, self.z * r);
}

vector vectorDivide(vector self, double r) {
  return vectorCreate(self.x / r, self.y / r, self.z / r);
}


vector vectorXProduct(vector a, vector b) {
  return vectorCreate(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

double vectorDotProduct(vector a,vector b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

vector vectorMatrixMultiply(vector self, matrix m) {
  return vectorCreate(
    self.x * m.a1 + self.y * m.a2 + self.z * m.a3 + m.a4,
    self.x * m.b1 + self.y * m.b2 + self.z * m.b3 + m.b4,
    self.x * m.c1 + self.y * m.c2 + self.z * m.c3 + m.c4
  );
}


Line *lineCreate(vector l, vector d) {
  Line *self = malloc(sizeof(Line));

  self->start = l;
  self->end = d;

  return self;
}

void lineDestroy(Line *self) {
  free(self);
}

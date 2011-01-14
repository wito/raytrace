#include "vector.h"

#include <math.h>

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

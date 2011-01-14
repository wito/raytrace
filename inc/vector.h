#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
  double x;
  double y;
  double z;
} vector;

vector vectorCreate(double, double, double);

double vectorLength(vector);
vector vectorUnit(vector);

vector vectorAddition(vector,vector);
vector vectorSubtraction(vector,vector);
vector vectorMultiply(vector,double);
vector vectorDivide(vector,double);

vector vectorXProduct(vector,vector);
double vectorDotProduct(vector,vector);

#endif // VECTOR_H

#ifndef VECTOR_H
#define VECTOR_H

#include "matrix.h"

typedef struct {
  double x;
  double y;
  double z;
} vector;

typedef struct {
  vector start;
  vector end;
} Line;

vector vectorCreate(double, double, double);

double vectorLength(vector);
vector vectorUnit(vector);

vector vectorAddition(vector,vector);
vector vectorSubtraction(vector,vector);
vector vectorMultiply(vector,double);
vector vectorDivide(vector,double);

vector vectorXProduct(vector,vector);
double vectorDotProduct(vector,vector);

vector vectorCProduct(vector,vector);

vector vectorMatrixMultiply(vector,matrix);

Line *lineCreate(vector, vector);
void lineDestroy(Line *);

#endif // VECTOR_H

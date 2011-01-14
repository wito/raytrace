#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
  double a1;
  double a2;
  double a3;
  double a4;

  double b1;
  double b2;
  double b3;
  double b4;

  double c1;
  double c2;
  double c3;
  double c4;

  double d1;
  double d2;
  double d3;
  double d4;
} matrix;

matrix matrixIdentity();

matrix matrixCreate(double[16]);

matrix matrixAddition(matrix, matrix);
matrix matrixSMultiply(matrix, double);
matrix matrixTranspose(matrix);

matrix matrixMultiply(matrix,matrix);

#endif // MATRIX_H

#include "matrix.h"

matrix matrixIdentity() {
  return matrixCreate((double[]){
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
  });
}


matrix matrixCreate(double values[16]) {
  matrix retval;

  retval.a1 = values[0];
  retval.a2 = values[1];
  retval.a3 = values[2];
  retval.a4 = values[3];

  retval.b1 = values[4];
  retval.b2 = values[5];
  retval.b3 = values[6];
  retval.b4 = values[7];

  retval.c1 = values[8];
  retval.c2 = values[9];
  retval.c3 = values[10];
  retval.c4 = values[11];

  retval.d1 = values[12];
  retval.d2 = values[13];
  retval.d3 = values[14];
  retval.d4 = values[15];

  return retval;
}


matrix matrixAddition(matrix A, matrix B) {
  matrix retval;

  retval.a1 = A.a1 + B.a1;
  retval.a2 = A.a2 + B.a2;
  retval.a3 = A.a3 + B.a3;
  retval.a4 = A.a4 + B.a4;

  retval.b1 = A.b1 + B.b1;
  retval.b2 = A.b2 + B.b2;
  retval.b3 = A.b3 + B.b3;
  retval.b4 = A.b4 + B.b4;

  retval.c1 = A.c1 + B.c1;
  retval.c2 = A.c2 + B.c2;
  retval.c3 = A.c3 + B.c3;
  retval.c4 = A.c4 + B.c4;

  retval.d1 = A.d1 + B.d1;
  retval.d2 = A.d2 + B.d2;
  retval.d3 = A.d3 + B.d3;
  retval.d4 = A.d4 + B.d4;

  return retval;
}

matrix matrixSMultiply(matrix A, double c) {
  matrix retval;

  retval.a1 = c * A.a1;
  retval.a2 = c * A.a2;
  retval.a3 = c * A.a3;
  retval.a4 = c * A.a4;

  retval.b1 = c * A.b1;
  retval.b2 = c * A.b2;
  retval.b3 = c * A.b3;
  retval.b4 = c * A.b4;

  retval.b1 = c * A.c1;
  retval.b2 = c * A.c2;
  retval.b3 = c * A.c3;
  retval.b4 = c * A.c4;

  retval.b1 = c * A.d1;
  retval.b2 = c * A.d2;
  retval.b3 = c * A.d3;
  retval.b4 = c * A.d4;

  return retval;
}

matrix matrixTranspose(matrix A) {
  matrix retval;

  retval.a1 = A.a1;
  retval.a2 = A.b1;
  retval.a3 = A.c1;
  retval.a4 = A.d1;

  retval.b1 = A.a2;
  retval.b2 = A.b2;
  retval.b3 = A.c2;
  retval.b4 = A.d2;

  retval.c1 = A.a3;
  retval.c2 = A.b3;
  retval.c3 = A.c3;
  retval.c4 = A.d3;

  retval.d1 = A.a4;
  retval.d2 = A.b4;
  retval.d3 = A.c4;
  retval.d4 = A.d4;

  return retval;
}


matrix matrixMultiply(matrix A, matrix B) {
  matrix retval;

  retval.a1 = A.a1 * B.a1 + A.a2 * B.b1 + A.a3 * B.c1 + A.a4 * B.d1;
  retval.a2 = A.a1 * B.a2 + A.a2 * B.b2 + A.a3 * B.c2 + A.a4 * B.d2;
  retval.a3 = A.a1 * B.a3 + A.a2 * B.b3 + A.a3 * B.c3 + A.a4 * B.d3;
  retval.a4 = A.a1 * B.a4 + A.a2 * B.b4 + A.a3 * B.c4 + A.a4 * B.d4;

  retval.b1 = A.b1 * B.a1 + A.b2 * B.b1 + A.b3 * B.c1 + A.b4 * B.d1;
  retval.b2 = A.b1 * B.a2 + A.b2 * B.b2 + A.b3 * B.c2 + A.b4 * B.d2;
  retval.b3 = A.b1 * B.a3 + A.b2 * B.b3 + A.b3 * B.c3 + A.b4 * B.d3;
  retval.b4 = A.b1 * B.a4 + A.b2 * B.b4 + A.b3 * B.c4 + A.b4 * B.d4;

  retval.c1 = A.c1 * B.a1 + A.c2 * B.b1 + A.c3 * B.c1 + A.c4 * B.d1;
  retval.c2 = A.c1 * B.a2 + A.c2 * B.b2 + A.c3 * B.c2 + A.c4 * B.d2;
  retval.c3 = A.c1 * B.a3 + A.c2 * B.b3 + A.c3 * B.c3 + A.c4 * B.d3;
  retval.c4 = A.c1 * B.a4 + A.c2 * B.b4 + A.c3 * B.c4 + A.c4 * B.d4;

  retval.d1 = A.d1 * B.a1 + A.d2 * B.b1 + A.d3 * B.c1 + A.d4 * B.d1;
  retval.d2 = A.d1 * B.a2 + A.d2 * B.b2 + A.d3 * B.c2 + A.d4 * B.d2;
  retval.d3 = A.d1 * B.a3 + A.d2 * B.b3 + A.d3 * B.c3 + A.d4 * B.d3;
  retval.d4 = A.d1 * B.a4 + A.d2 * B.b4 + A.d3 * B.c4 + A.d4 * B.d4;

  return retval;
}

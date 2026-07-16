#ifndef MATRIX_MUL_H
#define MATRIX_MUL_H

//#include<ap_int.h>
//typedef ap_int<8> data_t;
//typedef ap_int<32> acc_t;

typedef float data_t;
typedef float acc_t;

#define MAX_M 32
#define MAX_N 32
#define MAX_K 32

void matrix_mul(
data_t *A,
data_t *B,
data_t *C,
int M,
int K,
int N
);

#endif

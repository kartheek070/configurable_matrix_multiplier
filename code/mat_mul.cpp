#include "mat_mul.h"

void matrix_mul(
data_t *A,
data_t *B,
data_t *C,
int M,
int K,
int N
){

#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem

#pragma HLS INTERFACE s_axilite port=M bundle=control
#pragma HLS INTERFACE s_axilite port=K bundle=control
#pragma HLS INTERFACE s_axilite port=N bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

	//load buffer A
	data_t local_A[MAX_M][MAX_K];
	load_A:
	for (int r = 0; r < M; r++)
	{
	#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_M avg=16

	    for (int k = 0; k < K; k++)
	    {
	#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_K avg=16

	        local_A[r][k] = A[r*K + k];
	    }
	}

	//load buffer B
	data_t local_B[MAX_K][MAX_N];
	load_B:
	for (int r = 0; r < K; r++)
	{
	#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_K avg=16

	    for (int k = 0; k < N; k++)
	    {
	#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_N avg=16

	        local_B[r][k] = B[r*N + k];
	    }
	}

//Array Partitioning
//#pragma HLS ARRAY_PARTITION variable=local_A type=cyclic factor=4 dim=2
//#pragma HLS ARRAY_PARTITION variable=local_B type=cyclic factor=4 dim=1

	//computation
	row_loop: for (int r=0; r<M; r++)
	{
#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_M avg=16
	column_loop:	for(int c=0; c<N; c++)
		{
#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_N avg=16
			acc_t sum=0;
			iter_loop: for(int iter=0; iter<K; iter++)
			{
#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_K avg=16
#pragma HLS PIPELINE II=1
//#pragma HLS UNROLL factor=2
				sum += local_A[r][iter] * local_B[iter][c];
				//sum +=A[r*K+iter]*B[iter*N+c];
			}
			C[r*N + c]=(data_t)sum;
		}
	}
}

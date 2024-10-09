/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* gemm.c: this file is part of PolyBench/C */



#   define NI 60
#   define NJ 70
#   define NK 80
void dut (
		 double alpha,
		 double beta,
		 double C[NI][NJ],
		 double A[NI][NK],
		 double B[NK][NJ])
{
  int i, j, k;
  int ni = NI;
  int nj = NJ;
  int nk = NK;
  alpha = 1.5;
  beta = 1.2;

//BLAS PARAMS
//TRANSA = 'N'
//TRANSB = 'N'
// => Form C := alpha*A*B + beta*C,
//A is NIxNK
//B is NKxNJ
//C is NIxNJ

  for (i = 0; i < ni; i++) {
    for (j = 0; j < nj; j++)
	C[i][j] *= beta;
    for (k = 0; k < nk; k++) {
       for (j = 0; j < nj; j++)
	  C[i][j] += alpha * A[i][k] * B[k][j];
    }
  }


}


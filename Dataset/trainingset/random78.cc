/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* 2mm.c: this file is part of PolyBench/C */


#   define NI 16
#   define NJ 18
#   define NK 22
#   define NL 24
void dut(int ni, int nj, int nk, int nl,
		double alpha,
		double beta,
		double tmp[NI][NJ],
		double A[NI][NK],
		double B[NK][NJ],
		double C[NJ][NL],
		double D[NI][NL])
{
  int i, j, k;
  ni = NI;
  nj = NJ;
  nk = NK;
  nl = NL;

  /* D := alpha*A*B*C + beta*D */
  for (i = 0; i < ni; i++)
    for (j = 0; j < nj; j++)
      {
	tmp[i][j] = 0.0;
	for (k = 0; k < nk; ++k)
	  tmp[i][j] += alpha * A[i][k] * B[k][j];
      }
  for (i = 0; i < ni; i++)
    for (j = 0; j < nl; j++)
      {
	D[i][j] *= beta;
	for (k = 0; k < nj; ++k)
	  D[i][j] += tmp[i][k] * C[k][j];
      }


}


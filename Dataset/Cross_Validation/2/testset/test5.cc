/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* gemver.c: this file is part of PolyBench/C */


#   define N 40

void dut (
		   double alpha,
		   double beta,
		   double A[N][N],
		   double u1[N],
		   double v1[N],
		   double u2[N],
		   double v2[N],
		   double w[N],
		   double x[N],
		   double y[N],
		   double z[N])
{
  int i, j;
  int n = N;
  alpha = 1.5;
  beta = 1.2;


  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      A[i][j] = A[i][j] + u1[i] * v1[j] + u2[i] * v2[j];

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      x[i] = x[i] + beta * A[j][i] * y[j];

  for (i = 0; i < n; i++)
    x[i] = x[i] + z[i];

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      w[i] = w[i] +  alpha * A[i][j] * x[j];


}

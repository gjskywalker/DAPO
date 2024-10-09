/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* gesummv.c: this file is part of PolyBench/C */


#   define N 30

void dut (
		    double alpha,
		    double beta,
		    double A[N][N],
		    double B[N][N],
		    double tmp[N],
		    double x[N],
		    double y[N])
{
  int i, j;
  int n = N;
  alpha = 1.5;
  beta = 1.2;
  for (i = 0; i < n; i++)
    {
      tmp[i] = 0.0;
      y[i] = 0.0;
      for (j = 0; j < n; j++)
	{
	  tmp[i] = A[i][j] * x[j] + tmp[i];
	  y[i] = B[i][j] * x[j] + y[i];
	}
      y[i] = alpha * tmp[i] + beta * y[i];
    }


}

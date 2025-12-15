
#   define M 38
#   define N 42
void dut(
		 double A[M][N],
		 double x[N],
		 double y[N],
		 double tmp[M])
{
  int m = M;
  int n = N;
  int i, j;

  for (i = 0; i < n; i++)
    y[i] = 0;
  for (i = 0; i <   m; i++)
    {
      tmp[i] = 0.0;
      for (j = 0; j < n; j++)
	tmp[i] = tmp[i] + A[i][j] * x[j];
      for (j = 0; j < n; j++)
	y[j] = y[j] + A[i][j] * tmp[i];
    }

}

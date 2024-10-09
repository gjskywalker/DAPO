
#   define M 38
#   define N 42
void dut(
		 double A[N][M],
		 double s[M],
		 double q[N],
		 double p[M],
		 double r[N])
{
  int i, j;
  int m = M;
  int n = N;
  for (i = 0; i < m; i++)
    s[i] = 0;
  for (i = 0; i < n; i++)
  {
    q[i] = 0.0;
    for (j = 0; j < m; j++)
    {
      s[j] = s[j] + r[i] * A[i][j];
      q[i] = q[i] + A[i][j] * p[j];
    }
  }

}

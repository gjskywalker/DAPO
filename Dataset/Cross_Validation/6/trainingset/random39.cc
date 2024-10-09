
#   define N 40

void dut(int n,
		double x1[N],
		double x2[N],
		double y_1[N],
		double y_2[N],
		double A[N][N])
{
  int i, j;
  n = N;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      x1[i] = x1[i] + A[i][j] * y_1[j];
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      x2[i] = x2[i] + A[j][i] * y_2[j];

}


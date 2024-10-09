
#   define TSTEPS 20
#   define N 30
void dut (
    double A[N],
    double B[N])
{
  int t, i;
  int tsteps = TSTEPS;
  int n = N;

  for (t = 0; t < tsteps; t++)
    {
      for (i = 1; i < n - 1; i++)
	B[i] = 0.33333 * (A[i-1] + A[i] + A[i + 1]);
      for (i = 1; i < n - 1; i++)
	A[i] = 0.33333 * (B[i-1] + B[i] + B[i + 1]);
    }

}


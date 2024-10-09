/* generated on Fri Jun 07*/
/* This test set is based on HLS-benchmarks */
/* This test comes from HLS-benchmarks/C-Slow/doitgenTriple */

#define N 256

int dut()
{
  float A[N], B[N], sum[N], sum_[N], w[N * N];
  int p = 0;

  for (int i = 0; i < N; i++)
  {
    A[i] = (i % 2) ? 1 : -1;
    B[i] = A[i];
    for (int j = 0; j < N; j++)
      w[i * N + j] = 0;
    sum[i] = 0;
    sum_[i] = 0;
  }

  for (int i = 0; i < N; i++)
  {
    float s = 0;

    for (int j = 0; j < N; j++)
    {
      float a = A[j];
      float wt = w[p + j];
      if (a > 0.0)
      {
        float b = a * wt;
        float c = b + wt;
        float d = c * a;
        s = s + d;
      }
    }
    p += N;
    sum[i] = s;
  }

  for (int i = 0; i < N - 1; i++)
  {
    float q = sum[i];
    float b = A[i];
    float c = q * q;
    float d = c + 0.5f;
    float e = d * q;
    float f = e + b;
    A[i + 1] = f;
  }
  return 0;
}
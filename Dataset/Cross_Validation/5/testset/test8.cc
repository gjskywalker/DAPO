
#   define NQ 20
#   define NR 25
#   define NP 30
void dut (
      double A[NR][NQ][NP],
      double C4[NP][NP],
      double sum[NP])
{
  int r, q, p, s;
  int nr = NR;
  int nq = NQ;
  int np = NP;
  for (r = 0; r < nr; r++)
    for (q = 0; q < nq; q++)  {
      for (p = 0; p < np; p++)  {
	sum[p] = 0.0;
	for (s = 0; s < np; s++)
	  sum[p] += A[r][q][s] * C4[s][p];
      }
      for (p = 0; p < np; p++)
	A[r][q][p] = sum[p];
    }

}


/* generated on Sat Jun 08 */
/* This test set comes from MachSuite */
/* This test comes from MachSuite/spmv/crs */

/* gvn,loop-unroll,ipsccp,sroa,loop-simplifycfg,loop-unroll,gvn,loop-rotate */

#define NNZ 1666
#define N 494

#define TYPE double

void dut(TYPE val[NNZ], int cols[NNZ], TYPE vec[N], TYPE out[N])
{
    int i, j;
    TYPE sum, Si;

spmv_1:
    for (i = 0; i < N; i++)
    {
        sum = 0;
        sum = val[0] * vec[cols[0]] + val[1] * vec[cols[1]];
    spmv_2:
        for (j = 2; j < N * 3; j++)
        {
            #pragma HLS unroll factor=4
            Si = val[j] * vec[cols[j]];
            sum = sum + Si;
        }
        out[i] = sum;
    }
}
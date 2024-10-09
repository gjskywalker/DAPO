/* generated on Fri Jun 07*/
/* This test set comes from HLS-benchmarks */
/* This test comes from HLS-benchmarks/C-Slow/gramSchmidt */

int dut(float a[1024], float r[1024], float q[1024])
{
    int kk = 0;
    for (int k = 0; k < 32; k++)
    {
        float nrm = 0.0f;
        for (int i = 0; i < 32 * 32; i += 32)
            nrm += a[i + k] * a[i + k];
        nrm = 0.0019f * ((nrm - 8.0f) * nrm + 16.0f) * nrm + 2.0f;
        r[kk + k] = nrm;
        for (int i = 0; i < 32 * 32; i += 32)
            q[i + k] = a[i + k] / r[kk + k];
        for (int j = k + 1; j < 32; j++)
        {
            r[kk + j] = 0.0f;
            for (int i = 0; i < 32 * 32; i += 32)
                r[kk + j] += q[i + k] * a[i + j];
            for (int i = 0; i < 32 * 32; i += 32)
                a[i + j] = a[i + j] - q[i + k] * r[kk + j];
        }
        kk += 32;
    }
    return 0;
}
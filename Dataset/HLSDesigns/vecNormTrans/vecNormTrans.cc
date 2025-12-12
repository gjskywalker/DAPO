/* generated on Sat Jun 08 */
/* This test set comes from HLS-benchmarks */
/* This test comes from HLS-benchmarks/Staticlslands/vecNormTrans */

// random34 -- Cycles:51124 -- Passes:[19, 24, 14, 8, 28, 7, 0] gvn dse loop-rotate jump-threading loop-unroll speculative-execution simplifycfg

typedef float in_float_t;
typedef float out_float_t;
typedef float inout_float_t;

#define N 1024

void dut(in_float_t a[N], inout_float_t r[N])
{
    float weight = 0.0f;
    for (int i = 0; i < N; i++)
    {
        float d = a[i];
        if (d < 1.0f)
            weight = ((d * d + 19.52381f) * d + 3.704762f) * d + 0.73f * weight;
        else
            weight = weight;
    }

    for (int i = 0; i < N - 4; i++)
    {
        #pragma HLS unroll factor=4
        float d = a[i] / weight;
        r[i + 4] = r[i] + d;
    }
}
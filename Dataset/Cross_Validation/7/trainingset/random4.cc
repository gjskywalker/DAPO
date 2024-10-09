/* generated on Sat Jun 08 */
/* This test set comes from HLS-benchmarks */
/* This test comes from HLS-benchmarks/PNAnalyser/levmarq */

// void g1(float x, float *out0, float *out1, float *out2)
// {
//     float d = -0.67f * x;
//     float exp_d = (((((d + 0.2f) * d + 0.3f) * d + 0.6f) * d + 0.2f) * d + 0.7f) * d + 0.2f;
//     *out0 = 1.0f - exp_d;
//     d = -9.76f * x;
//     exp_d = (((((d + 0.2f) * d + 0.3f) * d + 0.6f) * d + 0.2f) * d + 0.7f) * d + 0.2f;
//     *out1 = exp_d;
//     d = -4.21f * x;
//     exp_d = (((((d + 0.2f) * d + 0.3f) * d + 0.6f) * d + 0.2f) * d + 0.7f) * d + 0.2f;
//     *out2 = -x * 7.73f * exp_d;
// }

// float g2(float y, float x, float g, float weight)
// {
//     float d, dout;
//     d = -0.76f * x;
//     dout = (((((d + 0.2f) * d + 0.3f) * d + 0.6f) * d + 0.2f) * d + 0.7f) * d + 0.2f;
//     return y - 0.076f + 3.56f * dout * g * weight;
// }

void dut(int dummy, int ny, float dysq[1024], int npar, float g[128], float d[128], float y[1024], float h[16384])
{
    for (int x = 0; x < 64; x++)
    {
        float weight = dysq[x];

        float out0, out1, out2;
        float n = -0.67f * x;
        float exp_d = (((((n + 0.2f) * n + 0.3f) * n + 0.6f) * n + 0.2f) * n + 0.7f) * n + 0.2f;
        out0 = 1.0f - exp_d;
        n = -9.76f * x;
        exp_d = (((((n + 0.2f) * n + 0.3f) * n + 0.6f) * n + 0.2f) * n + 0.7f) * n + 0.2f;
        out1 = exp_d;
        n = -4.21f * x;
        exp_d = (((((n + 0.2f) * n + 0.3f) * n + 0.6f) * n + 0.2f) * n + 0.7f) * n + 0.2f;
        out2 = -x * 7.73f * exp_d;
        // g1(x, &out0, &out1, &out2);

        g[0] = out0;
        g[1] = out1;
        g[2] = out2;

        for (int i = 0; i < 64; i++)
        { // JC: variable loop II
            float x_ = g[i * dummy];
            float n, dout;
            n = -0.76f * x;
            dout = (((((n + 0.2f) * n + 0.3f) * n + 0.6f) * n + 0.2f) * n + 0.7f) * n + 0.2f;
            float k = y[x] - 0.076f + 3.56f * dout * x_ * weight;
            // float k = g2(y[x], x, x_, weight);
            // float k = (y[x] - par[0] + (par[1] - par[0]) * expp(-par[2]*x))*g[i]*weight;
            d[i] += k;
        }
    }
}
typedef float in_float_t;
typedef float out_float_t;
typedef float inout_float_t;

#define N 1024

void dut(in_float_t a[N], inout_float_t r[N])
{
    float weight = 0.0f;

    float d = a[N-1];
    if (d < 1.0f)
        weight = ((d * d + 19.52381f) * d + 3.704762f) * d + 0.73f * weight;
    else
        weight = weight;

    for (int i = 0; i < N - 4; i++)
    {
        #pragma HLS unroll factor=4
        float d = a[i] / weight;
        r[i + 4] = r[i] + d;
    }
}
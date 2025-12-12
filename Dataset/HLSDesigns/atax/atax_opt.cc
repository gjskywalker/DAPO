#define M 100
#define N 5

void dut(double A[M][N], double x[N], double y[N], double tmp[M]) {
    int i, j;

    for (i = 0; i < M; i++) {
        tmp[i] = 0;
        for (j = 0; j < N; j++) {
            tmp[i] += A[i][j] * x[j];
        }
    }

    for (j = 0; j < N; j++) {
        y[j] = 0;
        for (i = 0; i < M; i++) {
            #pragma HLS unroll factor=5
            y[j] += A[i][j] * tmp[i];
        }
    }
}
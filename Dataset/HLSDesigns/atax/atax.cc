#define M 100
#define N 5

double dot(double* row, double* x, int n) {
    double sum = 0;
    for (int j=0; j<n; j++)
        sum += row[j] * x[j];
    return sum;
}

void dut(double A[M][N], double x[N], double y[N], double tmp[M]) {
    int i, j;

    for (j = 0; j < N; j++) {
        y[j] = 0;
        for (i = 0; i < M; i++) {
        #pragma HLS unroll factor=5
            y[j] += A[i][j] * dot(A[i], x, N);
        }
    }
}
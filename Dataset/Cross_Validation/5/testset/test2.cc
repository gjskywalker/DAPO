void dut(int m1[4096], int m2[4096], int prod[4096]) {
    int i, j, k;
    int k_col, i_col;
    int mult;

    for (i = 0; i < 64; i++) { // {"m1": {"dim": 1, "type": "block", "factor": 64}, "prod": {"dim": 1, "type": "block", "factor": 64}}
        for (j = 0; j < 64; j++) { // {"m2": {"dim": 1, "type": "cyclic", "factor": 0}, "prod": {"dim": 1, "type": "cyclic", "factor": 0}}
            i_col = i * 64;
            int sum = 0;
            for (k = 0; k < 64; k++) { // {"m2": {"dim": 1, "type": "block", "factor": 64}}
                k_col = k * 64;
                mult = m1[i_col + k] * m2[k_col + j];
                sum += mult;
            }
            prod[i_col + j] = sum;
        }
    }
}

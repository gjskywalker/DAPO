void dut(int m1[4096], int m2[4096], int prod[4096]) {
    int i, k, j, jj, kk;
    int i_row, k_row;
    int temp_x, mul;


    for (jj = 0; jj < 64; jj += 8) { // SKIP_DSE
        for (kk = 0; kk < 64; kk += 8) { // SKIP_DSE
            for (i = 0; i < 64; ++i) { // {"m1": {"dim": 1, "type": "block", "factor": 64}, "prod": {"dim": 1, "type": "block", "factor": 64}}
                for (k = 0; k < 8; ++k) { // {"m2": {"dim": 1, "type": "block", "factor": 64}}
                    i_row = i * 64;
                    k_row = (k + kk) * 64;
                    temp_x = m1[i_row + k + kk];
                    for (j = 0; j < 8; ++j) { // {"m2": {"dim": 1, "type": "cyclic", "factor": 0}}
                        mul = temp_x * m2[k_row + j + jj];
                        prod[i_row + j + jj] += mul;
                    }
                }
            }
        }
    }
}

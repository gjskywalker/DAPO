void dut(int weights1[832], int weights2[4096], int weights3[192], int d_weights1[832], int d_weights2[4096], int d_weights3[192], int biases1[64], int biases2[64], int biases3[3], int d_biases1[64], int d_biases2[64], int d_biases3[3]) {
    int i, j;
    int norm, bias_norm;
    norm = 0.0;
    bias_norm = 0.0;

    for (i = 0; i < 13; i++) {     // {"weights1": {"dim": 1, "type": "block", "factor": 64}, "d_weights1": {"dim": 1, "type": "block", "factor": 64}}
        for (j = 0; j < 64; j++) { // {"weights1": {"dim": 1, "type": "cyclic", "factor": 0}, "d_weights1": {"dim": 1, "type": "cyclic", "factor": 0}}
            weights1[i * 64 + j] -= (d_weights1[i * 64 + j] * 2);
            norm += weights1[i * 64 + j] * weights1[i * 64 + j];
        }
    }
    for (i = 0; i < 64; i++) { // {"biases1": {"dim": 1, "type": "cyclic", "factor": 0}, "d_biases1": {"dim": 1, "type": "cyclic", "factor": 0}}
        biases1[i] -= (d_biases1[i] * 2);
        bias_norm += biases1[i] * biases1[i];
    }

    // norm = sqrt(norm);
    // bias_norm = sqrt(bias_norm);

    for (i = 0; i < 13; i++) {     // {"weights1": {"dim": 1, "type": "block", "factor": 64}}
        for (j = 0; j < 64; j++) { // {"weights1": {"dim": 1, "type": "cyclic", "factor": 0}}
            weights1[i * 64 + j] = (weights1[i * 64 + j] / norm);
        }
    }
    for (i = 0; i < 64; i++) { // {"biases1": {"dim": 1, "type": "cyclic", "factor": 0}}
        biases1[i] = (biases1[i] / bias_norm);
    }

    // norm = (int)0.0;
    // bias_norm = (int)0.0;

    for (i = 0; i < 64; i++) {     // {"weights2": {"dim": 1, "type": "block", "factor": 64}, "d_weights2": {"dim": 1, "type": "block", "factor": 64}}
        for (j = 0; j < 64; j++) { // {"weights2": {"dim": 1, "type": "cyclic", "factor": 0}, "d_weights2": {"dim": 1, "type": "cyclic", "factor": 0}}
            weights2[i * 64 + j] -= (d_weights2[i * 64 + j] * 2);
            norm += weights2[i * 64 + j] * weights2[i * 64 + j];
        }
    }
    for (i = 0; i < 64; i++) { // {"biases2": {"dim": 1, "type": "cyclic", "factor": 0}, "d_biases2": {"dim": 1, "type": "cyclic", "factor": 0}}
        biases2[i] -= (d_biases2[i] * 2);
        bias_norm += biases2[i] * biases2[i];
    }

    // norm = sqrt(norm);
    // bias_norm = sqrt(bias_norm);

    for (i = 0; i < 64; i++) {     // {"weights2": {"dim": 1, "type": "block", "factor": 64}}
        for (j = 0; j < 64; j++) { // {"weights2": {"dim": 1, "type": "cyclic", "factor": 0}}
            weights2[i * 64 + j] = (weights2[i * 64 + j] / norm);
        }
    }
    for (i = 0; i < 64; i++) { // {"biases2": {"dim": 1, "type": "cyclic", "factor": 0}}
        biases2[i] = (biases2[i] / bias_norm);
    }

    // norm = (int)0.0;
    // bias_norm = (int)0.0;

    for (i = 0; i < 64; i++) {    // {"weights3": {"dim": 1, "type": "block", "factor": 3}, "d_weights3": {"dim": 1, "type": "block", "factor": 3}}
        for (j = 0; j < 3; j++) { // {"weights3": {"dim": 1, "type": "cyclic", "factor": 0}, "d_weights3": {"dim": 1, "type": "cyclic", "factor": 0}}
            weights3[i * 3 + j] -= (d_weights3[i * 3 + j] * 2);
            norm += weights3[i * 3 + j] * weights3[i * 3 + j];
        }
    }
    for (i = 0; i < 3; i++) { // {"biases3": {"dim": 1, "type": "cyclic", "factor": 0}, "d_biases3": {"dim": 1, "type": "cyclic", "factor": 0}}
        biases3[i] -= d_biases3[i] * 2;
        bias_norm += biases3[i] * biases3[i];
    }

    // norm = sqrt(norm);
    // bias_norm = sqrt(bias_norm);

    for (i = 0; i < 64; i++) {    // {"weights3": {"dim": 1, "type": "block", "factor": 3}}
        for (j = 0; j < 3; j++) { // {"weights3": {"dim": 1, "type": "cyclic", "factor": 0}}
            weights3[i * 3 + j] = (weights3[i * 3 + j] / norm);
        }
    }
    for (i = 0; i < 3; i++) { // {"biases3": {"dim": 1, "type": "cyclic", "factor": 0}}
        biases3[i] = (biases3[i] / bias_norm);
    }
}
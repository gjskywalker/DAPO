void dut(float weights1[832], float weights2[4096], float weights3[192], float d_weights1[832], float d_weights2[4096], float d_weights3[192], float biases1[64], float biases2[64], float biases3[3], float d_biases1[64], float d_biases2[64], float d_biases3[3]) {
    int i, j;
    float norm, bias_norm;
    norm = 0.0;
    bias_norm = 0.0;

    for (i = 0; i < 13; i++) {     // {"weights1": {"dim": 1, "type": "block", "factor": 64}, "d_weights1": {"dim": 1, "type": "block", "factor": 64}}
        for (j = 0; j < 64; j++) { // {"weights1": {"dim": 1, "type": "cyclic", "factor": 0}, "d_weights1": {"dim": 1, "type": "cyclic", "factor": 0}}
            weights1[i * 64 + j] -= (d_weights1[i * 64 + j] * 0.01);
            norm += weights1[i * 64 + j] * weights1[i * 64 + j];
        }
    }
    for (i = 0; i < 64; i++) { // {"biases1": {"dim": 1, "type": "cyclic", "factor": 0}, "d_biases1": {"dim": 1, "type": "cyclic", "factor": 0}}
        biases1[i] -= (d_biases1[i] * 0.01);
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

    // norm = (float)0.0;
    // bias_norm = (float)0.0;

    for (i = 0; i < 64; i++) {     // {"weights2": {"dim": 1, "type": "block", "factor": 64}, "d_weights2": {"dim": 1, "type": "block", "factor": 64}}
        for (j = 0; j < 64; j++) { // {"weights2": {"dim": 1, "type": "cyclic", "factor": 0}, "d_weights2": {"dim": 1, "type": "cyclic", "factor": 0}}
            weights2[i * 64 + j] -= (d_weights2[i * 64 + j] * 0.01);
            norm += weights2[i * 64 + j] * weights2[i * 64 + j];
        }
    }
    for (i = 0; i < 64; i++) { // {"biases2": {"dim": 1, "type": "cyclic", "factor": 0}, "d_biases2": {"dim": 1, "type": "cyclic", "factor": 0}}
        biases2[i] -= (d_biases2[i] * 0.01);
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

    // norm = (float)0.0;
    // bias_norm = (float)0.0;

    for (i = 0; i < 64; i++) {    // {"weights3": {"dim": 1, "type": "block", "factor": 3}, "d_weights3": {"dim": 1, "type": "block", "factor": 3}}
        for (j = 0; j < 3; j++) { // {"weights3": {"dim": 1, "type": "cyclic", "factor": 0}, "d_weights3": {"dim": 1, "type": "cyclic", "factor": 0}}
            weights3[i * 3 + j] -= (d_weights3[i * 3 + j] * 0.01);
            norm += weights3[i * 3 + j] * weights3[i * 3 + j];
        }
    }
    for (i = 0; i < 3; i++) { // {"biases3": {"dim": 1, "type": "cyclic", "factor": 0}, "d_biases3": {"dim": 1, "type": "cyclic", "factor": 0}}
        biases3[i] -= d_biases3[i] * 0.01;
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
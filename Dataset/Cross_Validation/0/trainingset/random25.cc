void dut(double weights3[192], double output_differences[3], double oracle_activations[64], double dactivations[64]) {
    int i, j;
    for (i = 0; i < 64; i++) { // {"oracle_activations": {"dim": 1, "type": "cyclic", "factor": 0}, "dactivations": {"dim": 1, "type": "cyclic", "factor": 0}, "weights3": {"dim": 1, "type": "block", "factor": 3}}
        oracle_activations[i] = (double)0.0;
        for (j = 0; j < 3; j++) { // {"output_differences": {"dim": 1, "type": "cyclic", "factor": 0}, "weights3": {"dim": 1, "type": "cyclic", "factor": 0}}
            oracle_activations[i] += output_differences[j] * weights3[i * 3 + j];
        }
        oracle_activations[i] = oracle_activations[i] * dactivations[i];
    }
}
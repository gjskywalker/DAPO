void dut(int biases[64], int weights[832], int activations[64], int input_sample[13]) {
    int i, j;
    for (j = 0; j < 64; j++) { // {"activations": {"dim": 1, "type": "cyclic", "factor": 0}, "weights": {"dim": 1, "type": "block", "factor": 13}}
        activations[j] = (int)0.0;
        for (i = 0; i < 13; i++) { // {"weights": {"dim": 1, "type": "cyclic", "factor": 0}, "input_sample": {"dim": 1, "type": "cyclic", "factor": 0}}
            activations[j] += weights[j * 13 + i] * input_sample[i];
        }
    }
    for (i = 0; i < 64; i++) { // {"activations": {"dim": 1, "type": "cyclic", "factor": 0}, "biases": {"dim": 1, "type": "cyclic", "factor": 0}}
        activations[i] = activations[i] + biases[i];
    }
}
void dut(int grad[1024], int feature[1024], int scale, int param[1024]) {
    for (int i = 0; i < 32; i++) { // {"grad": {"dim": 1, "type": "block", "factor": 32}, "feature": {"dim": 1, "type": "block", "factor": 32}}
        for (int j = 0; j < 32; j++) { // {"grad": {"dim": 1, "type": "cyclic", "factor": 0}, "feature": {"dim": 1, "type": "cyclic", "factor": 0}}
            grad[i * 32 + j] = (scale * feature[i * 32 + j]);
        }
    }
    for (int i = 0; i < 32 / 32; i++) { // {"grad": {"dim": 1, "type": "block", "factor": 32}, "param": {"dim": 1, "type": "block", "factor": 32}}
        for (int j = 0; j < 32; j++) { // {"grad": {"dim": 1, "type": "cyclic", "factor": 0}, "param": {"dim": 1, "type": "cyclic", "factor": 0}}
            param[i * 32 + j] += scale * grad[i * 32 + j];
        }
    }
}

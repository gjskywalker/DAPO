void dut(int frame1[32][32], int frame2[32][32], int frame3[32][32], int frame4[32][32], int frame5[32][32], int gradient_z[32][32]) {
    const int GRAD_WEIGHTS[] = {1, -8, 1, 8, -1};
    for (int r = 0; r < 32; r++) { // {"gradient_z": {"dim": 1, "type": "cyclic", "factor": 0}, "frame1": {"dim": 1, "type": "cyclic", "factor": 0}, "frame2": {"dim": 1, "type": "cyclic", "factor": 0}, "frame3": {"dim": 1, "type": "cyclic", "factor": 0}, "frame4": {"dim": 1, "type": "cyclic", "factor": 0}, "frame5": {"dim": 1, "type": "cyclic", "factor": 0}}
        for (int c = 0; c < 32; c++) { // {"gradient_z": {"dim": 2, "type": "cyclic", "factor": 0}, "frame1": {"dim": 2, "type": "cyclic", "factor": 0}, "frame2": {"dim": 2, "type": "cyclic", "factor": 0}, "frame3": {"dim": 2, "type": "cyclic", "factor": 0}, "frame4": {"dim": 2, "type": "cyclic", "factor": 0}, "frame5": {"dim": 2, "type": "cyclic", "factor": 0}}
            gradient_z[r][c] = ((int)(frame1[r][c] * GRAD_WEIGHTS[0] + frame2[r][c] * GRAD_WEIGHTS[1] + frame3[r][c] * GRAD_WEIGHTS[2] + frame4[r][c] * GRAD_WEIGHTS[3] + frame5[r][c] * GRAD_WEIGHTS[4])) / 12;
        }
    }
}
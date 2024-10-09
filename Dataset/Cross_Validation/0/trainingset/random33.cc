void dut(double val[32], double vec[16], double out[16]) {
    int i, j;
    double sum, Si;

    for (i = 0; i < 16; i++){   // {"rowDelimiters": {"dim": 1, "type": "cyclic", "factor": 0}, "out": {"dim": 1, "type": "cyclic", "factor": 0}}
        sum = 0; Si = 0;
        for (j = 0; j < 16; j++){ // {"val": {"dim": 1, "type": "cyclic", "factor": 0}, "cols": {"dim": 1, "type": "cyclic", "factor": 0}}
            Si = val[j] * vec[j];
            sum = sum + Si;
        }
        out[i] = sum;
    }
}
void dut(int a[128]) {
    int temp[128];
    int i, j, k;

    for (i = 0; i < 64; i++) { // {"temp": {"dim": 1, "type": "cyclic", "factor": 0}, "a": {"dim": 1, "type": "cyclic", "factor": 0}}
        temp[i] = a[i] + i;
    }

    for (j = 0; j < 64; j++) { // {"temp": {"dim": 1, "type": "cyclic", "factor": 0}, "a": {"dim": 1, "type": "cyclic", "factor": 0}}
        temp[127 - j] = a[j + 64] + j;
    }

    i = 0;
    j = 127;

    for (k = 0; k < 128; k++) { // {"a": {"dim": 1, "type": "cyclic", "factor": 0}}
        int tmp_j = temp[j];
        int tmp_i = temp[i];
        if (tmp_j < tmp_i) {
            a[k] = tmp_j + k;
            j--;
        } else {
            a[k] = tmp_i - k;
            i++;
        }
    }
}
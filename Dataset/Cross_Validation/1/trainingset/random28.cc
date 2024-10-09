void dut(int dlt, int *dlti, int *bli) {
    int i, wd2, wd3;
    /*if dlt is zero, then no sum into bli */
    if (dlt == 0) {
        for (i = 0; i < 128; i++) { // {"bli": {"dim": 1, "type": "cyclic", "factor": 0}}
            bli[i] = (int)((255L * bli[i]) >> 8L); 
        }
    }
    else {
        for (i = 0; i < 128; i++) { // {"bli": {"dim": 1, "type": "cyclic", "factor": 0}, "dlti": {"dim": 1, "type": "cyclic", "factor": 0}}
            if ((long)dlt * dlti[i] >= 0)
                wd2 = 128;
            else
                wd2 = -128;
            wd3 = (int)((255L * bli[i]) >> 8L); 
            bli[i] = wd2 + wd3;
        }
    }
    for(i = 1;i < 128; i++)
        dlti[i] = dlti[i-1];
    dlti[0] = dlt;
}
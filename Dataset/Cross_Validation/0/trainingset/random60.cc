int dut(int x[])
{
    // most straightforward implementation
    // actually not bad on FPGA
    int cnt = 0;
    for (int i = 0; i < 256; i++) { // {"x": {"dim": 1, "type": "cyclic", "factor": 0}}
        cnt = cnt + x[i];
    }

    return cnt;
}
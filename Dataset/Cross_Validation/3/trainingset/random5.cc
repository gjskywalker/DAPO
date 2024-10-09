void dut(int real[256], int img[256], int real_twid[128], int img_twid[128]) {
    int even, odd, span = 256, log, rootindex;
    int temp;
    log = 0;
    int i;
    for (i = 0; i < 11; i++) { // SKIP_DSE
        for (odd = 0; odd < 256; odd++) { // {"real": {"dim": 1, "type": "cyclic", "factor": 0}, "even": {"dim": 1, "type": "cyclic", "factor": 0}}
            odd |= span;
            even = odd ^ span;

            temp = real[even] + real[odd];
            real[odd] = real[even] - real[odd];
            real[even] = temp;

            temp = img[even] + img[odd];
            img[odd] = img[even] - img[odd];
            img[even] = temp;

            rootindex = (even << log) & (256 - 1);
            if (rootindex) {
                temp = real_twid[rootindex] * real[odd] - img_twid[rootindex] * img[odd];
                img[odd] = real_twid[rootindex] * img[odd] + img_twid[rootindex] * real[odd];
                real[odd] = temp;
            }
        }
        span >>= 1;
        log++;
    }
}

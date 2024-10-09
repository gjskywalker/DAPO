int dut(int statemt[32]) {
    int word[4][120];
    int ret[32], i, j;
     int x;

    for (j = 0; j < 8; j++) { // {"statemt": {"dim": 1, "type": "block", "factor": 8}, "word": {"dim": 2, "type": "cyclic", "factor": 0}}
        statemt[j * 4] ^= word[0][j + 8 * 14];
        statemt[1 + j * 4] ^= word[1][j + 8 * 14];
        statemt[2 + j * 4] ^= word[2][j + 8 * 14];
        statemt[3 + j * 4] ^= word[3][j + 8 * 14];
    }
    for (j = 0; j < 8; j++) { // {"ret": {"dim": 1, "type": "block", "factor": 8}, "statemt": {"dim": 1, "type": "block", "factor": 8}}
        for (i = 0; i < 4; i++) { // {"ret": {"dim": 1, "type": "cyclic", "factor": 0}, "statemt": {"dim": 1, "type": "cyclic", "factor": 0}}
            x = (statemt[i + j * 4] << 1);
            if ((x >> 8) == 1)
                x ^= 283;
            x ^= statemt[i + j * 4];
            x = (x << 1);
            if ((x >> 8) == 1)
                x ^= 283;
            x ^= statemt[i + j * 4];
            x = (x << 1);
            if ((x >> 8) == 1)
                x ^= 283;
            ret[i + j * 4] = x;

            x = (statemt[(i + 1) % 4 + j * 4] << 1);
            if ((x >> 8) == 1)
                x ^= 283;
            x = (x << 1);
            if ((x >> 8) == 1)
                x ^= 283;
            x ^= statemt[(i + 1) % 4 + j * 4];
            x = (x << 1);
            if ((x >> 8) == 1)
                x ^= 283;
            x ^= statemt[(i + 1) % 4 + j * 4];
            ret[i + j * 4] ^= x;

            x = (statemt[(i + 2) % 4 + j * 4] << 1);
            if ((x >> 8) == 1)
                x ^= 283;
            x ^= statemt[(i + 2) % 4 + j * 4];
            x = (x << 1);
            if ((x >> 8) == 1)
                x ^= 283;
            x = (x << 1);
            if ((x >> 8) == 1)
                x ^= 283;
            x ^= statemt[(i + 2) % 4 + j * 4];
            ret[i + j * 4] ^= x;

            x = (statemt[(i + 3) % 4 + j * 4] << 1);
            if ((x >> 8) == 1)
                x ^= 283;
            x = (x << 1);
            if ((x >> 8) == 1)
                x ^= 283;
            x = (x << 1);
            if ((x >> 8) == 1)
                x ^= 283;
            x ^= statemt[(i + 3) % 4 + j * 4];
            ret[i + j * 4] ^= x;
        }
    }
    for (i = 0; i < 8; i++) { // {"ret": {"dim": 1, "type": "block", "factor": 8}, "statemt": {"dim": 1, "type": "block", "factor": 8}}
        statemt[i * 4] = ret[i * 4];
        statemt[1 + i * 4] = ret[1 + i * 4];
        statemt[2 + i * 4] = ret[2 + i * 4];
        statemt[3 + i * 4] = ret[3 + i * 4];
    }
    return 0;
}
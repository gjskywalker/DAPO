void dut(int *buf) {
    int i = 0, j, a, b, c, d, e;

    for (j = 0; j < 4; j++) { // {"buf": {"dim": 1, "type": "cyclic", "factor": 4}}
        a = buf[i];
        b = buf[i + 1];
        c = buf[i + 2];
        d = buf[i + 3];
        e = a ^ b ^ c ^ d;
        buf[i] ^= e ^ (int)(a ^ b & 0x80) ? (int)((a ^ b << 1) ^ 0x1b) : (int)(a ^ b << 1);
        buf[i + 1] ^= e ^ (int)(b ^ c & 0x80) ? (int)((b ^ c << 1) ^ 0x1b) : (int)(b ^ c << 1);
        buf[i + 2] ^= e ^ (int)(c ^ d & 0x80) ? (int)((c ^ d << 1) ^ 0x1b) : (int)(c ^ d << 1);
        buf[i + 3] ^= e ^ (int)(d ^ a & 0x80) ? (int)((d ^ a << 1) ^ 0x1b) : (int)(d ^ a << 1);
        i += 4;
    }
}
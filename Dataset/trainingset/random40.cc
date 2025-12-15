void dut(int x[64], int y[64]) {
     int i;
     int *aptr;
     int a0, a1, a2, a3;
     int b0, b1, b2, b3;
     int c0, c1, c2, c3;

    /* Loop over columns */

    for (i = 0; i < 8; i++) { // {"x": {"dim": 1, "type": "cyclic", "factor": 0}, "y": {"dim": 1, "type": "cyclic", "factor": 0}}
        aptr = x + i;
        b0 = ((*aptr) << 2);
        aptr += 8;
        a0 = ((*aptr) << 2);
        aptr += 8;
        b2 = ((*aptr) << 2);
        aptr += 8;
        a1 = ((*aptr) << 2);
        aptr += 8;
        b1 = ((*aptr) << 2);
        aptr += 8;
        a2 = ((*aptr) << 2);
        aptr += 8;
        b3 = ((*aptr) << 2);
        aptr += 8;
        a3 = ((*aptr) << 2);

        /* Split into even mode  b0 = x0  b1 = x4  b2 = x2  b3 = x6.
       And the odd terms a0 = x1 a1 = x3 a2 = x5 a3 = x7.
       */

        c0 = ((100L * a0) - (502L * a3)) >> 9;
        c1 = ((426L * a2) - (284L * a1)) >> 9;
        c2 = ((426L * a1) + (284L * a2)) >> 9;
        c3 = ((502L * a0) + (100L * a3)) >> 9;

        /* First Butterfly on even terms.*/

        a0 = (362L * (b0 + b1)) >> 9;
        a1 = (362L * (b0 - b1)) >> 9;

        a2 = ((196L * b2) - (473L * b3)) >> 9;
        a3 = ((473L * b2) + (196L * b3)) >> 9;

        b0 = a0 + a3;
        b1 = a1 + a2;
        b2 = a1 - a2;
        b3 = a0 - a3;

        /* Second Butterfly */

        a0 = c0 + c1;
        a1 = c0 - c1;
        a2 = c3 - c2;
        a3 = c3 + c2;

        c0 = a0;
        c1 = (362L * (a2 - a1)) >> 9;
        c2 = (362L * (a2 + a1)) >> 9;
        c3 = a3;

        aptr = y + i;
        *aptr = b0 + c3;
        aptr += 8;
        *aptr = b1 + c2;
        aptr += 8;
        *aptr = b2 + c1;
        aptr += 8;
        *aptr = b3 + c0;
        aptr += 8;
        *aptr = b3 - c0;
        aptr += 8;
        *aptr = b2 - c1;
        aptr += 8;
        *aptr = b1 - c2;
        aptr += 8;
        *aptr = b0 - c3;
    }

    /* Loop over rows */

    for (i = 0; i < 8; i++) { // {"x": {"dim": 1, "type": "block", "factor": 8}, "y": {"dim": 1, "type": "block", "factor": 8}}
        aptr = y + (i << 3);
        b0 = *(aptr++);
        a0 = *(aptr++);
        b2 = *(aptr++);
        a1 = *(aptr++);
        b1 = *(aptr++);
        a2 = *(aptr++);
        b3 = *(aptr++);
        a3 = *(aptr);

        /*
      Split into even mode  b0 = x0  b1 = x4  b2 = x2  b3 = x6.
      And the odd terms a0 = x1 a1 = x3 a2 = x5 a3 = x7.
      */

        c0 = ((100L * a0) - (502L * a3)) >> 9;
        c1 = ((426L * a2) - (284L * a1)) >> 9;
        c2 = ((426L * a1) + (284L * a2)) >> 9;
        c3 = ((502L * a0) + (100L * a3)) >> 9;

        /* First Butterfly on even terms.*/

        a0 = (362L * (b0 + b1)) >> 9;
        a1 = (362L * (b0 - b1)) >> 9;

        a2 = ((196L * b2) - (473L * b3)) >> 9;
        a3 = ((473L * b2) + (196L * b3)) >> 9;

        /* Calculate last set of b's */

        b0 = a0 + a3;
        b1 = a1 + a2;
        b2 = a1 - a2;
        b3 = a0 - a3;

        /* Second Butterfly */

        a0 = c0 + c1;
        a1 = c0 - c1;
        a2 = c3 - c2;
        a3 = c3 + c2;

        c0 = a0;
        c1 = (362L * (a2 - a1)) >> 9;
        c2 = (362L * (a2 + a1)) >> 9;
        c3 = a3;

        aptr = y + (i << 3);
        *(aptr++) = b0 + c3;
        *(aptr++) = b1 + c2;
        *(aptr++) = b2 + c1;
        *(aptr++) = b3 + c0;
        *(aptr++) = b3 - c0;
        *(aptr++) = b2 - c1;
        *(aptr++) = b1 - c2;
        *(aptr) = b0 - c3;
    }

    /*
      Retrieve correct accuracy. We have additional factor
      of 16 that must be removed.
     */
    aptr = y;
    for (i = 0; i < 64; i++) { // {"y": {"dim": 1, "type": "cyclic", "factor": 0}}
        *aptr = (((*aptr < 0) ? (*aptr - 8) : (*aptr + 8)) / 16);
        aptr++;
    }
}
/* generated on Sat Jun 08 */
/* This test set comes from MachSuite */
/* This test comes from MachSuite/md/nw */

#define ALEN 128
#define BLEN 128

#define MATCH_SCORE 1
#define MISMATCH_SCORE -1
#define GAP_SCORE -1

#define ALIGN '\\'
#define SKIPA '^'
#define SKIPB '<'

void dut(char SEQA[ALEN], char SEQB[BLEN],
         char alignedA[ALEN + BLEN], char alignedB[ALEN + BLEN],
         int M[(ALEN + 1) * (BLEN + 1)], char ptr[(ALEN + 1) * (BLEN + 1)])
{

    int score, up_left, up, left, max;
    int row, row_up, r;
    int a_idx, b_idx;
    int a_str_idx, b_str_idx;

    for (a_idx = 0; a_idx < (ALEN + 1); a_idx++)
    {
        M[a_idx] = a_idx * GAP_SCORE;
    }
    for (b_idx = 0; b_idx < (BLEN + 1); b_idx++)
    {
        M[b_idx * (ALEN + 1)] = b_idx * GAP_SCORE;
    }

    for (b_idx = 1; b_idx < (BLEN + 1); b_idx++)
    {
        for (a_idx = 1; a_idx < (ALEN + 1); a_idx++)
        {
            if (SEQA[a_idx - 1] == SEQB[b_idx - 1])
            {
                score = MATCH_SCORE;
            }
            else
            {
                score = MISMATCH_SCORE;
            }

            row_up = (b_idx - 1) * (ALEN + 1);
            row = (b_idx) * (ALEN + 1);

            up_left = M[row_up + (a_idx - 1)] + score;
            up = M[row_up + (a_idx)] + GAP_SCORE;
            left = M[row + (a_idx - 1)] + GAP_SCORE;

            if (up_left > up)
            {
                if (up_left > left)
                {
                    max = up_left;
                }
            }
            else if (up > left)
            {
                max = up;
            }
            else if (up < left)
            {
                max = left;
            }

            M[row + a_idx] = max;
            if (max == left)
            {
                ptr[row + a_idx] = SKIPB;
            }
            else if (max == up)
            {
                ptr[row + a_idx] = SKIPA;
            }
            else
            {
                ptr[row + a_idx] = ALIGN;
            }
        }
    }

    // TraceBack (n.b. aligned sequences are backwards to avoid string appending)
    a_idx = ALEN;
    b_idx = BLEN;
    a_str_idx = 0;
    b_str_idx = 0;

    // while (a_idx > 0 || b_idx > 0)
    // {
    //     r = b_idx * (ALEN + 1);
    //     if (ptr[r + a_idx] == ALIGN)
    //     {
    //         alignedA[a_str_idx++] = SEQA[a_idx - 1];
    //         alignedB[b_str_idx++] = SEQB[b_idx - 1];
    //         a_idx--;
    //         b_idx--;
    //     }
    //     else if (ptr[r + a_idx] == SKIPB)
    //     {
    //         alignedA[a_str_idx++] = SEQA[a_idx - 1];
    //         alignedB[b_str_idx++] = '-';
    //         a_idx--;
    //     }
    //     else
    //     { // SKIPA
    //         alignedA[a_str_idx++] = '-';
    //         alignedB[b_str_idx++] = SEQB[b_idx - 1];
    //         b_idx--;
    //     }
    // }

    // Pad the result

    for (; a_str_idx < ALEN + BLEN; a_str_idx++)
    {
        alignedA[a_str_idx] = '_';
    }

    for (; b_str_idx < ALEN + BLEN; b_str_idx++)
    {
        alignedB[b_str_idx] = '_';
    }
}
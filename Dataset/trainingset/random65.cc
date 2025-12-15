/* generated on Sat Jun 08 */
/* This test set comes from MachSuite */
/* This test comes from MachSuite/kmp */

#define PATTERN_SIZE 4
#define STRING_SIZE 32411

int dut(char pattern[PATTERN_SIZE], char input[STRING_SIZE], int kmpNext[PATTERN_SIZE], int n_matches)
{
    int i, q;
    n_matches = 0;

    int k;
    k = 0;
    kmpNext[0] = 0;

    for (q = 1; q < PATTERN_SIZE; q++)
    {
        if (pattern[k] == pattern[q])
        {
            k++;
        }
        kmpNext[q] = k;
    }

    q = 0;

    for (i = 0; i < STRING_SIZE; i++)
    {
        if (pattern[q] == input[i])
        {
            q++;
        }
        if (q >= PATTERN_SIZE)
        {
            n_matches++;
            q = kmpNext[q - 1];
        }
    }
    return 0;
}
/* generated on Tue Jun 11 */
/* This test set comes from Rosetta */
/* This test comes from Rosetta/BNN */
#include <stdio.h>
#include <stdlib.h>

typedef int TwoBit;
typedef int Bit;
typedef int ConvOut;

const unsigned WORD_SIZE = 64;
const unsigned BANK_WIDTH = 8;
const unsigned CONV_BANKS = WORD_SIZE / BANK_WIDTH;
const unsigned CONV_ROWS = 3;
const unsigned CONV_COLS = BANK_WIDTH + 2;

// ConvOut conv3x3b(
//     const TwoBit line_buffer_m[CONV_BANKS][CONV_ROWS][CONV_COLS],
//     const Bit conv_params_m[3][3],
//     const int bank,
//     const int cc)
// {
//     ConvOut sum = 0;
//     for (int kr = 0; kr < 3; ++kr)
//     {
//         for (int kc = 0; kc < 3; ++kc)
//         {
//             TwoBit data = line_buffer_m[bank][kr][cc + kc];
//             const Bit &wt = conv_params_m[2 - kr][2 - kc];
//             data = (wt & data) ^ data;
//             sum += data;
//         }
//     }
//     return sum;
// }

void dut(
    const TwoBit line_buffer_m[CONV_BANKS][CONV_ROWS][CONV_COLS],
    const Bit conv_params_m[3][3],
    ConvOut conv_out_buffer_m[WORD_SIZE])
{
    for (unsigned bank = 0; bank < CONV_BANKS; ++bank)
    {
        for (unsigned cc = 0; cc < BANK_WIDTH; ++cc)
        {
            conv_out_buffer_m[bank * BANK_WIDTH + cc] = 0;
            for (int kr = 0; kr < 3; ++kr)
            {
                for (int kc = 0; kc < 3; ++kc)
                {
                    TwoBit data = line_buffer_m[bank][kr][cc + kc];
                    const Bit &wt = conv_params_m[2 - kr][2 - kc];
                    data = (wt & data) ^ data;
                    conv_out_buffer_m[bank * BANK_WIDTH + cc] += data;
                }
            }
            // conv_out_buffer_m[bank * BANK_WIDTH + cc] = conv3x3b(line_buffer_m, conv_params_m, bank, cc);
        }
    }
}

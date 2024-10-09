/* generated on Tue Jun 11 */
/* This test set comes from Rosetta */
/* This test comes from Rosetta/digit-recognition */
#include <stdio.h>
#include <stdlib.h>

#define NUM_TEST 2000
#define K_CONST 3
#define PAR_FACTOR 40
#define NUM_TRAINING 18000

typedef int WholeDigitType;
typedef unsigned long long DigitType;
typedef unsigned char LabelType;

// int popcount(WholeDigitType x[256])
// {
//     // most straightforward implementation
//     // actually not bad on FPGA
//     int cnt = 0;
//     for (int i = 0; i < 256; i++)
//         cnt = cnt + x[i];

//     return cnt;
// }

void dut(WholeDigitType test_inst[256], WholeDigitType train_inst[256], int min_distances[K_CONST])
{
    // Compute the difference using XOR
    WholeDigitType diff[256];
    for (int i = 0; i < 256; i++)
    {
        diff[i] = test_inst[i] ^ train_inst[i];
    }

    int dist = 0;

    for (int i=0; i < 256; i++)
        dist = dist + diff[i];

    // dist = popcount(diff);

    int max_dist = 0;
    int max_dist_id = K_CONST + 1;
    int k = 0;

// Find the max distance
FIND_MAX_DIST:
    for (int k = 0; k < K_CONST; ++k)
    {
        if (min_distances[k] > max_dist)
        {
            max_dist = min_distances[k];
            max_dist_id = k;
        }
    }

    // Replace the entry with the max distance
    if (dist < max_dist)
        min_distances[max_dist_id] = dist;

    return;
}
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

LabelType dut(int knn_set[PAR_FACTOR * K_CONST])
{

    // local buffers

    // final K nearest neighbors
    int min_distance_list[K_CONST];
    // labels for the K nearest neighbors
    int label_list[K_CONST];
    // voting boxes
    int vote_list[10];

    int pos = 1000;

// initialize
INIT_1:
    for (int i = 0; i < K_CONST; i++)
    {
        min_distance_list[i] = 256;
        label_list[i] = 9;
    }

INIT_2:
    for (int i = 0; i < 10; i++)
    {
        vote_list[i] = 0;
    }

// go through all the lanes
// do an insertion sort to keep a sorted neighbor list
LANES:
    for (int i = 0; i < PAR_FACTOR; i++)
    {
    INSERTION_SORT_OUTER:
        for (int j = 0; j < K_CONST; j++)
        {
            pos = 1000;
        INSERTION_SORT_INNER:
            for (int r = 0; r < K_CONST; r++)
            {
                pos = ((knn_set[i * K_CONST + j] < min_distance_list[r]) && (pos > K_CONST)) ? r : pos;
            }

        INSERT:
            for (int r = K_CONST; r > 0; r--)
            {
                if (r - 1 > pos)
                {
                    min_distance_list[r - 1] = min_distance_list[r - 2];
                    label_list[r - 1] = label_list[r - 2];
                }
                else if (r - 1 == pos)
                {
                    min_distance_list[r - 1] = knn_set[i * K_CONST + j];
                    label_list[r - 1] = i / (PAR_FACTOR / 10);
                }
            }
        }
    }

// vote
INCREMENT:
    for (int i = 0; i < K_CONST; i++)
    {
        vote_list[label_list[i]] += 1;
    }

    LabelType max_vote;
    max_vote = 0;

// find the maximum value
VOTE:
    for (int i = 0; i < 10; i++)
    {
        if (vote_list[i] >= vote_list[max_vote])
        {
            max_vote = i;
        }
    }

    return max_vote;
}
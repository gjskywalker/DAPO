/* generated on Tue Jun 11 */
/* This test set comes from Rosetta */
/* This test comes from Rosetta/optical-flow/src/sdsoc/optical-flow.cpp */
#include <stdio.h>
#include <stdlib.h>

const int MAX_HEIGHT = 436;
const int MAX_WIDTH = 1024;

// define these constants so they can be used in pragma
const int max_width = MAX_WIDTH;
const int default_depth = MAX_WIDTH;

void dut(double frame1[MAX_HEIGHT][MAX_WIDTH],
         double frame2[MAX_HEIGHT][MAX_WIDTH],
         double frame3[MAX_HEIGHT][MAX_WIDTH],
         double frame4[MAX_HEIGHT][MAX_WIDTH],
         double frame5[MAX_HEIGHT][MAX_WIDTH],
         double gradient_z[MAX_HEIGHT][MAX_WIDTH])
{
    const int GRAD_WEIGHTS[] = {1, -8, 0, 8, -1};
GRAD_Z_OUTER:
    for (int r = 0; r < MAX_HEIGHT; r++)
    {
    GRAD_Z_INNER:
        for (int c = 0; c < MAX_WIDTH; c++)
        {
            gradient_z[r][c] = ((double)(frame1[r][c] * GRAD_WEIGHTS[0] + frame2[r][c] * GRAD_WEIGHTS[1] + frame3[r][c] * GRAD_WEIGHTS[2] + frame4[r][c] * GRAD_WEIGHTS[3] + frame5[r][c] * GRAD_WEIGHTS[4])) / 12;
        }
    }
}
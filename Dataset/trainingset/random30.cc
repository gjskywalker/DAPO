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

// calculate gradient in x and y directions
void dut(double frame[MAX_HEIGHT][MAX_WIDTH],
         double gradient_x[MAX_HEIGHT][MAX_WIDTH],
         double gradient_y[MAX_HEIGHT][MAX_WIDTH])
{
    // our own line buffer
    double buf[5][MAX_WIDTH];

    // small buffer
    double smallbuf[5];

    int GRAD_WEIGHTS[] = {1, -8, 0, 8, -1};

GRAD_XY_OUTER:
    for (int r = 0; r < MAX_HEIGHT + 2; r++)
    {
    GRAD_XY_INNER:
        for (int c = 0; c < MAX_WIDTH + 2; c++)
        {
            // read out values from current line buffer
            for (int i = 0; i < 4; i++)
                smallbuf[i] = buf[i + 1][c];
            // the new value is either 0 or read from frame
            if (r < MAX_HEIGHT && c < MAX_WIDTH)
                smallbuf[4] = (double)(frame[r][c]);
            else if (c < MAX_WIDTH)
                smallbuf[4] = 0;
            // update line buffer
            if (r < MAX_HEIGHT && c < MAX_WIDTH)
            {
                for (int i = 0; i < 4; i++)
                    buf[i][c] = smallbuf[i];
                buf[4][c] = smallbuf[4];
            }
            else if (c < MAX_WIDTH)
            {
                for (int i = 0; i < 4; i++)
                    buf[i][c] = smallbuf[i];
                buf[4][c] = smallbuf[4];
            }

            // compute gradient
            double x_grad = 0;
            double y_grad = 0;
            if (r >= 4 && r < MAX_HEIGHT && c >= 4 && c < MAX_WIDTH)
            {
            GRAD_XY_XYGRAD:
                for (int i = 0; i < 5; i++)
                {
                    x_grad += GRAD_WEIGHTS[i];
                    y_grad += GRAD_WEIGHTS[i];
                }
                gradient_x[r - 2][c - 2] = x_grad / 12;
                gradient_y[r - 2][c - 2] = y_grad / 12;
            }
            else if (r >= 2 && c >= 2)
            {
                gradient_x[r - 2][c - 2] = 0;
                gradient_y[r - 2][c - 2] = 0;
            }
        }
    }
}
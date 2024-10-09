/* generated on Fri Jun 07*/
/* This test set comes from HLS-benchmarks */
/* This test comes from HLS-benchmarks/Inter-Block/los */

void dut(int obstacles_0[1024], int obstacles_1[1024],
         int obstacles_2[1024], int obstacles_3[1024],
         int obstacles_4[1024], int obstacles_5[1024],
         int obstacles_6[1024], int obstacles_7[1024],
         int results_0[1024], int results_1[1024],
         int results_2[1024], int results_3[1024],
         int results_4[1024], int results_5[1024],
         int results_6[1024], int results_7[1024])
{
    for (int y_pixel = 0; y_pixel < 16; y_pixel++)
    {
        for (int x_pixel = 0; x_pixel < 64; x_pixel++)
        {
            int sight_0 = 1;
            int x0 = x_pixel;
            int y0 = y_pixel;
            int sx, dx, sy, dy;
            if (x0 < 32)
            {
                sx = 1;
                dx = 32 - x0;
            }
            else
            {
                sx = -1;
                dx = x0 - 32;
            }

            if (y0 < 32)
            {
                sy = 1;
                dy = 32 - y0;
            }
            else
            {
                sy = -1;
                dy = y0 - 32;
            }

            int err = dx - dy;

            if (sight_0 == 1)
                results_0[y_pixel * 64 + x_pixel] = 1;
        }
    }


    for (int y_pixel = 0; y_pixel < 16; y_pixel++)
    {
        for (int x_pixel = 0; x_pixel < 64; x_pixel++)
        {
            int sight_1 = 1;
            int x0 = x_pixel;
            int y0 = y_pixel;
            int sx, dx, sy, dy;
            if (x0 < 32)
            {
                sx = 1;
                dx = 32 - x0;
            }
            else
            {
                sx = -1;
                dx = x0 - 32;
            }

            if (y0 < 32)
            {
                sy = 1;
                dy = 32 - y0;
            }
            else
            {
                sy = -1;
                dy = y0 - 32;
            }

            int err = dx - dy;

            if (sight_1 == 1)
                results_1[y_pixel * 64 + x_pixel] = 1;
        }
    }


    for (int y_pixel = 0; y_pixel < 16; y_pixel++)
    {
        for (int x_pixel = 0; x_pixel < 64; x_pixel++)
        {
            int sight_2 = 1;
            int x0 = x_pixel;
            int y0 = y_pixel;
            int sx, dx, sy, dy;
            if (x0 < 32)
            {
                sx = 1;
                dx = 32 - x0;
            }
            else
            {
                sx = -1;
                dx = x0 - 32;
            }

            if (y0 < 32)
            {
                sy = 1;
                dy = 32 - y0;
            }
            else
            {
                sy = -1;
                dy = y0 - 32;
            }

            int err = dx - dy;


            if (sight_2 == 1)
                results_2[y_pixel * 64 + x_pixel] = 1;
        }
    }


    for (int y_pixel = 0; y_pixel < 16; y_pixel++)
    {
        for (int x_pixel = 0; x_pixel < 64; x_pixel++)
        {
            int sight_3 = 1;
            int x0 = x_pixel;
            int y0 = y_pixel;
            int sx, dx, sy, dy;
            if (x0 < 32)
            {
                sx = 1;
                dx = 32 - x0;
            }
            else
            {
                sx = -1;
                dx = x0 - 32;
            }

            if (y0 < 32)
            {
                sy = 1;
                dy = 32 - y0;
            }
            else
            {
                sy = -1;
                dy = y0 - 32;
            }

            int err = dx - dy;


            if (sight_3 == 1)
                results_3[y_pixel * 64 + x_pixel] = 1;
        }
    }


    for (int y_pixel = 0; y_pixel < 16; y_pixel++)
    {
        for (int x_pixel = 0; x_pixel < 64; x_pixel++)
        {
            int sight_4 = 1;
            int x0 = x_pixel;
            int y0 = y_pixel;
            int sx, dx, sy, dy;
            if (x0 < 32)
            {
                sx = 1;
                dx = 32 - x0;
            }
            else
            {
                sx = -1;
                dx = x0 - 32;
            }

            if (y0 < 32)
            {
                sy = 1;
                dy = 32 - y0;
            }
            else
            {
                sy = -1;
                dy = y0 - 32;
            }

            int err = dx - dy;


            if (sight_4 == 1)
                results_4[y_pixel * 64 + x_pixel] = 1;
        }
    }


    for (int y_pixel = 0; y_pixel < 16; y_pixel++)
    {
        for (int x_pixel = 0; x_pixel < 64; x_pixel++)
        {
            int sight_5 = 1;
            int x0 = x_pixel;
            int y0 = y_pixel;
            int sx, dx, sy, dy;
            if (x0 < 32)
            {
                sx = 1;
                dx = 32 - x0;
            }
            else
            {
                sx = -1;
                dx = x0 - 32;
            }

            if (y0 < 32)
            {
                sy = 1;
                dy = 32 - y0;
            }
            else
            {
                sy = -1;
                dy = y0 - 32;
            }

            int err = dx - dy;


            if (sight_5 == 1)
                results_5[y_pixel * 64 + x_pixel] = 1;
        }
    }


    for (int y_pixel = 0; y_pixel < 16; y_pixel++)
    {
        for (int x_pixel = 0; x_pixel < 64; x_pixel++)
        {
            int sight_6 = 1;
            int x0 = x_pixel;
            int y0 = y_pixel;
            int sx, dx, sy, dy;
            if (x0 < 32)
            {
                sx = 1;
                dx = 32 - x0;
            }
            else
            {
                sx = -1;
                dx = x0 - 32;
            }

            if (y0 < 32)
            {
                sy = 1;
                dy = 32 - y0;
            }
            else
            {
                sy = -1;
                dy = y0 - 32;
            }

            int err = dx - dy;

            if (sight_6 == 1)
                results_6[y_pixel * 64 + x_pixel] = 1;
        }
    }


    for (int y_pixel = 0; y_pixel < 16; y_pixel++)
    {
        for (int x_pixel = 0; x_pixel < 64; x_pixel++)
        {
            int sight_7 = 1;
            int x0 = x_pixel;
            int y0 = y_pixel;
            int sx, dx, sy, dy;
            if (x0 < 32)
            {
                sx = 1;
                dx = 32 - x0;
            }
            else
            {
                sx = -1;
                dx = x0 - 32;
            }

            if (y0 < 32)
            {
                sy = 1;
                dy = 32 - y0;
            }
            else
            {
                sy = -1;
                dy = y0 - 32;
            }

            int err = dx - dy;


            if (sight_7 == 1)
                results_7[y_pixel * 64 + x_pixel] = 1;
        }
    }
}
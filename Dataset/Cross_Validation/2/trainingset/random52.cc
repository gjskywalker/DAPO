/* generated on Fri Jun 07*/
/* This test set comes from HLS-benchmarks */
/* This test comes from HLS-benchmarks/DSS/gSumIf */

double dut(double A[1000], double B[1000])
{

    double d = 0.0;
    double s = 0.0;
    int i;

    for (i = 0; i < 1000; i++)
    {
        d = A[i] + B[i];
        if (d >= 0)
        {
            // An if condition in the loop causes irregular computation.
            // Static scheduler reserves time slot for each iteration
            // causing unnecessary pipeline stalls.
            if (i > 5)
            {
                s = (((((d + 0.25) * d + 0.5) * d + 0.125) * d + 0.25) * d + 0.5) * d + 0.25;
            }
            else
            {
                s = (((((d + 0.64) * d + 0.7) * d + 0.21) * d + 0.33) * d + 0.25) * d + 0.125;
            }
        }
    }
    return s;
}
/* generated on Fri Jun 07*/
/* This test set comes from HLS-benchmarks */
/* This test comes from HLS-benchmarks/DSS/histogram */

int dut(int f[1000], double w[1000], double hist[1000])
{
    for (int i = 0; i < 1000; ++i)
    {
        double temp = w[i];
        if (temp >= 0)
        {
            double x = hist[f[i]];
            // An if condition in the loop causes irregular computation.
            // Static scheduler reserves time slot for each iteration
            // causing unnecessary pipeline stalls.
            hist[f[i]] +=  temp;
        }
    }
    return 0;
}
/* generated on Fri Jun 07*/
/* This test set comes from HLS-benchmarks */
/* This test comes from HLS-benchmarks/DSS/sparseMatrixPower */

void dut(int data[200], int all_zero[100], int w[10000])
{
    int j, i, temp;

    for (j = 0; j < 100; j++)
    {

        temp = all_zero[j];
        if (temp < 10)
        {
            // An if condition in the loop causes irregular computation.
            // Static scheduler reserves time slot for each iteration
            // causing unnecessary pipeline stalls.
            for (i = 0; i < 100; i++)
            {
                data[i + 100] += w[i * 100 + j] * data[i];
            }
        }
    }
}
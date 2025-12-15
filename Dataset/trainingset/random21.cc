/* generated on Sat Jun 08 */
/* This test set comes from HLS-benchmarks */
/* This test comes from HLS-benchmarks/PNAnalyser/vecTrans */

void dut(int A[1024], int b[1024])
{
    for (int i = 0; i < 1000; i++)
    {
        int d = A[i];
        A[b[i]] = (((((((d + 112) * d + 23) * d + 36) * d + 82) * d + 127) * d + 2) * d + 20) * d + 100; // need to change the mul to add
    }
}
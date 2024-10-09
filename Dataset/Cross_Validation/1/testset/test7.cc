/* generated on Sat Jun 08 */
/* This test set comes from MachSuite */
/* This test comes from MachSuite/sort/merge */

#define SIZE 2048
#define TYPE int

void dut(TYPE a[SIZE])
{
    TYPE temp[SIZE];
    int i, j, k;

merge_label1:
    for (i = 0; i <= 128; i++)
    {
        temp[i] = a[i];
    }

merge_label2:
    for (j = 128 + 1; j <= 256; j++)
    {
        temp[128 + 1 + 256 - j] = a[j];
    }

    i = 0;
    j = 256;

merge_label3:
    for (k = 0; k <= 256; k++)
    {
        TYPE tmp_j = temp[j];
        TYPE tmp_i = temp[i];
        if (tmp_j < tmp_i)
        {
            a[k] = tmp_j;
            j--;
        }
        else
        {
            a[k] = tmp_i;
            i++;
        }
    }
}
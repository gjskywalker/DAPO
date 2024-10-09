void dut(int bucket[64], int a[64], int exp)
{
    int blockID, i, bucket_indx, a_indx;
    blockID = 0;
    for (blockID = 0; blockID < 16; blockID++) {
        for(i = 0; i < 4; i++) {
            a_indx = blockID * 4 + i;
            bucket_indx = ((a[a_indx] >> exp) & 0x3)*16 + blockID + 1;
            if (a[a_indx] % 2 == 0) {
                bucket[bucket_indx] += 2;
            } else {
                bucket[bucket_indx] += 1;
            }
        }
    }
}

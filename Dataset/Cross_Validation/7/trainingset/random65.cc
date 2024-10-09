void dut(int orig[256], int sol[256], int filter[9]){
    int r, c, k1, k2;
    int temp, mul;

    for (r=0; r<6; r++) { // {"orig": {"dim": 1, "type": "block", "factor": 8}, "sol": {"dim": 1, "type": "block", "factor": 32}}
        for (c=0; c<6; c++) { // {"orig": {"dim": 1, "type": "cyclic", "factor": 0}, "sol": {"dim": 1, "type": "cyclic", "factor": 0}}
            temp = (int)0;
            for (k1=0;k1<3;k1++){ // {"filter": {"dim": 1, "type": "block", "factor": 3}, "orig": {"dim": 1, "type": "block", "factor": 32}}
                for (k2=0;k2<3;k2++){ // {"filter": {"dim": 1, "type": "cyclic", "factor": 0}, "orig": {"dim": 1, "type": "cyclic", "factor": 0}}
                    mul = filter[k1*3 + k2] * orig[(r+k1)*32 + c+k2];
                    temp += mul;
                }
            }
            sol[(r*8) + c] = temp;
        }
    }
}
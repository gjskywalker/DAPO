typedef int in_int_t;
typedef int out_int_t;
typedef int inout_int_t;

#include <stdlib.h>

#define N 32768
#define BIN_MAX_NUM 20

void dut (in_int_t array_0[4096], in_int_t array_1[4096],
               in_int_t array_2[4096], in_int_t array_3[4096],
               in_int_t array_4[4096], in_int_t array_5[4096],
               in_int_t array_6[4096], in_int_t array_7[4096],
               inout_int_t results_0[5], inout_int_t results_1[5],
               inout_int_t results_2[5], inout_int_t results_3[5],
               inout_int_t results_4[5], inout_int_t results_5[5],
               inout_int_t results_6[5], inout_int_t results_7[5],
               inout_int_t results[5]) {

loop_0:
  for (int i = 0; i < 4096; i++) {
    int num = array_0[i];
    if (num > 0 && num <= BIN_MAX_NUM) {
      results_0[0] += 1;
    } else if (num > BIN_MAX_NUM && num <= (BIN_MAX_NUM * 2)) {
      results_0[1] += 1;
    } else if (num > (BIN_MAX_NUM * 2) && num <= (BIN_MAX_NUM * 3)) {
      results_0[2] += 1;
    } else if (num > (BIN_MAX_NUM * 3) && num <= (BIN_MAX_NUM * 4)) {
      results_0[3] += 1;
    } else {
      results_0[4] += 1;
    }
  }

loop_1:
  for (int i = 0; i < 4096; i++) {
    int num = array_1[i];
    if (num > 0 && num <= BIN_MAX_NUM) {
      results_1[0] += 1;
    } else if (num > BIN_MAX_NUM && num <= (BIN_MAX_NUM * 2)) {
      results_1[1] += 1;
    } else if (num > (BIN_MAX_NUM * 2) && num <= (BIN_MAX_NUM * 3)) {
      results_1[2] += 1;
    } else if (num > (BIN_MAX_NUM * 3) && num <= (BIN_MAX_NUM * 4)) {
      results_1[3] += 1;
    } else {
      results_1[4] += 1;
    }
  }

loop_2:
  for (int i = 0; i < 4096; i++) {
    int num = array_2[i];
    if (num > 0 && num <= BIN_MAX_NUM) {
      results_2[0] += 1;
    } else if (num > BIN_MAX_NUM && num <= (BIN_MAX_NUM * 2)) {
      results_2[1] += 1;
    } else if (num > (BIN_MAX_NUM * 2) && num <= (BIN_MAX_NUM * 3)) {
      results_2[2] += 1;
    } else if (num > (BIN_MAX_NUM * 3) && num <= (BIN_MAX_NUM * 4)) {
      results_2[3] += 1;
    } else {
      results_2[4] += 1;
    }
  }

loop_3:
  for (int i = 0; i < 4096; i++) {
    int num = array_3[i];
    if (num > 0 && num <= BIN_MAX_NUM) {
      results_3[0] += 1;
    } else if (num > BIN_MAX_NUM && num <= (BIN_MAX_NUM * 2)) {
      results_3[1] += 1;
    } else if (num > (BIN_MAX_NUM * 2) && num <= (BIN_MAX_NUM * 3)) {
      results_3[2] += 1;
    } else if (num > (BIN_MAX_NUM * 3) && num <= (BIN_MAX_NUM * 4)) {
      results_3[3] += 1;
    } else {
      results_3[4] += 1;
    }
  }

loop_4:
  for (int i = 0; i < 4096; i++) {
    int num = array_4[i];
    if (num > 0 && num <= BIN_MAX_NUM) {
      results_4[0] += 1;
    } else if (num > BIN_MAX_NUM && num <= (BIN_MAX_NUM * 2)) {
      results_4[1] += 1;
    } else if (num > (BIN_MAX_NUM * 2) && num <= (BIN_MAX_NUM * 3)) {
      results_4[2] += 1;
    } else if (num > (BIN_MAX_NUM * 3) && num <= (BIN_MAX_NUM * 4)) {
      results_4[3] += 1;
    } else {
      results_4[4] += 1;
    }
  }

loop_5:
  for (int i = 0; i < 4096; i++) {
    int num = array_5[i];
    if (num > 0 && num <= BIN_MAX_NUM) {
      results_5[0] += 1;
    } else if (num > BIN_MAX_NUM && num <= (BIN_MAX_NUM * 2)) {
      results_5[1] += 1;
    } else if (num > (BIN_MAX_NUM * 2) && num <= (BIN_MAX_NUM * 3)) {
      results_5[2] += 1;
    } else if (num > (BIN_MAX_NUM * 3) && num <= (BIN_MAX_NUM * 4)) {
      results_5[3] += 1;
    } else {
      results_5[4] += 1;
    }
  }

loop_6:
  for (int i = 0; i < 4096; i++) {
    int num = array_6[i];
    if (num > 0 && num <= BIN_MAX_NUM) {
      results_6[0] += 1;
    } else if (num > BIN_MAX_NUM && num <= (BIN_MAX_NUM * 2)) {
      results_6[1] += 1;
    } else if (num > (BIN_MAX_NUM * 2) && num <= (BIN_MAX_NUM * 3)) {
      results_6[2] += 1;
    } else if (num > (BIN_MAX_NUM * 3) && num <= (BIN_MAX_NUM * 4)) {
      results_6[3] += 1;
    } else {
      results_6[4] += 1;
    }
  }

loop_7:
  for (int i = 0; i < 4096; i++) {
    int num = array_7[i];
    if (num > 0 && num <= BIN_MAX_NUM) {
      results_7[0] += 1;
    } else if (num > BIN_MAX_NUM && num <= (BIN_MAX_NUM * 2)) {
      results_7[1] += 1;
    } else if (num > (BIN_MAX_NUM * 2) && num <= (BIN_MAX_NUM * 3)) {
      results_7[2] += 1;
    } else if (num > (BIN_MAX_NUM * 3) && num <= (BIN_MAX_NUM * 4)) {
      results_7[3] += 1;
    } else {
      results_7[4] += 1;
    }
  }

loop_8:
  for (int i = 0; i < 5; i++)
    results[i] = results_0[i] + results_1[i] + results_2[i] + results_3[i] +
                 results_4[i] + results_5[i] + results_6[i] + results_7[i];
}

#include <stdlib.h>
typedef int in_int_t;
typedef int out_int_t;
typedef int inout_int_t;
#define N 32768

out_int_t dut(in_int_t str_0[4096], in_int_t str_1[4096],
                    in_int_t str_2[4096], in_int_t str_3[4096],
                    in_int_t str_4[4096], in_int_t str_5[4096],
                    in_int_t str_6[4096], in_int_t str_7[4096]) {
  int s_0 = 0;
  int s_1 = 0;
  int s_2 = 0;
  int s_3 = 0;
  int s_4 = 0;
  int s_5 = 0;
  int s_6 = 0;
  int s_7 = 0;

loop_0:
  for (int i = 0; i < 4096; i++) {
    int j = 0;
    int match = 1;
    for (j = 0; j < 3; j++) {
      int k = i + j;
      int c;
      if (k < 4096)
        c = str_0[k];
      else
        c = str_1[k - 4096];
      if (c != j + 1)
        match = 0;
    }
    if (match == 1)
      s_0++;
  }

loop_1:
  for (int i = 0; i < 4096; i++) {
    int j = 0;
    int match = 1;
    for (j = 0; j < 3; j++) {
      int k = i + j;
      int c;
      if (k < 4096)
        c = str_1[k];
      else
        c = str_2[k - 4096];
      if (c != j + 1)
        match = 0;
    }
    if (match == 1)
      s_1++;
  }

loop_2:
  for (int i = 0; i < 4096; i++) {
    int j = 0;
    int match = 1;
    for (j = 0; j < 3; j++) {
      int k = i + j;
      int c;
      if (k < 4096)
        c = str_2[k];
      else
        c = str_3[k - 4096];
      if (c != j + 1)
        match = 0;
    }
    if (match == 1)
      s_2++;
  }

loop_3:
  for (int i = 0; i < 4096; i++) {
    int j = 0;
    int match = 1;
    for (j = 0; j < 3; j++) {
      int k = i + j;
      int c;
      if (k < 4096)
        c = str_3[k];
      else
        c = str_4[k - 4096];
      if (c != j + 1)
        match = 0;
    }
    if (match == 1)
      s_3++;
  }

loop_4:
  for (int i = 0; i < 4096; i++) {
    int j = 0;
    int match = 1;
    for (j = 0; j < 3; j++) {
      int k = i + j;
      int c;
      if (k < 4096)
        c = str_4[k];
      else
        c = str_5[k - 4096];
      if (c != j + 1)
        match = 0;
    }
    if (match == 1)
      s_4++;
  }

loop_5:
  for (int i = 0; i < 4096; i++) {
    int j = 0;
    int match = 1;
    for (j = 0; j < 3; j++) {
      int k = i + j;
      int c;
      if (k < 4096)
        c = str_5[k];
      else
        c = str_6[k - 4096];
      if (c != j + 1)
        match = 0;
    }
    if (match == 1)
      s_5++;
  }

loop_6:
  for (int i = 0; i < 4096; i++) {
    int j = 0;
    int match = 1;
    for (j = 0; j < 3; j++) {
      int k = i + j;
      int c;
      if (k < 4096)
        c = str_6[k];
      else
        c = str_7[k - 4096];
      if (c != j + 1)
        match = 0;
    }
    if (match == 1)
      s_6++;
  }

loop_7:
  for (int i = 0; i < 4096 - 3; i++) {
    int j = 0;
    int match = 1;
    for (j = 0; j < 3; j++) {
      int k = i + j;
      int c;
      c = str_7[k];
      if (c != j + 1)
        match = 0;
    }
    if (match == 1)
      s_7++;
  }

  return s_0 + s_1 + s_2 + s_3 + s_4 + s_5 + s_6 + s_7;
}
typedef float in_float_t;
typedef float out_float_t;
typedef float inout_float_t;

#include <stdlib.h>

#define N 32768

out_float_t dut (in_float_t array_0[8192], in_float_t array_1[8192],
                      in_float_t array_2[8192], in_float_t array_3[8192],
                      in_float_t array_4[8192], in_float_t array_5[8192],
                      in_float_t array_6[8192], in_float_t array_7[8192]) {
  float s_0 = 0.0f;
  float s_1 = 0.0f;
  float s_2 = 0.0f;
  float s_3 = 0.0f;
  float s_4 = 0.0f;
  float s_5 = 0.0f;
  float s_6 = 0.0f;
  float s_7 = 0.0f;

loop_0:
  for (int i = 0; i < 8192; i++) {
    float temp = array_0[i];
    if (temp != 0.0f)
      s_0 += temp;
  }

loop_1:
  for (int i = 0; i < 8192; i++) {
    float temp = array_1[i];
    if (temp != 0.0f)
      s_1 += temp;
  }

loop_2:
  for (int i = 0; i < 8192; i++) {
    float temp = array_2[i];
    if (temp != 0.0f)
      s_2 += temp;
  }

loop_3:
  for (int i = 0; i < 8192; i++) {
    float temp = array_4[i];
    if (temp != 0.0f)
      s_4 += temp;
  }

loop_5:
  for (int i = 0; i < 8192; i++) {
    float temp = array_6[i];
    if (temp != 0.0f)
      s_6 += temp;
  }

loop_7:
  for (int i = 0; i < 8192; i++) {
    float temp = array_7[i];
    if (temp != 0.0f)
      s_7 += temp;
  }

  return s_0 + s_1 + s_2 + s_3 + s_4 + s_5 + s_6 + s_7;
}

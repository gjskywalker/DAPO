/* generated on Sat Jun 08 */
/* This test set comes from HLS-benchmarks */
/* This test comes from HLS-benchmarks/Inter-Block/matrixmult */

typedef float in_float_t;
typedef float out_float_t;
typedef float inout_float_t;

void dut(in_float_t A_0[8192], in_float_t A_1[8192],
                in_float_t A_2[8192], in_float_t A_3[8192],
                in_float_t A_4[8192], in_float_t A_5[8192],
                in_float_t A_6[8192], in_float_t A_7[8192],
                in_float_t B_0[8192], in_float_t B_1[8192],
                in_float_t B_2[8192], in_float_t B_3[8192],
                in_float_t B_4[8192], in_float_t B_5[8192],
                in_float_t B_6[8192], in_float_t B_7[8192], out_float_t C_0[32],
                out_float_t C_1[32], out_float_t C_2[32], out_float_t C_3[32],
                out_float_t C_4[32], out_float_t C_5[32], out_float_t C_6[32],
                out_float_t C_7[32]) {

loop_0:
  for (int i = 0; i < 2; i++) {
    float s_0 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_0[i * 16 + j];
      float temp_b = B_0[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_0 += temp_a * temp_b;
    }
    C_0[i * 16 + 0] = s_0;
    float s_1 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_1[i * 16 + j];
      float temp_b = B_0[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_1 += temp_a * temp_b;
    }
    C_0[i * 16 + 1] = s_1;
    float s_2 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_2[i * 16 + j];
      float temp_b = B_0[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_2 += temp_a * temp_b;
    }
    C_0[i * 16 + 2] = s_2;
    float s_3 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_3[i * 16 + j];
      float temp_b = B_0[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_3 += temp_a * temp_b;
    }
    C_0[i * 16 + 3] = s_3;
    float s_4 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_4[i * 16 + j];
      float temp_b = B_0[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_4 += temp_a * temp_b;
    }
    C_0[i * 16 + 4] = s_4;
    float s_5 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_5[i * 16 + j];
      float temp_b = B_0[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_5 += temp_a * temp_b;
    }
    C_0[i * 16 + 5] = s_5;
    float s_6 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_6[i * 16 + j];
      float temp_b = B_0[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_6 += temp_a * temp_b;
    }
    C_0[i * 16 + 6] = s_6;
    float s_7 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_7[i * 16 + j];
      float temp_b = B_0[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_7 += temp_a * temp_b;
    }
    C_0[i * 16 + 7] = s_7;
  }

loop_1:
  for (int i = 0; i < 2; i++) {
    float s_0 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_0[i * 16 + j];
      float temp_b = B_1[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_0 += temp_a * temp_b;
    }
    C_1[i * 16 + 0] = s_0;
    float s_1 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_1[i * 16 + j];
      float temp_b = B_1[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_1 += temp_a * temp_b;
    }
    C_1[i * 16 + 1] = s_1;
    float s_2 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_2[i * 16 + j];
      float temp_b = B_1[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_2 += temp_a * temp_b;
    }
    C_1[i * 16 + 2] = s_2;
    float s_3 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_3[i * 16 + j];
      float temp_b = B_1[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_3 += temp_a * temp_b;
    }
    C_1[i * 16 + 3] = s_3;
    float s_4 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_4[i * 16 + j];
      float temp_b = B_1[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_4 += temp_a * temp_b;
    }
    C_1[i * 16 + 4] = s_4;
    float s_5 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_5[i * 16 + j];
      float temp_b = B_1[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_5 += temp_a * temp_b;
    }
    C_1[i * 16 + 5] = s_5;
    float s_6 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_6[i * 16 + j];
      float temp_b = B_1[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_6 += temp_a * temp_b;
    }
    C_1[i * 16 + 6] = s_6;
    float s_7 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_7[i * 16 + j];
      float temp_b = B_1[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_7 += temp_a * temp_b;
    }
    C_1[i * 16 + 7] = s_7;
  }

loop_2:
  for (int i = 0; i < 2; i++) {
    float s_0 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_0[i * 16 + j];
      float temp_b = B_2[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_0 += temp_a * temp_b;
    }
    C_2[i * 16 + 0] = s_0;
    float s_1 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_1[i * 16 + j];
      float temp_b = B_2[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_1 += temp_a * temp_b;
    }
    C_2[i * 16 + 1] = s_1;
    float s_2 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_2[i * 16 + j];
      float temp_b = B_2[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_2 += temp_a * temp_b;
    }
    C_2[i * 16 + 2] = s_2;
    float s_3 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_3[i * 16 + j];
      float temp_b = B_2[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_3 += temp_a * temp_b;
    }
    C_2[i * 16 + 3] = s_3;
    float s_4 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_4[i * 16 + j];
      float temp_b = B_2[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_4 += temp_a * temp_b;
    }
    C_2[i * 16 + 4] = s_4;
    float s_5 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_5[i * 16 + j];
      float temp_b = B_2[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_5 += temp_a * temp_b;
    }
    C_2[i * 16 + 5] = s_5;
    float s_6 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_6[i * 16 + j];
      float temp_b = B_2[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_6 += temp_a * temp_b;
    }
    C_2[i * 16 + 6] = s_6;
    float s_7 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_7[i * 16 + j];
      float temp_b = B_2[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_7 += temp_a * temp_b;
    }
    C_2[i * 16 + 7] = s_7;
  }

loop_3:
  for (int i = 0; i < 2; i++) {
    float s_0 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_0[i * 16 + j];
      float temp_b = B_3[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_0 += temp_a * temp_b;
    }
    C_3[i * 16 + 0] = s_0;
    float s_1 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_1[i * 16 + j];
      float temp_b = B_3[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_1 += temp_a * temp_b;
    }
    C_3[i * 16 + 1] = s_1;
    float s_2 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_2[i * 16 + j];
      float temp_b = B_3[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_2 += temp_a * temp_b;
    }
    C_3[i * 16 + 2] = s_2;
    float s_3 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_3[i * 16 + j];
      float temp_b = B_3[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_3 += temp_a * temp_b;
    }
    C_3[i * 16 + 3] = s_3;
    float s_4 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_4[i * 16 + j];
      float temp_b = B_3[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_4 += temp_a * temp_b;
    }
    C_3[i * 16 + 4] = s_4;
    float s_5 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_5[i * 16 + j];
      float temp_b = B_3[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_5 += temp_a * temp_b;
    }
    C_3[i * 16 + 5] = s_5;
    float s_6 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_6[i * 16 + j];
      float temp_b = B_3[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_6 += temp_a * temp_b;
    }
    C_3[i * 16 + 6] = s_6;
    float s_7 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_7[i * 16 + j];
      float temp_b = B_3[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_7 += temp_a * temp_b;
    }
    C_3[i * 16 + 7] = s_7;
  }

loop_4:
  for (int i = 0; i < 2; i++) {
    float s_0 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_0[i * 16 + j];
      float temp_b = B_4[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_0 += temp_a * temp_b;
    }
    C_4[i * 16 + 0] = s_0;
    float s_1 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_1[i * 16 + j];
      float temp_b = B_4[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_1 += temp_a * temp_b;
    }
    C_4[i * 16 + 1] = s_1;
    float s_2 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_2[i * 16 + j];
      float temp_b = B_4[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_2 += temp_a * temp_b;
    }
    C_4[i * 16 + 2] = s_2;
    float s_3 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_3[i * 16 + j];
      float temp_b = B_4[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_3 += temp_a * temp_b;
    }
    C_4[i * 16 + 3] = s_3;
    float s_4 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_4[i * 16 + j];
      float temp_b = B_4[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_4 += temp_a * temp_b;
    }
    C_4[i * 16 + 4] = s_4;
    float s_5 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_5[i * 16 + j];
      float temp_b = B_4[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_5 += temp_a * temp_b;
    }
    C_4[i * 16 + 5] = s_5;
    float s_6 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_6[i * 16 + j];
      float temp_b = B_4[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_6 += temp_a * temp_b;
    }
    C_4[i * 16 + 6] = s_6;
    float s_7 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_7[i * 16 + j];
      float temp_b = B_4[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_7 += temp_a * temp_b;
    }
    C_4[i * 16 + 7] = s_7;
  }

loop_5:
  for (int i = 0; i < 2; i++) {
    float s_0 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_0[i * 16 + j];
      float temp_b = B_5[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_0 += temp_a * temp_b;
    }
    C_5[i * 16 + 0] = s_0;
    float s_1 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_1[i * 16 + j];
      float temp_b = B_5[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_1 += temp_a * temp_b;
    }
    C_5[i * 16 + 1] = s_1;
    float s_2 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_2[i * 16 + j];
      float temp_b = B_5[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_2 += temp_a * temp_b;
    }
    C_5[i * 16 + 2] = s_2;
    float s_3 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_3[i * 16 + j];
      float temp_b = B_5[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_3 += temp_a * temp_b;
    }
    C_5[i * 16 + 3] = s_3;
    float s_4 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_4[i * 16 + j];
      float temp_b = B_5[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_4 += temp_a * temp_b;
    }
    C_5[i * 16 + 4] = s_4;
    float s_5 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_5[i * 16 + j];
      float temp_b = B_5[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_5 += temp_a * temp_b;
    }
    C_5[i * 16 + 5] = s_5;
    float s_6 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_6[i * 16 + j];
      float temp_b = B_5[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_6 += temp_a * temp_b;
    }
    C_5[i * 16 + 6] = s_6;
    float s_7 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_7[i * 16 + j];
      float temp_b = B_5[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_7 += temp_a * temp_b;
    }
    C_5[i * 16 + 7] = s_7;
  }

loop_6:
  for (int i = 0; i < 2; i++) {
    float s_0 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_0[i * 16 + j];
      float temp_b = B_6[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_0 += temp_a * temp_b;
    }
    C_6[i * 16 + 0] = s_0;
    float s_1 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_1[i * 16 + j];
      float temp_b = B_6[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_1 += temp_a * temp_b;
    }
    C_6[i * 16 + 1] = s_1;
    float s_2 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_2[i * 16 + j];
      float temp_b = B_6[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_2 += temp_a * temp_b;
    }
    C_6[i * 16 + 2] = s_2;
    float s_3 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_3[i * 16 + j];
      float temp_b = B_6[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_3 += temp_a * temp_b;
    }
    C_6[i * 16 + 3] = s_3;
    float s_4 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_4[i * 16 + j];
      float temp_b = B_6[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_4 += temp_a * temp_b;
    }
    C_6[i * 16 + 4] = s_4;
    float s_5 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_5[i * 16 + j];
      float temp_b = B_6[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_5 += temp_a * temp_b;
    }
    C_6[i * 16 + 5] = s_5;
    float s_6 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_6[i * 16 + j];
      float temp_b = B_6[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_6 += temp_a * temp_b;
    }
    C_6[i * 16 + 6] = s_6;
    float s_7 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_7[i * 16 + j];
      float temp_b = B_6[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_7 += temp_a * temp_b;
    }
    C_6[i * 16 + 7] = s_7;
  }

loop_7:
  for (int i = 0; i < 2; i++) {
    float s_0 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_0[i * 16 + j];
      float temp_b = B_7[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_0 += temp_a * temp_b;
    }
    C_7[i * 16 + 0] = s_0;
    float s_1 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_1[i * 16 + j];
      float temp_b = B_7[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_1 += temp_a * temp_b;
    }
    C_7[i * 16 + 1] = s_1;
    float s_2 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_2[i * 16 + j];
      float temp_b = B_7[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_2 += temp_a * temp_b;
    }
    C_7[i * 16 + 2] = s_2;
    float s_3 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_3[i * 16 + j];
      float temp_b = B_7[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_3 += temp_a * temp_b;
    }
    C_7[i * 16 + 3] = s_3;
    float s_4 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_4[i * 16 + j];
      float temp_b = B_7[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_4 += temp_a * temp_b;
    }
    C_7[i * 16 + 4] = s_4;
    float s_5 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_5[i * 16 + j];
      float temp_b = B_7[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_5 += temp_a * temp_b;
    }
    C_7[i * 16 + 5] = s_5;
    float s_6 = 0.0f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_6[i * 16 + j];
      float temp_b = B_7[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_6 += temp_a * temp_b;
    }
    C_7[i * 16 + 6] = s_6;
    float s_7 = 0.1f;
    for (int j = 0; j < 8192; j++) {
      float temp_a = A_7[i * 16 + j];
      float temp_b = B_7[i * 16 + j];
      if (temp_a != 0.0f && temp_b != 0.0f)
        s_7 += temp_a * temp_b;
    }
    C_7[i * 16 + 7] = s_7;
  }
}
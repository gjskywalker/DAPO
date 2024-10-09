void dut(int p_x[16], int p_y[16], int p_color[16], int frame_buffer[16][16], int output[64])
{
    // update the framebuffer
    for (int i = 0; i < 16; i++) { // {"p_x": {"dim": 1, "type": "cyclic", "factor": 0}, "p_y": {"dim": 1, "type": "cyclic", "factor": 0}, "p_color": {"dim": 1, "type": "cyclic", "factor": 0}}
        frame_buffer[p_x[i]][p_y[i]] = p_color[i];
    }
    int out_FB = 0;

    for (int i = 0; i < 16; i++) { // {"frame_buffer": {"dim": 1, "type": "cyclic", "factor": 0}, "output": {"dim": 1, "type": "block", "factor": 16}}
        for (int jj = 0; jj < 4; jj++) { // {"frame_buffer": {"dim": 2, "type": "block", "factor": 4}, "output": {"dim": 1, "type": "cyclic", "factor": 0}}
            int j = jj * 4;
            out_FB += frame_buffer[i][j + 0];
            out_FB += frame_buffer[i][j + 1];
            out_FB += frame_buffer[i][j + 2];
            out_FB += frame_buffer[i][j + 3];
            output[i * 4 + jj] = out_FB;
        }
    }
}
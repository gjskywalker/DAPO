int dut(int f_x[16], int f_y[16], int f_z[16], int f_color[16], int p_x[16], int p_y[16], int p_color[16]) {

    int z_buffer[16][16];
    for (int i = 0; i < 16; i++) { // {"z_buffer": {"dim": 1, "type": "cyclic", "factor": 0}}
        for (int j = 0; j < 16; j++) { // {"z_buffer": {"dim": 2, "type": "cyclic", "factor": 0}}
            z_buffer[i][j] = 255;
        }
    }

    int pixel_cntr = 0;

    for (int n = 0; n < 16; n++) { // {"f_x": {"dim": 1, "type": "cyclic", "factor": 0}, "f_y": {"dim": 1, "type": "cyclic", "factor": 0}, "f_z": {"dim": 1, "type": "cyclic", "factor": 0}, "f_color": {"dim": 1, "type": "cyclic", "factor": 0}}
        if (f_z[n] < z_buffer[f_y[n]][f_x[n]]) {
            p_x[pixel_cntr] = f_x[n];
            p_y[pixel_cntr] = f_y[n];
            p_color[pixel_cntr] = f_color[n];
            z_buffer[f_y[n]][f_x[n]] = f_z[n];
            pixel_cntr++;
        }
    }

    return pixel_cntr;
}
void dut(const int word_buffer_m[8][10], const bool lb[8], const bool rb[8], int line_buffer_m[8][3][10], const int conv_params_m[3][3], int conv_out_buffer_m[64], const int log_width)
{
    // slices_per_line = width / BANK_WIDTH
    const int slices_per_line = 1 << (log_width - 3);

    // Convolution
    for (int bank = 0; bank < 8; bank++)
    { // {"line_buffer_m": {"dim": 1, "type": "cyclic", "factor": 0}, "conv_out_buffer_m": {"dim": 1, "type": "block", "factor": 8}}
        for (int cc = 0; cc < 8; cc++)
        { // {"line_buffer_m": {"dim": 3, "type": "cyclic", "factor": 0}, "conv_out_buffer_m": {"dim": 1, "type": "cyclic", "factor": 0}}
            int sum = 0;
            for (int kr = 0; kr < 3; kr++)
            {
                for (int kc = 0; kc < 3; kc++)
                {
                    int data = line_buffer_m[bank][kr][cc + kc];
                    const int &wt = conv_params_m[2 - kr][2 - kc];
                    data = (wt & data) ^ data;
                    sum += data;
                }
            }
            conv_out_buffer_m[bank * 8 + cc] = sum;
        }
    }

    // Update
    // Fill line buffer with lines from the new word
    for (int bank = 0; bank < 8; bank++)
    { // {"line_buffer_m": {"dim": 1, "type": "cyclic", "factor": 0}, "lb": {"dim": 1, "type": "cyclic", "factor": 0}, "rb": {"dim": 1, "type": "cyclic", "factor": 0}}
        // --------------------------------------------------------------
        // Top row, slices are shifted right by slices_per_line
        int s_idx0 = bank - slices_per_line;
        // slice from input word
        for (int cc = 0; cc < 8; cc++)
        { // {"line_buffer_m": {"dim": 3, "type": "cyclic", "factor": 0}, "word_buffer_m": {"dim": 2, "type": "cyclic", "factor": 0}}
            line_buffer_m[bank][0][cc + 1] = word_buffer_m[s_idx0][cc + 1];
        }
        line_buffer_m[bank][0][0] = lb[bank] ? int(0) : word_buffer_m[s_idx0][0];
        line_buffer_m[bank][0][9] = rb[bank] ? int(0) : word_buffer_m[s_idx0][9];

        // --------------------------------------------------------------
        // Middle row, simply copy the word into the line buffer
        for (int cc = 0; cc < 8; cc++)
        { // {"line_buffer_m": {"dim": 3, "type": "cyclic", "factor": 0}, "word_buffer_m": {"dim": 2, "type": "cyclic", "factor": 0}}
            line_buffer_m[bank][1][cc + 1] = word_buffer_m[bank][cc + 1];
        }
        // Fill end buffer bits
        line_buffer_m[bank][1][0] = lb[bank] ? int(0) : word_buffer_m[bank][0];
        line_buffer_m[bank][1][9] = rb[bank] ? int(0) : word_buffer_m[bank][9];
    }
}
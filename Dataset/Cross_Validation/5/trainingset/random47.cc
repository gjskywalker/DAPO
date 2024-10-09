void dut(const int word_buffer_m[8][10], const int old_word_buffer_m[8][10], const bool lb[8], const bool rb[8], int line_buffer_m[8][3][10], const int log_width, const int words_per_image, const int wrd) {
    // slices_per_line = width / BANK_WIDTH
    const int slices_per_line = 1 << (log_width - 3);
    const bool first_wrd = (wrd == 0);
    const bool last_wrd = (wrd == words_per_image);

    // Prologue
    // Update bottom row, slices are shifted left. Some slices copied from previous word (middle row)
    for (int bank = 0; bank < 8; bank++) { // {"line_buffer_m": {"dim": 1, "type": "cyclic", "factor": 0}, "lb": {"dim": 1, "type": "cyclic", "factor": 0}, "rb": {"dim": 1, "type": "cyclic", "factor": 0}}
        int s_idx = bank + slices_per_line - 8;
        if (s_idx < 0) {
            // set to zero or copy from old word (middle row)
            for (int cc = 0; cc < 8; cc++) { // {"line_buffer_m": {"dim": 3, "type": "cyclic", "factor": 0}, "old_word_buffer_m": {"dim": 2, "type": "cyclic", "factor": 0}}
                line_buffer_m[bank][2][cc + 1] = old_word_buffer_m[8 + s_idx][cc + 1];
            }
            line_buffer_m[bank][2][0] = lb[bank] ? int(0) : old_word_buffer_m[8 + s_idx][0];
            line_buffer_m[bank][2][9] = rb[bank] ? int(0) : old_word_buffer_m[8 + s_idx][9];
        }
        else {
            // fill from new word
            for (int cc = 0; cc < 8; cc++) { // {"line_buffer_m": {"dim": 3, "type": "cyclic", "factor": 0}, "word_buffer_m": {"dim": 2, "type": "cyclic", "factor": 0}}
                line_buffer_m[bank][2][cc + 1] = (last_wrd) ? int(0) : word_buffer_m[s_idx][cc + 1];
            }
            line_buffer_m[bank][2][0] = (last_wrd || lb[bank]) ? int(0) : word_buffer_m[s_idx][0];
            line_buffer_m[bank][2][9] = (last_wrd || rb[bank]) ? int(0) : word_buffer_m[s_idx][9];
        }
    }
}
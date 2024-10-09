typedef unsigned char LabelType;
LabelType dut(int knn_set[64], int min_distance_list[4], int label_list[4], int vote_list[16]) {
    int pos = 1000;
    for (int i = 0; i < 16; i++) { // {"knn_set": {"dim": 1, "type": "block", "factor": 16}}
        for (int j = 0; j < 4; j++) { // {"knn_set": {"dim": 1, "type": "cyclic", "factor": 0}}
            pos = 1000;
            for (int r = 0; r < 4; r++) { // {"min_distance_list": {"dim": 1, "type": "cyclic", "factor": 0}}
                pos = ((knn_set[i * 4 + j] < min_distance_list[r]) && (pos > 4)) ? r : pos;
            }
            int r = 4;
            for (int rr = 0; rr < 4; rr++) { // {"min_distance_list": {"dim": 1, "type": "cyclic", "factor": 4}}
                if (r - 1 > pos) {
                    min_distance_list[r - 1] = min_distance_list[r - 2];
                    label_list[r - 1] = label_list[r - 2];
                }
                else if (r - 1 == pos) {
                    min_distance_list[r - 1] = knn_set[i * 4 + j];
                    label_list[r - 1] = i / 4;
                }
                r--;
            }
        }
    }

// vote
    for (int i = 0; i < 4; i++) { // {"label_list": {"dim": 1, "type": "cyclic", "factor": 0}}
        vote_list[label_list[i]] += 1;
    }

    LabelType max_vote;
    max_vote = 0;

// find the maximum value
    for (int i = 0; i < 16; i++) { // {"vote_list": {"dim": 1, "type": "cyclic", "factor": 0}}
        if (vote_list[i] >= vote_list[max_vote]) {
            max_vote = i;
        }
    }

    return max_vote;
}
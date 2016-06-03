#include "connectfour.h"

void gameOn(int player, int rows, int cols) {
    int             round;
    int             time;
    vector<spot>    board(rows*cols);
    string          line;
    string          this_move;
    // Initialize an empty board
    for (int i = 0; i < rows * cols; i++) {
        board[i] = (spot)0;
    }
    while (1) {
        
        // Check if update or action
        string u_or_a;
        getline(cin, line);
        if (line == "") {
            continue;
        }
        istringstream info(line);
        info >> u_or_a; 

        if (u_or_a == "action") {
            info >> this_move >> time;
            //printBoard(board, rows, cols);
            pickMove (board, rows, cols, time);
        }
        else if (u_or_a == "update") {
            getUpdate (line, round, board, player);
            //printBoard(board, rows, cols);
        }
    }
}

void pickMove (vector<spot> board, int rows, int cols, int time) {
    // See if there's any move to extend a 'line' with potential to be a 4
    // There are only ever COLS possible moves at max
    move_action best_move;
    int move_col;
    best_move.col = 0;
    best_move.score = 0;
    
    if (openHorizontal (board, rows, cols, move_col, MINE)) {
        if (!opponentWouldWin (board, rows, cols, move_col)) {
            cout << "place_disc " << move_col << endl << flush;
            return;
        }
    }
    if (openHorizontal (board, rows, cols, move_col, THEIRS)) {
        if (!opponentWouldWin (board, rows, cols, move_col)) {
            cout << "place_disc " << move_col << endl << flush;
            return;
        }
    }

    
    for (int i = 0; i < cols; i++) {
        int row_pos = -1;
        for (int j = rows - 1; j >= 0; j--) {
            if ( board[getIdx(j, i, cols)] == OPEN ) {
                row_pos = j;
                break;
            }
        }

        if (row_pos == -1) {
            continue;
        }
        
        int score = getMoveScore(board, rows, cols, i);
        
        if (opponentWouldWin(board, rows, cols, i)) {
            score = -10000000 + i;
        }

        if (score > best_move.score) {
            best_move.score = score;
            best_move.col = i;
        }
        else if (score == best_move.score) {
            // Choose the one that's closer 
            // to the middle for more 'surface area'
            float middle = cols / 2;
            float diff_best;
            float diff_curr;
            diff_best = abs(best_move.col - middle);
            diff_curr = abs(i - middle);
            if (diff_curr < diff_best) {
                best_move.col = i;
            }
        }
    }

    cout << "place_disc " << best_move.col << endl << flush;
}

bool opponentCanWin (vector<spot> board, int rows, int cols) {
    for (int i = 0; i < cols; i++) {
        int score = getMoveScore (board, rows, cols, i);
        if (score == 100 || score == 1000) {
            return true;
        }
    }
    return false;
}


int getMoveScore (vector<spot> board, int rows, int cols, int move) {
    // Determine the position this move would end up at
    int row_pos;
    for (int i = rows - 1; i >= 0; i--) {
        if ( board[getIdx(i, move, cols)] == OPEN ) {
            row_pos = i;
            break;
        }
    }
    // So we'd place at COLUMN: 'move' and ROW: 'row_pos'
    // Look at the pieces to the right, to the left, and underneath
    // Add pow(2, chain_length) to the score if there is a 
    // row that could possibly be made to have length = 4.
    // If max length is 1, then return 0.

    // My chain scores 
    int my_horiz_chain = getChain (board, row_pos, move, rows, cols, 0, MINE);
    int my_vert_chain  = getChain (board, row_pos, move, rows, cols, 1, MINE);
    int my_diag_chain  = getChain (board, row_pos, move, rows, cols, 2, MINE);
    
    // Opponent chain scores 
    int opponent_horiz_chain = getChain (board, row_pos, move, rows, cols, 0, THEIRS);
    int opponent_vert_chain  = getChain (board, row_pos, move, rows, cols, 1, THEIRS);
    int opponent_diag_chain  = getChain (board, row_pos, move, rows, cols, 2, THEIRS);

    // Check if we can win on a move
    if (my_horiz_chain == 3 || 
        my_vert_chain == 3 || 
        my_diag_chain == 3) {
        return 1000;
    }
    // If we can't win on the next move, check to see if
    // the opponent can, if he can, then block it
    else if (opponent_horiz_chain == 3 || 
            opponent_vert_chain == 3 || 
            opponent_diag_chain == 3) {
        return 100;
    }
    else {
        return my_horiz_chain + my_vert_chain + my_diag_chain +
            opponent_horiz_chain + opponent_vert_chain + opponent_diag_chain;
    }
}

bool openHorizontal (vector<spot> board, int rows, int cols, int& move, spot a) {
    spot prev;
    int open_chain_length;

    for (int i = rows - 1; i >= 0; i--) {
        open_chain_length = 0;
        for (int j = 1; j < cols - 1; j++) {
            prev = board[getIdx(i, j - 1, cols)];
            if (prev == OPEN && board[getIdx(i, j, cols)] == a) {
                if (i == rows - 1) {
                    open_chain_length = 1;
                }
                else if (board[getIdx(i + 1, j, cols)] != OPEN) {
                    open_chain_length = 1;
                }
            }
            else if (open_chain_length && prev == a && board[getIdx (i, j, cols)] == a) {
                if (board[getIdx(i, j + 1, cols)] == OPEN) {
                    move = j + 1;
                    if (i == rows - 1) {
                        return true;   
                    }
                    else if (board[getIdx(i + 1, j+1, cols)] != OPEN) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool opponentWouldWin (vector<spot> board, int rows, int cols, int move) {
    int row_pos;
    for (int i = rows - 1; i >= 0; i--) {
        if ( board[getIdx(i, move, cols)] == OPEN ) {
            row_pos = i;
            break;
        }
    }
    vector<spot>    tmp_board(rows*cols);
    for (int i = 0; i < rows * cols; i++) {
        tmp_board[i] = board[i];
    }
   

    // Place the move
    tmp_board[getIdx(row_pos, move, cols)] = MINE;
    //printBoard(tmp_board, rows, cols);
    
    row_pos = -1;
    
    for (int j = rows - 1; j >= 0; j--) {
        if ( tmp_board[getIdx(j, move, cols)] == OPEN ) {
            row_pos = j;
            break;
        }
    }

    if (row_pos == -1) {
        return false;
    }
    
    int score = getMoveScore(tmp_board, rows, cols, move);
    if (score == 100) {
        return true;
    }
    
    return false;    
}

int getChain (vector<spot> board, int row, int col, int rows, int cols, int type, spot a) {
    if (type == 0) {
        // Get the horizontal chain length
        int left = 0;
        int right = 0;
        int pointer = col - 1;
        // Look left
        while (pointer >= 0) {
            if (board[getIdx(row, pointer--, cols)] == a) {
                left++;
            }
            else {
                break;
            }
        }
        // Look right
        pointer = col + 1;
        while (pointer < cols) {
            if (board[getIdx(row, pointer++, cols)] == a) {
                right++;
            }
            else {
                break;
            }
        }
        return left + right;
    }
    else if (type == 1) {
        // Get the vertical chain length
        int down = 0;
        int pointer = row + 1;
        // Look down
        while (pointer < rows) {
            if (board[getIdx(pointer++, col, cols)] == a) {
                down++;
            }
            else {
                break;
            }
        }
        return down;
    }
    else {
        // Get the diagonal chain length
        // dl_ur
        int down_left = 0;
        int up_right = 0;
        int down_right = 0;
        int up_left = 0;
        int plr_row, pud_col;
        // look down left
        plr_row = row + 1;
        pud_col = col - 1;
        while (plr_row < rows && pud_col >= 0) {
            if (board[getIdx(plr_row++, pud_col--, cols)] == a) {
                down_left++;
            }
            else {
                break;
            }
        }
        // look down right
        plr_row = row + 1;
        pud_col = col + 1;
        while (plr_row < rows && pud_col < cols) {
            if (board[getIdx(plr_row++, pud_col++, cols)] == a) {
                down_right++;
            }            
            else {
                break;
            }
        }
        // look up left
        plr_row = row - 1;
        pud_col = col - 1;
        while (plr_row >= 0 && pud_col >= 0) {
            if (board[getIdx(plr_row--, pud_col--, cols)] == a) {
                up_left++;
            }
            else {
                break;
            }
        }
        // look up right
        plr_row = row - 1;
        pud_col = col + 1;
        while (plr_row >= 0 && pud_col < cols) {
            if (board[getIdx(plr_row--, pud_col++, cols)] == a) {
                up_right++;
            }
            else {
                break;
            }
        }


        if (up_right + down_left > up_left + down_right) {
            return up_right + down_left;
        }
        else {
            return up_left + down_right;
        }
    }
}

int getTime () {
    string move;
    string line;
    int time;
    //do {
        getline(cin, line);
        istringstream info(line);
        for (int i = 0; i < 1; i++) {
            info.ignore(20, ' ');
        }
        info >> move >> time;
    //}   while (move != "move");
    return time;
}

int getIdx (int row, int col, int cols) {
    return row * cols + col;
}

// For debugging purposes only
void printBoard (vector<spot> board, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << board[getIdx(i, j, cols)] << " "; 
        }
        cout << endl;
    }
}


void getUpdate (string in, int& round, vector<spot>& board, int player) {
    string line;
    string row_str;
    istringstream info (in);
    info.ignore(20, ' ');
    info.ignore(20, ' ');
    info >> line;
    if (line == "round") {
        info >> round;
        return;
    }
    else if (line == "field") {
        info >> row_str;
        size_t idx = row_str.find_first_of(",;");
        int pos = 0;
        if (player) {
            board[pos] = (row_str[0] - 48 == 2) ? MINE :
                         (row_str[0] - 48 == 1) ? THEIRS : OPEN;
            while (idx != string::npos) {
                board[++pos] = (row_str[idx + 1] - 48 == 2) ? MINE :
                             (row_str[idx + 1] - 48 == 1) ? THEIRS : OPEN;
                idx = row_str.find_first_of(",;", idx + 1); 
            }
        }
        else {
            board[pos] = (spot)(row_str[0] - 48);
            while (idx != string::npos) {
                board[++pos] = (spot)(row_str[idx + 1] - 48);
                idx = row_str.find_first_of(",;", idx + 1);
            }
        }
    }
    return;
}

int getSettings (string& p1, string& p2, string& my_bot, 
        int& timebank, int& time_per_move, int& my_bot_id, 
        int& cols, int& rows) {

    string line, setting;
    string p;
    int player = 0;

    while ( rows == -1 ) {
        // Get a line from the input
        getline(cin, line);
        // Assign to a string stream
        istringstream input(line);
        if (line == "") {
            continue;
        }
        // Assign to variables
        input >> line >> setting;
        
        if ( setting == "timebank" )
            input >> timebank; 
        else if ( setting == "time_per_move" )
            input >> time_per_move;
        else if ( setting == "player_names" ) {
            getline(input, p1, ' ');
            getline(input, p1, ',');
            getline(input, p2);
        }
        else if ( setting == "your_bot" ) {
            input >> p;
            if (p1.compare(p) == 0) {
                player = 0;
            }
            else if (p2.compare(p) == 0){
                player = 1;
            }
        }
        else if ( setting == "your_botid" )
            input >> my_bot_id;
        else if ( setting == "field_columns" )
            input >> cols;
        else if ( setting == "field_rows" )
            input >> rows;

    }
    return player;
}

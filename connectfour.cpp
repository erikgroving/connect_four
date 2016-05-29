#include "connectfour.h"

void gameOn(int rows, int cols) {
    bool            game = true;
    int             round;
    vector<int>     board(rows*cols);
    
    // Initialize an empty board
    for (int i = 0; i < rows * cols; i++) {
        board[i] = 0;
    }
    while (game) {
        // Get the round number
        getRoundNum(round);
        // Get the current status of the board
        
        // Get the amount of time you have
        game = false;
    }
}

void getRoundNum (int& round) {
    string line;
    do {
        getline(cin, line);
        istringstream info(line);
        for (int i = 0; i < 2; i++) {
            info.ignore(20, ' ');
        }
        info >> line >> round;
    } while (line != "round");
    cout << line <<"num was " << round << endl;
    return;
}

void getSettings (string& p1, string& p2, string& my_bot, 
        int& timebank, int& time_per_move, int& my_bot_id, 
        int& cols, int& rows) {

    string line, setting;

    while ( rows == -1 ) {
        // Get a line from the input
        getline(cin, line);
        // Assign to a string stream
        istringstream input(line);
        // Assign to variables
        input >> line >> setting;
        
        if ( setting == "timebank" )
            input >> timebank; 
        else if ( setting == "time_per_move" )
            input >> time_per_move;
        else if ( setting == "player_names" ) {
            getline(input, p1, ',');
            getline(input, p2);
        }
        else if ( setting == "your_bot" )
            input >> p1;
        else if ( setting == "your_botid" )
            input >> my_bot_id;
        else if ( setting == "field_columns" )
            input >> cols;
        else if ( setting == "field_rows" )
            input >> rows;

    }

    printf ("Timebank: %d\tTPM: %d\tP1: %s\tP2: %s\tBot ID: %d\tCols: %d\tRows: %d\n",
            timebank, time_per_move, p1.c_str(), p2.c_str(), my_bot_id, cols, rows);
}

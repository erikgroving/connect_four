#ifndef CONNECTFOURH
#define CONNECTFOURH

#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>
#include <cmath>

using namespace std; 

enum spot { OPEN=0, MINE=1, THEIRS=2 };
class move_action {
    public:
        int col;
        int score;
};


void gameOn (int, int, int);
void pickMove (vector<spot>, int, int, int);
int getMoveScore (vector<spot>, int, int, int);
int getChain (vector<spot>, int, int, int, int, int, spot);
bool openHorizontal (vector<spot>, int, int, int&, spot);
bool opponentWouldWin (vector<spot>, int, int, int);

int getIdx(int, int, int);
void printBoard (vector<spot>, int, int);

void getUpdate (string in, int& , vector<spot>&, int);
int getSettings (string& p1, string& p2, string& my_bot, 
                int& timebank, int& time_per_move, int& my_bot_id, 
                int& cols, int& rows);



#endif

#ifndef CONNECTFOURH
#define CONNECTFOURH

#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>

using namespace std; 

void getRoundNum (int& );

void getSettings (string& p1, string& p2, string& my_bot, 
                int& timebank, int& time_per_move, int& my_bot_id, 
                int& cols, int& rows);

void gameOn (int, int);


#endif

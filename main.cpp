#include "connectfour.h"

int main() {
    string setting, var_val;
    string  p1, p2;
    string  my_bot;
    int     timebank;
    int     time_per_move;
    int     my_bot_id;
    int     cols;
    int     rows = -1;
    int     player;
    player = getSettings (p1, p2, my_bot, timebank, time_per_move,
            my_bot_id, cols, rows);
    gameOn(player, rows, cols);

    return 0;
}

#ifndef TETRIS_H
#define TETRIS_H

#include <vector>
using namespace std;

struct Tetris {
    int lines;
    vector<vector<int>> board;
    int H, W;
    int block;
    bool end_of_game;

    void init(int h, int w);
    bool inn(int y, int x);
    void erase_row(vector<int>& a);
    void move(int col, int rot);
    void render();
};

#endif

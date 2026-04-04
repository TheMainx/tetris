#ifndef TETRIS_H
#define TETRIS_H

#include <vector>
using namespace std;
typedef long double ld;

struct Tetris {
    int lines;
    vector<int> board; //dla kazdego wiersza trzyma liczbe ktora jest bitmaska wiersza
    ld w1, w2, w3, w4;
    int ile_dziur;
    int H, W;
    int block;
    bool end_of_game;
    vector<int> ile_w_kolumnie_dziur;
    vector<int> wysokosci;

    void init(int h, int w, ld we1, ld we2, ld we3, ld we4);
    int pole(int h, int w);
    bool inn(int y, int x);
    void erase_row(vector<int>& a);
    void move(int col, int rot);
    void render();
    int max_height();
    int kwadraty_roznic();
    ld position_value();
};

#endif

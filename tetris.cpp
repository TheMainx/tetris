#include "tetris.h"
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

#define st first
#define nd second
#define pb push_back
#define all(a) a.begin(), a.end()
#define sz(a) int(a.size())
#define f(i, a, b) for (int i = a; i < b; i++)
#define rep(i, a) f(i, 0, a)
#define tv(a, x) for (auto& a : x)
#define en "\n"
using vi = vector<int>;

mt19937 gen(chrono::high_resolution_clock::now().time_since_epoch().count());

int los(int l, int r) {
    return (l + (gen() % (r - l + 1)));
}

const int W = 10;
const int H = 20;

vector<vector<vector<pair<int, int>>>> tetromina = {
    { // I
        {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}}
    },

    { // L
        {{0, 0}, {0, 1}, {0, 2}, {1, 0}},
        {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
        {{-1, 2}, {0, 0}, {0, 1}, {0, 2}},
        {{0, 0}, {1, 0}, {2, 0}, {2, 1}}
    },

    { // J
        {{0, 0}, {0, 1}, {0, 2}, {1, 2}},
        {{-2, 1}, {-1, 1}, {0, 0}, {0, 1}},
        {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 0}, {0, 1}, {1, 0}, {2, 0}}
    },

    { // T
        {{0, 0}, {0, 1}, {0, 2}, {1, 1}},
        {{-1, 1}, {0, 0}, {0, 1}, {1, 1}},
        {{-1, 1}, {0, 0}, {0, 1}, {0, 2}},
        {{0, 0}, {1, 0}, {1, 1}, {2, 0}}
    },

    { // S
        {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
        {{-1, 1}, {0, 0}, {0, 1}, {1, 0}},
        {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
        {{-1, 1}, {0, 0}, {0, 1}, {1, 0}}
    },

    { // Z
        {{-1, 1}, {-1, 2}, {0, 0}, {0, 1}},
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
        {{-1, 1}, {-1, 2}, {0, 0}, {0, 1}},
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}}
    },

    { // O
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}}
    }
};

void Tetris::init(int h, int w, ld we1, ld we2, ld we3, ld we4) {
    w1 = we1;
    w2 = we2;
    w3 = we3;
    w4 = we4;
    H = h;
    W = w;
    ile_dziur = 0;
    wysokosci = {};
    wysokosci.resize(W, 0);
    ile_w_kolumnie_dziur = {};
    ile_w_kolumnie_dziur.resize(W, 0);
    board = {};
    board.resize(h, 0);
    block = los(0, sz(tetromina) - 1);
    end_of_game = false;
    lines = 0;
}

bool Tetris::inn(int y, int x) {
    if (min(y, x) < 0 || y >= H || x >= W) return false;
    return true;
}

int Tetris::pole(int h, int w) {
    return (board[h]&(1 << w));
}

void Tetris::erase_row(vi& a) {
    if (sz(a) == 0) {
        return;
    }
    sort(all(a));
    lines += sz(a);
    a.pb(H + 10);
    int wsk = 0;
    f(i, 0, H) {
        if (a[wsk] == i) {
            f(j, 0, W) {
                board[i] = 0;
            }
            wsk++;
        } else {
            if (wsk) {
                swap(board[i], board[i - wsk]);
            }
        }
    }
    f(i, 0, W) {
        wysokosci[i] = 0;
        for (int h = H - 1; h >= 0; h--) {
            if (pole(h, i)) {
                wysokosci[i] = h + 1;
                break;
            }
        }
    }
    ile_dziur = 0;
    f(j, 0, W) {
        ile_w_kolumnie_dziur[j] = 0;
        int zj = 0;
        for (int i = H - 1; i >= 0; i--) {
            if (!pole(i, j) && zj) {
                ile_w_kolumnie_dziur[j]++;
            }
            zj += pole(i, j);
        } 
        ile_dziur += ile_w_kolumnie_dziur[j];
    }
}

void Tetris::move(int col, int rot) {
    rot = rot % (sz(tetromina[block]));
    int maksx = col;
    f(i, 0, 4) {
        maksx = max(maksx, col + tetromina[block][rot][i].nd);
    }
    if (maksx >= W) {
        end_of_game = true;
        return;
    }

    int stop = 0; //stop jest liczony wzgledem klocka {0, 0}
    tv(ele, tetromina[block][rot]) {
        int nx = col + ele.nd;
        stop = max(stop, wysokosci[nx] - ele.st);
    }

    int ys = stop;
    int xs = col;
    vi do_usuniecia;
    tv(ele, tetromina[block][rot]) {
        int ny = ys + ele.st;
        int nx = xs + ele.nd;
        if (!inn(ny, nx)) {
            end_of_game = true;
            return;
        }
    }

    tv(ele, tetromina[block][rot]) {
        int ny = ys + ele.st;
        int nx = xs + ele.nd;
        board[ny] += (1 << nx);
        if (board[ny] == ((1 << W) - 1)) {
            do_usuniecia.pb(ny);
        }
        wysokosci[nx] = max(wysokosci[nx], ny + 1);
    }

    tv(ele, tetromina[block][rot]) {
        int ny = ys + ele.st;
        int nx = xs + ele.nd;
        ile_dziur -= ile_w_kolumnie_dziur[nx];
        ile_w_kolumnie_dziur[nx] = 0;
        int zj = 0;
        for (int i = H - 1; i >= 0; i--) {
            if (!pole(i, nx) && zj) {
                ile_w_kolumnie_dziur[nx]++;
            }
            zj += pole(i, nx);
        } 
        ile_dziur += ile_w_kolumnie_dziur[nx];
    }

    erase_row(do_usuniecia);
    block = los(0, sz(tetromina) - 1);
}

void Tetris::render() {
    cout << "ile dziur = " << ile_dziur << en;
    for (int h = H - 1; h >= 0; h--) {
        f(w, 0, W) {
            if (pole(h, w)) cout << '#';
            else cout << '.';
        }
        cout << "\n";
    }
}

int Tetris::max_height() {
    int wyn = 0;
    f(j, 0, W) {
        wyn = max(wyn, wysokosci[j]);
    }
    return wyn;
}

int Tetris::kwadraty_roznic() { //kwadraty roznic wzgeldnych
    int ans = 0;
    f(i, 1, W) {
        ans += ((wysokosci[i] - wysokosci[i - 1])*(wysokosci[i] - wysokosci[i - 1]));
    }
    return ans;
}

ld Tetris::position_value() {
    return ld(ile_dziur) * w1 + ld(max_height()) * w2 - ld(lines) * w3 + ld(kwadraty_roznic()) * w4;
}


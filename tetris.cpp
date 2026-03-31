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

void Tetris::init(int h, int w, ld we1, ld we2, ld we3) {
    w1 = we1;
    w2 = we2;
    w3 = we3;
    H = h;
    W = w;
    board = {};
    board.resize(h, vi(w, 0));
    block = los(0, sz(tetromina) - 1);
    end_of_game = false;
    lines = 0;
}

bool Tetris::inn(int y, int x) {
    if (min(y, x) < 0 || y >= H || x >= W) return false;
    return true;
}

void Tetris::erase_row(vi& a) {
    sort(all(a));
    lines += sz(a);
    a.pb(H + 10);
    int wsk = 0;
    f(i, 0, H) {
        if (a[wsk] == i) {
            f(j, 0, W) {
                board[i][j] = 0;
            }
            wsk++;
        } else {
            if (wsk) {
                swap(board[i], board[i - wsk]);
            }
        }
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

    int stop = 0;
    for (int h = H - 1; h >= 0; h--) {
        if (stop) break;
        int ys = h;
        int xs = col;
        tv(ele, tetromina[block][rot]) {
            int ny = ys + ele.st;
            int nx = xs + ele.nd;
            if (ny < 0) {
                stop = h + 1;
                break;
            }
            if (ny >= H) {
                continue;
            }
            if (board[ny][nx]) {
                stop = h + 1;
                break;
            }
        }
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
        board[ny][nx]++;
        int sum = 0;
        f(i, 0, W) {
            sum += board[ny][i];
        }
        if (sum == W) {
            do_usuniecia.pb(ny);
        }
    }

    erase_row(do_usuniecia);
    block = los(0, sz(tetromina) - 1);
}

void Tetris::render() {
    for (int h = H - 1; h >= 0; h--) {
        f(i, 0, W) {
            if (board[h][i]) cout << '#';
            else cout << '.';
        }
        cout << "\n";
    }
}

int Tetris::how_many_holes() {
    int ans = 0;
    f(j, 0, W) {
        int lnk = 0;
        f(i, 0, H) {
            if (board[i][j] && lnk) {
                ans++;
            }
            lnk += (board[i][j] ^ 1);
        }
    }
    return ans;
}

int Tetris::max_height() {
    for (int i = H - 1; i >= 0; i--) {
        f(j, 0, W) {
            if (board[i][j]) return i;
        }
    }
    return 0;
}

ld Tetris::position_value() {
    return ld(how_many_holes()) * w1 + ld(max_height()) * w2 + ld(lines) * w3;
}


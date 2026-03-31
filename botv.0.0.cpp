#include "tetris.h"
#include <bits/stdc++.h>

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

const int H = 20;
const int W = 10;
const int ile_gier = 100;
const int ile_maks_ruchow = 10000; //ile maksymalnie klockow na gre

vi ile_rotacji = {2, 4, 4, 4, 4, 4, 1}; //minimalny opt i to jeszcze zle napisany - trzeba pozniej bedzie poprawic by lepiej dzialal

void trenuj(ld p1, ld p2, ld p3) { //ma dac mediane po ile_gier grach dla danego zestawu parametrow
    vi wyn; //wyniki
    rep(_, ile_gier) {
        Tetris gra;
        gra.init(H, W, p1, p2, p3);
        f(j, 0, ile_maks_ruchow) {
            pair<ld, pair<int, int>> ruch = {1'000'000'000, {-1, -1}}; //chcemy go minimalizowac
            f(w, 0, W) {
                f(rot, 0, ile_rotacji[gra.block]) {
                    Tetris kopia = gra;
                    kopia.move(w, rot);
                    if (kopia.end_of_game) continue;
                    ruch = min(ruch, {kopia.position_value(), {w, rot}});
                }
            }
            if (ruch.nd.st == -1) { //nie ma dobrego ruchu
                wyn.pb(gra.lines);
                break;
            }
            gra.move(ruch.nd.st, ruch.nd.nd);
            if (j == (ile_maks_ruchow  -1)) {
                wyn.pb(gra.lines);
                break;
            }
        }
    }
    sort(all(wyn));
    int maks = wyn[0];
    int minn = wyn[0];
    int med = wyn[sz(wyn)/2];
    int sum = 0;
    tv(ele, wyn) {
        maks = max(maks, ele);
        minn = min(minn, ele);
        sum += ele;
    }
    cout << "maks = " << maks << en;
    cout << "min = " << minn << en;
    cout << "mediana = " << med << en;
    cout << "srednia = " << ld(ld(sum)/ld(sz(wyn))) << en;
}

int main() {
   trenuj(20, 5, -10);    
}

#include "tetris.h"
#include <bits/stdc++.h>

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

mt19937 genn(chrono::high_resolution_clock::now().time_since_epoch().count());

ld losuj(int l, int r) {
    return ld(l + genn()%(r - l + 1));
}

const int H = 20;
const int W = 10;
const int ile_gier = 1;
const int ile_maks_ruchow = 100000; //ile maksymalnie klockow na gre

vi ile_rotacji = {2, 4, 4, 4, 4, 4, 1}; //minimalny opt i to jeszcze zle napisany - trzeba pozniej bedzie poprawic by lepiej dzialal
vector<char> klocki = {'I', 'L', 'J', 'T', 'S', 'Z', 'O'};
bool debug = true;
bool wypisywanie = true;

int trenuj(vector<ld>& a) { //ma dac mediane po ile_gier grach dla danego zestawu parametrow
    vi wyn; //wyniki
    rep(_, ile_gier) {
        Tetris gra;
        gra.init(H, W, a[0], a[1], a[2], a[3]);
        f(j, 0, ile_maks_ruchow) {
            if (wypisywanie) {
                cout << "blok = " << klocki[gra.block] << en;
                gra.render();
                cout << en;
            }
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
                cout << "do konca doszlismy\n";
                wyn.pb(gra.lines);
                break;
            }
        }
    }
    sort(all(wyn));
    int med = wyn[sz(wyn)/2];
    int sum = 0;
    int maks = wyn[0];
    int minn = wyn[0];
    tv(ele, wyn) {
        sum += ele;
        maks = max(maks, ele);
        minn = min(minn, ele);
    }
    if (debug){
        cout << "maks=" << maks << en;
        cout << "minn=" << minn << en;
        cout << "med=" << med << en;
        cout << "srednia=" << ld(ld(sum)/ld(ile_gier)) << en;
    }
    return ((med * sz(wyn) * 2) + sum);
}

const ld d = 100;

pair<int, vector<ld>> dobierz_parametry(int zm) {
    int n = 4;
    vector<ld> wyn = {ld(losuj(50, 100))/d, ld(losuj(1, 50))/d, ld(losuj(1, 50))/d, ld(losuj(1, 50))/d};
    int liczba_epok = 100; //ile moze i tak bedzie break
    int ile_prob = 12;
    pair<int, vector<ld>> aktl_kand = {trenuj(wyn), wyn};
    f(j, 0, liczba_epok) {
        cout << "start epoki " << j << en;
        int zakres = (300 - (zm * j));
        if (zakres <= 0) {
            break;
        }
        rep(_, ile_prob) {
            cout << "proba " << _ << en;
            vector<ld> kand = wyn;
            f(i, 0, n) {
                int zm = losuj(0, zakres * 2);
                zm -= zakres;
                zm += 100;
                ld pr = ld(zm)/d;
                kand[i] *= pr;
            }
            int nowy_wyn = trenuj(kand);
            //cout << "pot wyn to " << ld(ld(nowy_wyn)/ld(ile_gier * 3)) << "med + srednia/2" << en;
            aktl_kand = max(aktl_kand, {nowy_wyn, kand});
        }
        wyn = aktl_kand.nd;
        cout << "koniec epoki pot wyn to " << ld(ld(aktl_kand.st)/ld(ile_gier * 3)) << "\n";
    }
    return aktl_kand;
}

int main() {
    /*pair<int, vector<ld>> wyn = dobierz_parametry(30);
    tv(ele, wyn.nd) {
        cout << ele << " ";
    }
    cout << en;*/
    vector<ld> parametry = {5.75484 ,0.303995 ,0.553957 ,0.316186};
    trenuj(parametry);
}

#include <string>
#include <vector>
#include "Champion.h"
using namespace std;

struct GameState {
    // gold shop, board, bench, augments, time, level, traits, items, 
    int gold = 0;
    int level = 1;
    float time = 0.0f;

    vector<Champion> shop = vector<Champion>(5);
    vector<Champion> bench = vector<Champion>(9);
    vector<vector<Champion>> board = vector<vector<Champion>>(4, vector<Champion>(7));
};
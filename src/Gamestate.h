#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Champion.h"
using namespace std;

struct GameState {
    // gold shop, board, bench, augments, time, level, traits, items, 

    // basics
    int gold = 0;
    int level = 1;
    int xp = 0;
    float time = 0.0f;
    int stage = 0;

    // board
    vector<Champion> bench = vector<Champion>(9);
    vector<vector<Champion>> board = vector<vector<Champion>>(4, vector<Champion>(7));
    int boardUnitCount = 0;
    vector<int> activeTraits;
    vector<int> items;

    // shop
    vector<Champion> shop = vector<Champion>(5);
    bool shoplocked = false;
    unordered_map<int,int> pool;

    // miscellaneous
    // pris ticket
    // zed augment
    bool zed = false;
    // miss fortune

};
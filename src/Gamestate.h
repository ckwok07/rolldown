#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Champion.h"
#include <variant>
using namespace std;
#include "Globals.h"
using Item = variant<int, pair<int,int>>;
#include "SetId.h"

struct Set17State {
    bool zed = false;
    // Miss Fortune mechanic
};

struct Set18State {
    // Lux mechanic
};

using SetState = variant<Set17State, Set18State>;

struct GameState {
    // set
    SetId activeSet = SetId::Set17;
    const vector<Champion>* activeChampions = nullptr;
    SetState setState = Set17State{};

    // gold shop, board, bench, augments, time, level, traits, items, 

    // basics
    int gold = 0;
    int level = 8;
    int xp = 0;
    float time = 0.0f;
    int stage = 0;

    // board
    vector<Champion> bench = vector<Champion>(9);
    vector<vector<Champion>> board = vector<vector<Champion>>(4, vector<Champion>(7));
    int boardUnitCount = 0;
    vector<TraitEntry> activeTraits;
    vector<Item> items;

    // shop
    vector<Champion> shop = vector<Champion>(5);
    vector<bool> shopSameChampion = vector<bool>(5, false);
    vector<int> shopStarUppable = vector<int>(5, 0);
    vector<bool> shopInTeamBuilder = vector<bool>(5, false);

    bool shoplocked = false;
    unordered_map<int,int> pool;

    // miscellaneous
    // pris ticket

    Champion tempSlot = nullChamp;

};
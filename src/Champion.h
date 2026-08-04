#pragma once
#include <string>
#include <vector>
#include <variant>
#include "Trait.h"
using namespace std;
using Item = variant<int, pair<int,int>>;

struct Champion {
    int id;
    string name;
    int cost;
    int starLevel;
    vector<trait> traits;
    vector<Item> items;

    bool operator==(const Champion& other) const {
        return id == other.id;
    }

    bool operator!=(const Champion& other) const {
        return !(*this == other);
    }
};

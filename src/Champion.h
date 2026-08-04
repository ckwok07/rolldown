#pragma once
#include <string>
#include <vector>
#include <variant>
using namespace std;
using Item = variant<int, pair<int,int>>;

struct Champion {
    int id;
    string name;
    int cost;
    int starLevel;
    vector<int> traits;
    vector<Item> items;

    bool operator==(const Champion& other) const {
        return id == other.id;
    }

    bool operator!=(const Champion& other) const {
        return !(*this == other);
    }
};

#pragma once
#include <string>
#include <vector>
using namespace std;

struct Champion {
    int id;
    string name;
    int cost;
    int starLevel;
    vector<int> traits;
    vector<int> items;

    bool operator==(const Champion& other) const {
        return id == other.id;
    }
};
#include "Globals.h"
#include "Champion.h"
#include <unordered_map>

unordered_map<int, Champion> champions = {

};

unordered_map<int, string> traits = {
    {0, "Bruiser"},
    {1, "Sorcerer"},
    {2, "Bastion"}
};

unordered_map<pair<int, int>, string> completed_items = {
    {{0, 0}, "Deathblade"},
    {{0, 1}, "Giant Slayer"},
    {{1, 1}, "Guinsoo's Rageblade"}
};

unordered_map<int, string> component_items = {
    {0, "B.F. Sword"},
    {1, "Recurve Bow"},
    {2, "Needlessly Large Rod"}
};
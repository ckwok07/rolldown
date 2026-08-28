#pragma once
#include "SetId.h"
#include "Champion.h"
#include "Globals.h"
#include <functional>

class Teambuilder
{
private:
    /* data */
public:
    Teambuilder(/* args */);
    ~Teambuilder();

    void init(function<void()> callback);
    
    vector<vector<Champion>> teams = {};
    vector<int> ordering = {}; // last viewed -> first viewed ordering
    int selected = -1;
    int active = -1;
    std::function<void()> onActiveTeamChanged;

    // mechanics
    void addTeam();
    void deleteTeam(int index);

    void makeActive(int index);
    void deactivate();

    void selectTeam(int index);

    // selected team mechanics
    bool addToSelected(Champion champ);
    void removeFromSelected(int index);
};

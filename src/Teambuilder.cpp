#include "Teambuilder.h"

Teambuilder::Teambuilder(/* args */)
{
}

Teambuilder::~Teambuilder()
{
}

void Teambuilder::init(std::function<void()> callback) {
    onActiveTeamChanged = callback;
}

void Teambuilder::addTeam() {
    if (teams.size() >= 30) return;

    teams.push_back(vector<Champion>(10,nullChamp));
    ordering.push_back(teams.size() - 1);
}

void Teambuilder::deleteTeam(int index) {
    if (index < 0 || index >= ordering.size()) return;
    int num = ordering[index];

    if (selected == num) selected = -1;
    else if (selected > num) selected--;

    if (active == num) {
        active = -1;
        if (onActiveTeamChanged) {
            onActiveTeamChanged();
        }
    } else if (active > num) active--;

    teams.erase(teams.begin() + num);
    ordering.erase(ordering.begin() + index);

    for (int i = 0; i < ordering.size(); i++) {
        if (ordering[i] > num) ordering[i]--;
    }
}

void Teambuilder::makeActive(int index) {
    active = ordering[index];

    if (onActiveTeamChanged) {
        onActiveTeamChanged();
    }
}

void Teambuilder::selectTeam(int index) {
    if (index < 0 || index >= ordering.size()) return;

    selected = ordering[index];

    ordering.erase(ordering.begin() + index);
    ordering.insert(ordering.begin(), selected);
}

bool Teambuilder::addToSelected(Champion champ) {
    if (selected == -1) return false;

    for (int i = 0; i < teams[selected].size(); i++) {
        if (teams[selected][i] == nullChamp) {
            teams[selected][i] = champ;
            if (selected == active && onActiveTeamChanged) {
                onActiveTeamChanged();
            }
            return true;
        }
    }

    return false;
}

void Teambuilder::removeFromSelected(int index) {
    if (selected == -1) return;

    teams[selected][index] = nullChamp;

    if (selected == active && onActiveTeamChanged) {
        onActiveTeamChanged();
    }
}
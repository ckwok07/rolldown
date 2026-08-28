#pragma once
#include "../Teambuilder.h"

class TeambuilderUI
{
private:
    /* data */
public:
    TeambuilderUI(/* args */);
    ~TeambuilderUI();
    void init();
    void shutdown();

    Teambuilder teambuilder;
    bool open = false;

    void drawTeambuilder();

    void makeOpen();
    void drawClose();
    void drawOpen(); 
    void drawUnselected();
    void drawSelected();
};

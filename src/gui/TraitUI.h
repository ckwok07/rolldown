#pragma once

#include "raylib.h"
#include "Engine.h"

class TraitUI
{
private:
    /* data */
    int scrollOffset = 0;

    Font traitFont = {};
    Font uiFont = {};
public:
    TraitUI(/* args */);
    ~TraitUI();

    void init();
    void shutdown();
    
    Rectangle TraitBarRect();
    void DrawTraitHexs(Engine& engine);
    void DrawTraits(Engine& engine);

    void DrawTraitsVisuals();
};
#pragma once

#include <string>
#include <unordered_map>
#include "raylib.h"
#include "../Engine.h"
#include "Drag.h"
#include "ShopUI.h"
#include "TraitUI.h"
#include "BoardUI.h"
#include "TeambuilderUI.h"

class App
{
private:
    Texture2D background;
    Engine engine;

    Camera3D camera = { 0 };

    // dragging stuff
    Drag drag;
    // shop
    ShopUI shop;

    // trait
    TraitUI traits;

    // board
    BoardUI board;

    // teambuilder
    TeambuilderUI teambuilderUI;

    void HandleDragPress(int hoveredShop, int hoveredBench, int hoveredRow, int hoveredCol);
    void HandleDragRelease(int hoveredBench, int hoveredRow, int hoveredCol, int hoveredItem);
    void HandleChampionDrop(const SlotRef& target);

    SlotRef GetDropTarget(int hoveredBench, int hoveredRow, int hoveredCol);

    const Champion* GetDraggedChampion() const;

    bool GetMouseGroundPosition(Vector3& position) const;
    void DrawDraggedChampionModel();
public:
    App(/* args */);
    ~App();

    float r = 0.55f;
    float drawR = r * 0.90f; 
    float squareSide = r * 1.2f;

    void DrawTraitHexs();

    bool init();
    void run();
    void shutdown();
};
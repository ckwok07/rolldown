#pragma once

#include <string>
#include <unordered_map>
#include "raylib.h"
#include "../Engine.h"
#include "Drag.h"
#include "ShopUI.h"
#include "TraitUI.h"

class App
{
private:
    Texture2D background;
    Engine engine;

    Camera3D camera = { 0 };
    // std::unordered_map<std::string, Texture2D> splashTextures;

    // Texture2D* GetChampionSplash(const std::string& championName);

    // void DrawTextureCover(Texture2D texture, Rectangle destination, Color tint = WHITE);

    // void DrawShopIcon(Rectangle rect, Champion& champion, Color tierColor, int i,  bool highlighted);

    std::unordered_map<int, Model> champModels;
    std::unordered_map<int, ModelAnimation*> champAnims;
    std::unordered_map<int, int> champAnimCounts;
    std::unordered_map<int, float> champScales;
    std::unordered_map<int, float> champYOffsets;
    std::unordered_map<int, float> champAnimFrame;
    std::unordered_map<int, float> champAnimDir;

    // dragging stuff
    Drag drag;
    // shop
    ShopUI shop;

    // trait
    TraitUI traits;

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

    Vector3 HexCenter(int row, int col);
    Vector3 BenchCenter(int i);
    // Rectangle ShopSlotRect(int i);
    // void DrawShopTrapezoid();
    // Rectangle ShopBarRect();
    Rectangle TraitBarRect();
    void DrawTraitHexs();

    // Rectangle ShopXpRect();
    // Rectangle ShopRerollRect();


    bool init();
    void run();
    void shutdown();
};
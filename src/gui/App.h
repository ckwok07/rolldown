#pragma once

#include <string>
#include <unordered_map>
#include "raylib.h"
#include "../Engine.h"

// describes where an object is located
enum class Zone {
    None, // no valid location
    Shop,
    Bench,
    Board,
    Inventory,
    SellArea
};

// describes the object being dragged
enum class DragPayload {
    None,
    Champion,
    Item
};

// current state of mouse
enum class DragPhase {
    Idle,
    Dragging
};

// generic reference to  any location that can involve dragging
struct SlotRef {
    Zone zone = Zone::None;
    int index = -1;
    int row = -1;
    int col = -1;
};

// state of a current drag
struct DragState {
    DragPhase phase = DragPhase::Idle;
    DragPayload payload = DragPayload::None;

    SlotRef source;
    SlotRef holdTarget;

    Vector2 pressPos = {};
    Vector2 grabOffset = {};

    float holdTime = 0.0f;
    bool holdReady = false;
};


class App
{
private:
    Texture2D background;
    Engine engine;

    Camera3D camera = { 0 };
    std::unordered_map<std::string, Texture2D> splashTextures;

    Texture2D* GetChampionSplash(const std::string& championName);

    void DrawTextureCover(Texture2D texture, Rectangle destination, Color tint = WHITE);

    void DrawShopIcon(Rectangle rect, Champion& champion, Color tierColor, int i,  bool highlighted);

    std::unordered_map<int, Model> champModels;
    std::unordered_map<int, ModelAnimation*> champAnims;
    std::unordered_map<int, int> champAnimCounts;
    std::unordered_map<int, float> champScales;
    std::unordered_map<int, float> champYOffsets;
    std::unordered_map<int, float> champAnimFrame;
    std::unordered_map<int, float> champAnimDir;

    // dragging stuff
    DragState drag;

    void BeginDrag(DragPayload payload, const SlotRef& source, Vector2 grabOffset = {});
    void ResetDrag();

    void HandleDragPress(int hoveredShop, int hoveredBench, int hoveredRow, int hoveredCol);
    void HandleDragRelease(int hoveredBench, int hoveredRow, int hoveredCol);
    void HandleChampionDrop(const SlotRef& target);

    SlotRef GetDropTarget(int hoveredBench, int hoveredRow, int hoveredCol);
    Rectangle ShopSellRect();

    bool IsDraggedSource(const SlotRef& slot) const;
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
    Rectangle ShopSlotRect(int i);
    Rectangle ShopBarRect();

    Rectangle ShopXpRect();
    Rectangle ShopRerollRect();


    bool init();
    void run();
    void shutdown();
};
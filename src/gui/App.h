#pragma once

#include <string>
#include <unordered_map>
#include "raylib.h"
#include "../Engine.h"


enum class Zone { None, Shop, Bench, Board };
enum class DragPhase { Idle, Pending, Dragging };

struct DragState {
    DragPhase phase = DragPhase::Idle;
    Zone sourceZone = Zone::None;
    int sourceIndex = -1;
    pair<int,int> sourceBoard = make_pair(-1,-1);
    Vector2 pressPos = { 0, 0 };
    Vector2 grabOffset = { 0, 0 };
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
public:
    App(/* args */);
    ~App();

    float r = 0.55f;
    float drawR = r * 0.90f; 
    float squareSide = r * 1.2f;

    Model ahriModel = {};

    Vector3 HexCenter(int row, int col);
    Vector3 BenchCenter(int i);
    Rectangle ShopSlotRect(int i);
    Rectangle ShopBarRect();

    Rectangle ShopXpRect();
    Rectangle ShopRerollRect();

    DragState drag;

    bool init();
    void run();
    void shutdown();
};
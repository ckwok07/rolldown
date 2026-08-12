#pragma once

#include "raylib.h"
#include "Engine.h"
#include <unordered_map>
#include <variant>
#include <string>
#include "Drag.h"
using Item = variant<int, pair<int,int>>;

class TraitUI
{
private:
    /* data */
    int scrollOffset = 0;

    Font traitFont = {};
    Font uiFont = {};

    std::unordered_map<std::string, Texture2D> itemTextures;
public:
    TraitUI(/* args */);
    ~TraitUI();

    void init();
    void shutdown();
    
    Rectangle TraitBarRect();
    void DrawTraitHexs(Engine& engine);
    void DrawTraits(Engine& engine, const Drag& drag);

    void DrawTraitsVisuals();
    void DrawItems(const vector<Item>& items, const Drag& drag);

    string GetItemName(const Item& item);
    Texture2D* GetItemTexture(const Item& item);

    // mechanics

    int GetHoveredItemSlot(Vector2 mousePos);
    void DrawDraggedItem(const vector<Item>& items, const Drag& drag);
};
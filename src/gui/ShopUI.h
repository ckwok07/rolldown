#include "raylib.h"
#include <unordered_map>
#include "../Engine.h"
#include "Drag.h"

class ShopUI
{
private:
    /* data */
    Font traitFont = {};
    Font uiFont = {};
public:
    ShopUI(/* args */);
    ~ShopUI();

    std::unordered_map<std::string, Texture2D> splashTextures;
    void init();
    void shutdown();

    Texture2D* GetChampionSplash(const std::string& championName);
    void DrawTextureCover(Texture2D texture, Rectangle destination, Color tint = WHITE);
    void DrawShopIcon(Rectangle rect, Champion& champion, Color tierColor, bool highlighted, bool sourceHidden);

    Rectangle ShopSlotRect(int i);
    void DrawShopTrapezoid();
    Rectangle ShopBarRect();
    Rectangle TraitBarRect();
    void DrawTraitHexs();
    Rectangle ShopXpRect();
    Rectangle ShopRerollRect();
    Rectangle ShopSellRect();

    void DrawShop(vector<Champion>& shop, int hoveredShop, bool hoverXp, bool hoverReroll, const DragState& dragState, const Champion* champion);

    bool HandleShopDragPress(std::vector<Champion>& shop, int hoveredShop, Drag& drag);
    bool HandleShopDrop(Engine& engine, Drag& drag);
};
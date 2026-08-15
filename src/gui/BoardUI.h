#include <string>
#include <unordered_map>
#include "raylib.h"
#include "../Engine.h"
#include "Drag.h"
#include "ShopUI.h"
#include "TraitUI.h"
#include "../SetId.h"

class BoardUI
{
private:
    /* data */
    float r = 0.55f;
    float drawR = r * 0.90f; 
    float squareSide = r * 1.2f;

    std::unordered_map<int, Model> champModels;
    std::unordered_map<int, ModelAnimation*> champAnims;
    std::unordered_map<int, int> champAnimCounts;
    std::unordered_map<int, float> champScales;
    std::unordered_map<int, float> champYOffsets;
    std::unordered_map<int, float> champAnimFrame;
    std::unordered_map<int, float> champAnimDir;

    unordered_map<int, BoundingBox> champBounds;

public:
    BoardUI(/* args */);
    ~BoardUI();


    void init(SetId set);
    void shutdown();

    Vector3 HexCenter(int row, int col);
    Vector3 BenchCenter(int i);

    void drawHexes(int hoveredRow, int hoveredCol);
    void drawSquares(int hoveredBench);
    void drawChampions(Engine& engine, const Drag& drag, const Vector3& mousePosition, const Champion* champion);

    void drawVisuals(Engine& engine, const Drag& drag, const Vector3& mousePosition, int hoveredRow, int hoveredCol, int hoveredBench, const Champion* draggedChampion);

    void drawDraggedChampionModel(const Drag& drag, const Vector3& mousePosition, const Champion* champion);
    const Champion* GetDraggedChampion(const Drag& drag, Engine& engine) const;

    SlotRef getHoveredChampion(const Ray& ray, Engine& engine);
    void drawAllChampions();

    BoundingBox GetChampionHitbox(int id, const Vector3& pos, float scale) const;
};

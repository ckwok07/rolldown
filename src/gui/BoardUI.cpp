#include "BoardUI.h"
#include <cfloat>

BoardUI::BoardUI(/* args */)
{
}

BoardUI::~BoardUI()
{
}


void BoardUI::init(SetId set) {
    string prefix;
    string suffix;

    if (set == SetId::Set17) {
        prefix = "Set17";
        suffix = "TFT17_";
    } else {
        prefix = "Set18";
        suffix = "TFT18_";
    }
    const std::vector<Champion>& roster = (set == SetId::Set17) ? Set17::ALL_CHAMPIONS : Set18::ALL_CHAMPIONS;

    for (const Champion& champ : roster) {
        std::string assetName;
        for (char ch : champ.name) {
            if (ch != ' ' && ch != '\'') assetName += ch;
        }

        std::string path = "assets/" + prefix + "/models/" + prefix + "_" + assetName + ".glb";

        if (!FileExists(path.c_str())) continue;

        Model model = LoadModel(path.c_str());
        int count = 0;
        ModelAnimation* anims = LoadModelAnimations(path.c_str(), &count);

        BoundingBox bounds = GetModelBoundingBox(model);
        champBounds[champ.id] = bounds;
        float modelHeight = bounds.max.y - bounds.min.y;
        float baseScale = 1.2f / modelHeight;

        float scaleMultiplier = 1.0f;

        auto scaleIt = Set18::scales.find(champ.id);
        if (scaleIt != Set18::scales.end()) {
            scaleMultiplier = scaleIt->second / 100.0f;
        }

        float scale = baseScale * scaleMultiplier * 0.95f;
        float yOffset = -bounds.min.y * scale;

        champModels[champ.id]     = model;
        champAnims[champ.id]      = anims;
        champAnimCounts[champ.id] = count;
        champScales[champ.id]     = scale;
        champYOffsets[champ.id]   = yOffset;
        champAnimFrame[champ.id]  = 0.0f;
        champAnimDir[champ.id]    = 1.0f;
    }
}
void BoardUI::shutdown() {
    for (auto& e : champModels) UnloadModel(e.second);
    for (auto& e : champAnims)  UnloadModelAnimations(e.second, champAnimCounts[e.first]);
    champModels.clear();
    champAnims.clear();
}

Vector3 BoardUI::HexCenter(int row, int col) {
    float w = sqrtf(3.0f) * r;
    float cx = (col - 3) * w + (row % 2 == 0 ? -w / 4.0f : w / 4.0f);
    float cz = (row - 1.5f) * 1.5f * r;
    return { cx, 0.0f, cz };
}

Vector3 BoardUI::BenchCenter(int i) {
    float w = sqrtf(3.0f) * r;
    float squareGap = r * 0.40f;
    float boardGap = r * 0.45f;
    float halfSquare = squareSide / 2.0f;

    float boardRightX = 3.0f * w + w / 4.0f + sqrtf(3.0f) * drawR / 2.0f;
    float boardBottomZ = (3.0f - 1.5f) * 1.5f * r + drawR;
    float cz = boardBottomZ + boardGap + halfSquare;

    float cx = boardRightX - halfSquare - (9 - 1 - i) * (squareSide + squareGap);

    return { cx, 0.0f, cz };
}

void BoardUI::drawHexes(int hoveredRow, int hoveredCol) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 7; col++) {
            Vector3 center = HexCenter(row, col);
            Color hexColor = (row == hoveredRow && col == hoveredCol) ? YELLOW : SKYBLUE;

            Vector3 pts[6];

            for (int i = 0; i < 6; i++) {
                float a = (60.0f * i - 90.0f) * DEG2RAD;

                pts[i] = {
                    center.x + drawR * cosf(a),
                    0.0f,
                    center.z + drawR * sinf(a)
                };
            }

            for (int i = 0; i < 6; i++) {
                DrawCylinderEx(
                    pts[i],
                    pts[(i + 1) % 6],
                    0.018f,
                    0.018f,
                    6,
                    hexColor
                );
            }
        }
    }
}

void BoardUI::drawSquares(int hoveredBench) {
    const int squareCount = 9;
    float halfSquare = squareSide / 2.0f;

    float panelY = 0.0f;
    float tilt = 1.0f * DEG2RAD;

    for (int i = 0; i < squareCount; i++) {
        Vector3 c = BenchCenter(i);

        Color benchColor = (i == hoveredBench) ? YELLOW : SKYBLUE;

        float dz = halfSquare * cosf(tilt);
        float dy = halfSquare * sinf(tilt);

        Vector3 square[4] = {
            { c.x - halfSquare, panelY + dy, c.z - dz },
            { c.x + halfSquare, panelY + dy, c.z - dz },
            { c.x + halfSquare, panelY - dy, c.z + dz },
            { c.x - halfSquare, panelY - dy, c.z + dz }
        };

        for (int edge = 0; edge < 4; edge++) {
            DrawCylinderEx(square[edge], square[(edge + 1) % 4], 0.018f, 0.018f, 6, benchColor);
        }
    }
}

void BoardUI::drawChampions(Engine& engine, const Drag& drag, const Vector3& mousePosition, const Champion* draggedChampion) {

        for (auto& entry : champModels) {
            int id = entry.first;
            Model& model = entry.second;
            bool present = false;
            for (int row = 0; row < 4 && !present; row++)
                for (int col = 0; col < 7; col++)
                    if (engine.gamestate.board[row][col].id == id) { present = true; break; }
            for (int i = 0; i < 9 && !present; i++)
                if (engine.gamestate.bench[i].id == id) { present = true; break; }

            if (!present) continue;

            ModelAnimation* anims = champAnims[id];
            int count = champAnimCounts[id];

            if (anims != nullptr && count > 0 && anims[0].keyframeCount > 1) {
                float endFrame = (float)anims[0].keyframeCount - 2.0f;
                champAnimFrame[id] += GetFrameTime() * 30.0f * champAnimDir[id];

                if (champAnimFrame[id] >= endFrame) {
                    champAnimFrame[id] = endFrame;
                    champAnimDir[id] = -1.0f;
                } else if (champAnimFrame[id] <= 0.0f) {
                    champAnimFrame[id] = 0.0f;
                    champAnimDir[id] = 1.0f;
                }

                UpdateModelAnimation(model, anims[0], (int)champAnimFrame[id]);
            }

            float scale = champScales[id];
            float yOff  = champYOffsets[id];

            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 7; col++) {
                    if (engine.gamestate.board[row][col].id == id) {
                        SlotRef slot = {Zone::Board, -1, row, col};

                        if (drag.IsDraggedSource(slot)) {
                            continue;
                        }
                        Vector3 pos = HexCenter(row, col);
                        pos.y = yOff;

                        DrawModelEx(model, pos, {0,1,0}, 0.0f, {scale,scale,scale}, WHITE);

                        BoundingBox bounds = champBounds[id];

                        BoundingBox worldBounds = {
                            {pos.x + bounds.min.x * scale, pos.y + bounds.min.y * scale, pos.z + bounds.min.z * scale},
                            {pos.x + bounds.max.x * scale, pos.y + bounds.max.y * scale, pos.z + bounds.max.z * scale}
                        };

                        DrawBoundingBox(worldBounds, RED);
                        pos.y = yOff;
                        DrawModelEx(model, pos, {0,1,0}, 0.0f, {scale,scale,scale}, WHITE);
                    }
                }
            }

            for (int i = 0; i < 9; i++) {
                if (engine.gamestate.bench[i].id == id) {
                    SlotRef slot = {Zone::Bench, i, -1, -1};

                    if (drag.IsDraggedSource(slot)) {
                        continue;
                    }
                    Vector3 pos = BenchCenter(i);
                    pos.y = yOff;
                    DrawModelEx(model, pos, {0,1,0}, 0.0f, {scale,scale,scale}, WHITE);
                }
            }
        }
        drawDraggedChampionModel(drag, mousePosition, draggedChampion);
}

void BoardUI::drawVisuals(Engine& engine, const Drag& drag, const Vector3& mousePosition, int hoveredRow, int hoveredCol, int hoveredBench, const Champion* draggedChampion) {
    bool draggingBoardChampion =
        drag.GetState().phase == DragPhase::Dragging &&
        drag.GetState().payload == DragPayload::Champion &&
        (drag.GetState().source.zone == Zone::Bench || drag.GetState().source.zone == Zone::Board);

    if (draggingBoardChampion) {
        drawHexes(hoveredRow, hoveredCol);
        drawSquares(hoveredBench);
    }

    drawChampions(engine, drag, mousePosition, draggedChampion);
}

void BoardUI::drawDraggedChampionModel(const Drag& drag, const Vector3& mousePosition, const Champion* champion) {
    if (drag.GetState().phase != DragPhase::Dragging) return;
    if (drag.GetState().payload != DragPayload::Champion) return;
    if (drag.GetState().source.zone != Zone::Bench && drag.GetState().source.zone != Zone::Board) return;

    if (champion == nullptr || champion->id == 0) return;

    auto modelIt = champModels.find(champion->id);
    if (modelIt == champModels.end()) return;

    Vector3 position = mousePosition;

    float scale = champScales[champion->id];
    position.y = champYOffsets[champion->id];

    DrawModelEx(modelIt->second, position, {0.0f, 1.0f, 0.0f}, 0.0f, {scale, scale, scale}, WHITE);
}

SlotRef BoardUI::getHoveredChampion(const Ray& ray, Engine& engine) {
    float closestDistance = FLT_MAX;
    SlotRef hovered = {};

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 7; col++) {
            const Champion& champion = engine.gamestate.board[row][col];
            if (champion.id == 0) continue;

            auto modelIt = champModels.find(champion.id);
            if (modelIt == champModels.end()) continue;

            float scale = champScales[champion.id];
            Vector3 pos = HexCenter(row, col);
            pos.y = champYOffsets[champion.id];

            BoundingBox bounds = champBounds[champion.id];

            BoundingBox worldBounds = {
                {
                    pos.x + bounds.min.x * scale,
                    pos.y + bounds.min.y * scale,
                    pos.z + bounds.min.z * scale
                },
                {
                    pos.x + bounds.max.x * scale,
                    pos.y + bounds.max.y * scale,
                    pos.z + bounds.max.z * scale
                }
            };

            RayCollision hit = GetRayCollisionBox(ray, worldBounds);

            if (hit.hit && hit.distance < closestDistance) {
                closestDistance = hit.distance;
                hovered = {Zone::Board, -1, row, col};
            }
        }
    }

    for (int i = 0; i < 9; i++) {
        const Champion& champion = engine.gamestate.bench[i];
        if (champion.id == 0) continue;

        auto modelIt = champModels.find(champion.id);
        if (modelIt == champModels.end()) continue;

        float scale = champScales[champion.id];
        Vector3 pos = BenchCenter(i);
        pos.y = champYOffsets[champion.id];

        BoundingBox bounds = champBounds[champion.id];

        BoundingBox worldBounds = {
            {
                pos.x + bounds.min.x * scale,
                pos.y + bounds.min.y * scale,
                pos.z + bounds.min.z * scale
            },
            {
                pos.x + bounds.max.x * scale,
                pos.y + bounds.max.y * scale,
                pos.z + bounds.max.z * scale
            }
        };

        RayCollision hit = GetRayCollisionBox(ray, worldBounds);

        if (hit.hit && hit.distance < closestDistance) {
            closestDistance = hit.distance;
            hovered = {Zone::Bench, i, -1, -1};
        }
    }

    return hovered;
}
void BoardUI::drawAllChampions() {
    const int perRow = 10;
    const float spacingX = 1.25f;
    const float spacingZ = 1.55f;

    int count = min(30, (int)Set18::ALL_CHAMPIONS.size());
    int rows = (count + perRow - 1) / perRow;

    float totalWidth = (perRow - 1) * spacingX;
    float totalDepth = (rows - 1) * spacingZ;

    for (int i = 0; i < count; i++) {
        const Champion& champion = Set18::ALL_CHAMPIONS[i];
        int id = champion.id;

        auto modelIt = champModels.find(id);
        if (modelIt == champModels.end()) continue;

        Model& model = modelIt->second;

        ModelAnimation* anims = champAnims[id];
        int animCount = champAnimCounts[id];

        if (anims != nullptr && animCount > 0 && anims[0].keyframeCount > 1) {
            float endFrame = (float)anims[0].keyframeCount - 2.0f;
            champAnimFrame[id] += GetFrameTime() * 30.0f * champAnimDir[id];

            if (champAnimFrame[id] >= endFrame) {
                champAnimFrame[id] = endFrame;
                champAnimDir[id] = -1.0f;
            } else if (champAnimFrame[id] <= 0.0f) {
                champAnimFrame[id] = 0.0f;
                champAnimDir[id] = 1.0f;
            }

            UpdateModelAnimation(model, anims[0], (int)champAnimFrame[id]);
        }

        int col = i % perRow;
        int row = i / perRow;

        Vector3 pos = {
            col * spacingX - totalWidth / 2.0f,
            0.0f,
            row * spacingZ - totalDepth / 2.0f
        };

        float scale = champScales[id];
        pos.y = champYOffsets[id];

        DrawModelEx(model, pos, {0,1,0}, 0.0f, {scale,scale,scale}, WHITE);

        BoundingBox bounds = champBounds[id];

        BoundingBox worldBounds = GetChampionHitbox(id, pos, scale);
        DrawBoundingBox(worldBounds, RED);

        DrawBoundingBox(worldBounds, RED);
    }
}

BoundingBox BoardUI::GetChampionHitbox(int id, const Vector3& pos, float scale) const {
    BoundingBox bounds = champBounds.at(id);

    Vector3 hitboxScale = {1.0f, 1.0f, 1.0f};
    Vector3 hitboxOffset = {0.0f, 0.0f, 0.0f};

    auto scaleIt = Set18::hitboxScales.find(id);
    if (scaleIt != Set18::hitboxScales.end()) {
        hitboxScale = scaleIt->second;
    }

    auto offsetIt = Set18::hitboxOffsets.find(id);
    if (offsetIt != Set18::hitboxOffsets.end()) {
        hitboxOffset = offsetIt->second;
    }
    float centerX = (bounds.min.x + bounds.max.x) / 2.0f;
    float centerY = (bounds.min.y + bounds.max.y) / 2.0f;
    float centerZ = (bounds.min.z + bounds.max.z) / 2.0f;

    float width = (bounds.max.x - bounds.min.x) * scale;
    float height = (bounds.max.y - bounds.min.y) * scale;
    float depth = (bounds.max.z - bounds.min.z) * scale;

    float halfX = width / 2.0f * hitboxScale.x;
    float halfY = height / 2.0f * hitboxScale.y;
    float halfZ = depth / 2.0f * hitboxScale.z;

    float offsetX = width * hitboxOffset.x;
    float offsetY = height * hitboxOffset.y;
    float offsetZ = depth * hitboxOffset.z;
    float minY = pos.y + bounds.min.y * scale;
    float maxY = minY + height * hitboxScale.y;

    return {
        {
            pos.x + centerX * scale + offsetX - halfX,
            minY + offsetY,
            pos.z + centerZ * scale + offsetZ - halfZ
        },
        {
            pos.x + centerX * scale + offsetX + halfX,
            maxY + offsetY,
            pos.z + centerZ * scale + offsetZ + halfZ
        }
    };
}
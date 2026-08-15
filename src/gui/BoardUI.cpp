#include "BoardUI.h"
#include <cfloat>
#include "rlgl.h"

static BoundingBox BuildDragHitbox(int id, const BoundingBox& bounds, const Vector3& pos, float scale) {
    Vector3 hitboxScale = {1.0f, 1.0f, 1.0f};
    Vector3 hitboxOffset = {0.0f, 0.0f, 0.0f};

    auto scaleIt = Set18::hitboxScales.find(id);
    if (scaleIt != Set18::hitboxScales.end()) hitboxScale = scaleIt->second;

    auto offsetIt = Set18::hitboxOffsets.find(id);
    if (offsetIt != Set18::hitboxOffsets.end()) hitboxOffset = offsetIt->second;

    float width = (bounds.max.x - bounds.min.x) * scale;
    float height = (bounds.max.y - bounds.min.y) * scale;
    float depth = (bounds.max.z - bounds.min.z) * scale;

    float centerX = pos.x + ((bounds.min.x + bounds.max.x) / 2.0f) * scale;
    float centerZ = pos.z + ((bounds.min.z + bounds.max.z) / 2.0f) * scale;

    float halfX = width / 2.0f * hitboxScale.x;
    float halfZ = depth / 2.0f * hitboxScale.z;

    float offsetX = width * hitboxOffset.x;
    float offsetY = height * hitboxOffset.y;
    float offsetZ = depth * hitboxOffset.z;

    float minY = pos.y + bounds.min.y * scale + offsetY;
    float maxY = minY + height * hitboxScale.y;

    return {
        {centerX + offsetX - halfX, minY, centerZ + offsetZ - halfZ},
        {centerX + offsetX + halfX, maxY, centerZ + offsetZ + halfZ}
    };
}

static bool IntersectRayPlane(const Ray& ray, const Vector3& planePoint, const Vector3& planeNormal, Vector3& result) {
    float denom = ray.direction.x * planeNormal.x + ray.direction.y * planeNormal.y + ray.direction.z * planeNormal.z;
    if (fabsf(denom) < 0.0001f) return false;

    Vector3 diff = {
        planePoint.x - ray.position.x,
        planePoint.y - ray.position.y,
        planePoint.z - ray.position.z
    };

    float t = (diff.x * planeNormal.x + diff.y * planeNormal.y + diff.z * planeNormal.z) / denom;
    if (t < 0.0f) return false;

    result = {
        ray.position.x + ray.direction.x * t,
        ray.position.y + ray.direction.y * t,
        ray.position.z + ray.direction.z * t
    };

    return true;
}

static Vector3 GetCenteredDragPosition(int id, const BoundingBox& bounds, float scale, const Camera3D& camera) {
    float centerX = (bounds.min.x + bounds.max.x) / 2.0f;
    float centerY = (bounds.min.y + bounds.max.y) / 2.0f;
    float centerZ = (bounds.min.z + bounds.max.z) / 2.0f;

    float modelHeight = (bounds.max.y - bounds.min.y) * scale;
    float centerHeight = modelHeight * 0.5f;

    Ray mouseRay = GetScreenToWorldRay(GetMousePosition(), camera);

    float t = (centerHeight - mouseRay.position.y) / mouseRay.direction.y;

    Vector3 mouseCenter = {
        mouseRay.position.x + mouseRay.direction.x * t,
        centerHeight,
        mouseRay.position.z + mouseRay.direction.z * t
    };

    Vector3 position = {
        mouseCenter.x - centerX * scale,
        mouseCenter.y - centerY * scale,
        mouseCenter.z - centerZ * scale
    };

    for (int iteration = 0; iteration < 3; iteration++) {
        BoundingBox box = BuildDragHitbox(id, bounds, position, scale);

        Vector3 corners[8] = {
            {box.min.x, box.min.y, box.min.z},
            {box.max.x, box.min.y, box.min.z},
            {box.min.x, box.max.y, box.min.z},
            {box.max.x, box.max.y, box.min.z},
            {box.min.x, box.min.y, box.max.z},
            {box.max.x, box.min.y, box.max.z},
            {box.min.x, box.max.y, box.max.z},
            {box.max.x, box.max.y, box.max.z}
        };

        float minX = FLT_MAX;
        float minY = FLT_MAX;
        float maxX = -FLT_MAX;
        float maxY = -FLT_MAX;

        for (int i = 0; i < 8; i++) {
            Vector2 screen = GetWorldToScreen(corners[i], camera);

            if (screen.x < minX) minX = screen.x;
            if (screen.x > maxX) maxX = screen.x;
            if (screen.y < minY) minY = screen.y;
            if (screen.y > maxY) maxY = screen.y;
        }

        Vector2 screenCenter = {
            (minX + maxX) * 0.5f,
            (minY + maxY) * 0.5f
        };

        Vector2 mouse = GetMousePosition();

        if (fabsf(mouse.x - screenCenter.x) < 0.1f && fabsf(mouse.y - screenCenter.y) < 0.1f) break;

        Vector3 boxCenter = {
            (box.min.x + box.max.x) * 0.5f,
            (box.min.y + box.max.y) * 0.5f,
            (box.min.z + box.max.z) * 0.5f
        };

        Vector3 planeNormal = {
            camera.target.x - camera.position.x,
            camera.target.y - camera.position.y,
            camera.target.z - camera.position.z
        };

        float length = sqrtf(planeNormal.x * planeNormal.x + planeNormal.y * planeNormal.y + planeNormal.z * planeNormal.z);

        planeNormal.x /= length;
        planeNormal.y /= length;
        planeNormal.z /= length;

        Ray centerRay = GetScreenToWorldRay(screenCenter, camera);
        Ray targetRay = GetScreenToWorldRay(mouse, camera);

        Vector3 currentWorld;
        Vector3 targetWorld;

        if (!IntersectRayPlane(centerRay, boxCenter, planeNormal, currentWorld)) break;
        if (!IntersectRayPlane(targetRay, boxCenter, planeNormal, targetWorld)) break;

        position.x += targetWorld.x - currentWorld.x;
        position.y += targetWorld.y - currentWorld.y;
        position.z += targetWorld.z - currentWorld.z;
    }

    return position;
}

BoardUI::BoardUI(/* args */)
{
}

BoardUI::~BoardUI()
{
}


void BoardUI::init(SetId set) {
    outlineMask = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    SetTextureFilter(outlineMask.texture, TEXTURE_FILTER_BILINEAR);
    outlineShader = LoadShader(0, "assets/shaders/outline.fs");
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
    if (outlineMask.id != 0) {
        UnloadRenderTexture(outlineMask);
        outlineMask = {};
    }
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

void BoardUI::drawChampions(Engine& engine, const Drag& drag, const Camera3D& camera, const Champion* draggedChampion, int hoveredRow, int hoveredCol, int hoveredBench) {
    for (auto& entry : champModels) {
        int id = entry.first;
        Model& model = entry.second;

        bool present = false;

        for (int row = 0; row < 4 && !present; row++) {
            for (int col = 0; col < 7; col++) {
                if (engine.gamestate.board[row][col].id == id) {
                    present = true;
                    break;
                }
            }
        }

        for (int i = 0; i < 9 && !present; i++) {
            if (engine.gamestate.bench[i].id == id) {
                present = true;
                break;
            }
        }

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
        float yOff = champYOffsets[id];

        auto groundIt = Set18::groundOffsets.find(id);
        if (groundIt != Set18::groundOffsets.end()) {
            BoundingBox bounds = champBounds[id];
            float modelHeight = (bounds.max.y - bounds.min.y) * scale;
            yOff += modelHeight * groundIt->second;
        }

        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 7; col++) {
                Champion& champion = engine.gamestate.board[row][col];
                if (champion.id != id) continue;

                SlotRef slot = {Zone::Board, -1, row, col};
                if (drag.IsDraggedSource(slot)) continue;

                Vector3 pos = HexCenter(row, col);
                pos.y = yOff;

                bool hovered = row == hoveredRow && col == hoveredCol;
                drawChampionModel(model, pos, scale, hovered);

                Vector3 hitboxPos = HexCenter(row, col);
                hitboxPos.y = champYOffsets[id];

                BoundingBox hitbox = GetChampionHitbox(id, hitboxPos, scale);
                DrawBoundingBox(hitbox, RED);
                drawHealthBar(champion, hitbox, camera);
            }
        }

        for (int i = 0; i < 9; i++) {
            Champion& champion = engine.gamestate.bench[i];
            if (champion.id != id) continue;

            SlotRef slot = {Zone::Bench, i, -1, -1};
            if (drag.IsDraggedSource(slot)) continue;

            Vector3 pos = BenchCenter(i);
            pos.y = yOff;

            bool hovered = i == hoveredBench;
            drawChampionModel(model, pos, scale, hovered);

            Vector3 hitboxPos = BenchCenter(i);
            hitboxPos.y = champYOffsets[id];

            BoundingBox hitbox = GetChampionHitbox(id, hitboxPos, scale);
            drawHealthBar(champion, hitbox, camera);
        }
    }

    drawDraggedChampionModel(drag, camera, draggedChampion);
}

void BoardUI::drawVisuals(Engine& engine, const Drag& drag, const Camera3D& camera, const Vector3& mousePosition, int hoveredRow, int hoveredCol, int hoveredBench, const Champion* draggedChampion) {
    renderHoveredChampionMask(engine, drag, camera, mousePosition, hoveredRow, hoveredCol, hoveredBench, draggedChampion);

    BeginMode3D(camera);

    rlDisableBackfaceCulling();

    bool draggingBoardChampion =
        drag.GetState().phase == DragPhase::Dragging &&
        drag.GetState().payload == DragPayload::Champion &&
        (drag.GetState().source.zone == Zone::Bench || drag.GetState().source.zone == Zone::Board);

    if (draggingBoardChampion) {
        drawHexes(hoveredRow, hoveredCol);
        drawSquares(hoveredBench);
    }

    drawChampions(engine, drag, camera, draggedChampion, hoveredRow, hoveredCol, hoveredBench);

    rlEnableBackfaceCulling();

    EndMode3D();

    drawChampionOutline();
}

SlotRef BoardUI::getHoveredChampion(const Ray& ray, Engine& engine) {
    float closestDistance = FLT_MAX;
    SlotRef hovered = {};

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 7; col++) {
            const Champion& champion = engine.gamestate.board[row][col];
            if (champion.id == 0) continue;

            int id = champion.id;

            auto modelIt = champModels.find(id);
            if (modelIt == champModels.end()) continue;

            float scale = champScales[id];

            Vector3 pos = HexCenter(row, col);
            pos.y = champYOffsets[id];

            BoundingBox worldBounds = GetChampionHitbox(id, pos, scale);
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

        int id = champion.id;

        auto modelIt = champModels.find(id);
        if (modelIt == champModels.end()) continue;

        float scale = champScales[id];

        Vector3 pos = BenchCenter(i);
        pos.y = champYOffsets[id];

        BoundingBox worldBounds = GetChampionHitbox(id, pos, scale);
        RayCollision hit = GetRayCollisionBox(ray, worldBounds);

        if (hit.hit && hit.distance < closestDistance) {
            closestDistance = hit.distance;
            hovered = {Zone::Bench, i, -1, -1};
        }
    }

    return hovered;
}

void BoardUI::drawAllChampions() {
    const int startIndex = 30;
    const int amount = 30;
    const int perRow = 10;
    const float spacingX = 1.25f;
    const float spacingZ = 1.55f;

    int endIndex = min(startIndex + amount, (int)Set18::ALL_CHAMPIONS.size());
    int count = endIndex - startIndex;
    int rows = (count + perRow - 1) / perRow;

    float totalWidth = (perRow - 1) * spacingX;
    float totalDepth = (rows - 1) * spacingZ;

    DrawGrid(30, 1.0f);

    for (int i = startIndex; i < endIndex; i++) {
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

        int displayIndex = i - startIndex;
        int col = displayIndex % perRow;
        int row = displayIndex / perRow;

        Vector3 basePos = {
            col * spacingX - totalWidth / 2.0f,
            0.0f,
            row * spacingZ - totalDepth / 2.0f
        };

        float scale = champScales[id];

        BoundingBox bounds = champBounds[id];
        float modelHeight = (bounds.max.y - bounds.min.y) * scale;

        float groundOffset = 0.0f;

        auto groundIt = Set18::groundOffsets.find(id);
        if (groundIt != Set18::groundOffsets.end()) {
            groundOffset = groundIt->second;
        }

        // Hitbox position does NOT use groundOffset
        Vector3 hitboxPos = basePos;
        hitboxPos.y = champYOffsets[id];

        // Model position DOES use groundOffset
        Vector3 modelPos = hitboxPos;
        modelPos.y += modelHeight * groundOffset;

        // Solid flattened mesh on actual ground
        Vector3 shadowPos = {modelPos.x, 0.01f, modelPos.z};
        DrawModelEx(model, shadowPos, {0,1,0}, 0.0f, {scale, 0.001f, scale}, BLACK);

        // Actual champion
        DrawModelEx(model, modelPos, {0,1,0}, 0.0f, {scale,scale,scale}, WHITE);

        // Hitbox remains unaffected by groundOffset
        BoundingBox worldBounds = GetChampionHitbox(id, hitboxPos, scale);
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

    float width = (bounds.max.x - bounds.min.x) * scale;
    float height = (bounds.max.y - bounds.min.y) * scale;
    float depth = (bounds.max.z - bounds.min.z) * scale;

    float centerX = pos.x + ((bounds.min.x + bounds.max.x) / 2.0f) * scale;
    float centerZ = pos.z + ((bounds.min.z + bounds.max.z) / 2.0f) * scale;

    float halfX = width / 2.0f * hitboxScale.x;
    float halfZ = depth / 2.0f * hitboxScale.z;

    float offsetX = width * hitboxOffset.x;
    float offsetY = height * hitboxOffset.y;
    float offsetZ = depth * hitboxOffset.z;

    float minY = pos.y + bounds.min.y * scale + offsetY;
    float maxY = minY + height * hitboxScale.y;

    return {
        {centerX + offsetX - halfX, minY, centerZ + offsetZ - halfZ},
        {centerX + offsetX + halfX, maxY, centerZ + offsetZ + halfZ}
    };
}

void BoardUI::drawDraggedChampionModel(const Drag& drag, const Camera3D& camera, const Champion* champion) {
    if (drag.GetState().phase != DragPhase::Dragging) return;
    if (drag.GetState().payload != DragPayload::Champion) return;
    if (drag.GetState().source.zone != Zone::Bench && drag.GetState().source.zone != Zone::Board) return;
    if (champion == nullptr || champion->id == 0) return;

    int id = champion->id;

    auto modelIt = champModels.find(id);
    if (modelIt == champModels.end()) return;

    float scale = champScales[id];
    Vector3 position = GetCenteredDragPosition(id, champBounds[id], scale, camera);

    DrawModelEx(modelIt->second, position, {0,1,0}, 0.0f, {scale,scale,scale}, WHITE);

    Vector3 hitboxPos = position;
    BoundingBox hitbox = GetChampionHitbox(id, hitboxPos, scale);

    drawHealthBar(*champion, hitbox, camera);
}

void BoardUI::drawChampionModel(Model& model, const Vector3& pos, float scale, bool hovered) {
    DrawModelEx(model, pos, {0,1,0}, 0.0f, {scale,scale,scale}, WHITE);
}

void BoardUI::renderHoveredChampionMask(Engine& engine, const Drag& drag, const Camera3D& camera, const Vector3& mousePosition, int hoveredRow, int hoveredCol, int hoveredBench, const Champion* draggedChampion) {
    const Champion* champion = nullptr;
    Vector3 pos = {0.0f, 0.0f, 0.0f};

    bool draggingChampion =
        drag.GetState().phase == DragPhase::Dragging &&
        drag.GetState().payload == DragPayload::Champion &&
        (drag.GetState().source.zone == Zone::Board || drag.GetState().source.zone == Zone::Bench);

    if (draggingChampion && draggedChampion != nullptr) {
        champion = draggedChampion;

        int id = champion->id;
        float scale = champScales[id];

        pos = GetCenteredDragPosition(id, champBounds[id], scale, camera);
    } else if (hoveredRow != -1 && hoveredCol != -1 && engine.gamestate.board[hoveredRow][hoveredCol].id != 0) {
        champion = &engine.gamestate.board[hoveredRow][hoveredCol];
        pos = HexCenter(hoveredRow, hoveredCol);
    } else if (hoveredBench != -1 && engine.gamestate.bench[hoveredBench].id != 0) {
        champion = &engine.gamestate.bench[hoveredBench];
        pos = BenchCenter(hoveredBench);
    }

    BeginTextureMode(outlineMask);
    ClearBackground(BLANK);

    if (champion != nullptr && champion->id != 0) {
        int id = champion->id;

        auto modelIt = champModels.find(id);

        if (modelIt != champModels.end()) {
            float scale = champScales[id];

            if (!draggingChampion) {
                float yOff = champYOffsets[id];

                auto groundIt = Set18::groundOffsets.find(id);
                if (groundIt != Set18::groundOffsets.end()) {
                    BoundingBox bounds = champBounds[id];
                    float modelHeight = (bounds.max.y - bounds.min.y) * scale;
                    yOff += modelHeight * groundIt->second;
                }

                pos.y = yOff;
            }

            BeginMode3D(camera);

            rlDisableBackfaceCulling();

            DrawModelEx(modelIt->second, pos, {0,1,0}, 0.0f, {scale,scale,scale}, WHITE);

            rlEnableBackfaceCulling();

            EndMode3D();
        }
    }

    EndTextureMode();
}

void BoardUI::drawChampionOutline() {
    Vector2 texelSize = {
        1.0f / outlineMask.texture.width,
        1.0f / outlineMask.texture.height
    };

    int texelLoc = GetShaderLocation(outlineShader, "texelSize");
    SetShaderValue(outlineShader, texelLoc, &texelSize, SHADER_UNIFORM_VEC2);

    BeginShaderMode(outlineShader);

    DrawTextureRec(
        outlineMask.texture,
        {0.0f, 0.0f, (float)outlineMask.texture.width, -(float)outlineMask.texture.height},
        {0.0f, 0.0f},
        WHITE
    );

    EndShaderMode();
}

void BoardUI::drawHealthBar(const Champion& champion, const BoundingBox& hitbox, const Camera3D& camera) {
    Vector3 anchor = {(hitbox.min.x + hitbox.max.x) * 0.5f, hitbox.max.y, (hitbox.min.z + hitbox.max.z) * 0.5f};
    Vector2 screen = GetWorldToScreen(anchor, camera);

    float width = 70.0f;
    float height = 8.0f;
    float offsetY = 25.0f;

    Rectangle bar = {screen.x - width * 0.5f, screen.y - offsetY - height, width, height};

    EndMode3D();
    DrawRectangleRec(bar, SKYBLUE);
    BeginMode3D(camera);
}
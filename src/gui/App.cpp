#include "App.h"
#include <iostream>
#include "raylib.h"
#include <cmath>
#include "rlgl.h"
#include "raymath.h"
#include "../SetId.h"

static bool SameSlot(const SlotRef& a, const SlotRef& b) {
    if (a.zone != b.zone) return false;

    if (a.zone == Zone::Board) {
        return a.row == b.row && a.col == b.col;
    }

    return a.index == b.index;
}

App::App() {}

App::~App() {
    shutdown();
}

bool App::init() {
    engine.initGameState(SetId::Set18);

    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_TOPMOST);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(1792, 1008, "Rolldown Simulator");
    shop.init();
    traits.init();

    background = LoadTexture("assets/image2.png");
    camera.position = { 0.2f, 12.0f, 9.0f };
    camera.target   = { 0.2f, 0.0f, 0.0f };
    camera.up       = { 0.0f, 1.0f, 0.0f };
    camera.fovy     = 35.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    string prefix;
    string suffix;

    if (engine.gamestate.activeSet == SetId::Set17) {
        prefix = "Set17";
        suffix = "TFT17_";
    } else {
        prefix = "Set18";
        suffix = "TFT18_";
    }

    const unordered_map<int, float> set18ScalePercent = {
        {Set18::Akali.id, 85.0f},
        {Set18::Camille.id, 100.0f},
        {Set18::Cinderling.id, 40.0f},
        {Set18::Karma.id, 100.0f},
        {Set18::Kobuko.id, 75.0f},
        {Set18::Leona.id, 100.0f},
        {Set18::Ornn.id, 100.0f},
        {Set18::Pebbles.id, 100.0f},
        {Set18::Rakan.id, 100.0f},
        {Set18::RekSai.id, 150.0f},
        {Set18::Varus.id, 105.0f},
        {Set18::Veigar.id, 80.0f},
        {Set18::Xayah.id, 95.0f},
        {Set18::Yorick.id, 110.0f},

        {Set18::Alistar.id, 100.0f},
        {Set18::Caitlyn.id, 85.0f},
        {Set18::Elise.id, 230.0f},
        {Set18::Gromp.id, 75.0f},
        {Set18::Kayle.id, 120.0f},
        {Set18::LeBlanc.id, 95.0f},
        {Set18::Murkwolf.id, 40.0f},
        {Set18::Scuttlecrab.id, 40.0f},
        {Set18::Sejuani.id, 100.0f},
        {Set18::Shen.id, 85.0f},
        {Set18::Teemo.id, 55.0f},
        {Set18::Warwick.id, 85.0f},
        {Set18::Yunara.id, 90.0f},

        {Set18::Azir.id, 180.0f},
        {Set18::Cassiopeia.id, 85.0f},
        {Set18::Diana.id, 95.0f},
        {Set18::Fiddlesticks.id, 200.0f},
        {Set18::Hecarim.id, 160.0f},
        {Set18::KhaZix.id, 75.0f},
        {Set18::KogMaw.id, 80.0f},
        {Set18::Krug.id, 60.0f},
        {Set18::MamaBeak.id, 60.0f},
        {Set18::MasterYi.id, 90.0f},
        {Set18::Rammus.id, 200.0f},
        {Set18::Rengar.id, 97.0f},
        {Set18::Tristana.id, 60.0f},
        {Set18::Vi.id, 95.0f},

        {Set18::Ahri.id, 100.0f},
        {Set18::Amumu.id, 75.0f},
        {Set18::Aphelios.id, 180.0f},
        {Set18::Brambleback.id, 75.0f},
        {Set18::Ezreal.id, 80.0f},
        {Set18::Lillia.id, 100.0f},
        {Set18::Malphite.id, 100.0f},
        {Set18::Morgana.id, 90.0f},
        {Set18::Nidalee.id, 95.0f},
        {Set18::Sentinel.id, 75.0f},
        {Set18::Sett.id, 100.0f},
        {Set18::Sivir.id, 80.0f},
        {Set18::Soraka.id, 240.0f},
        {Set18::Zyra.id, 87.0f},

        {Set18::Alune.id, 100.0f},
        {Set18::Ashe.id, 102.0f},
        {Set18::Draven.id, 85.0f},
        {Set18::ElderDragon.id, 140.0f},
        {Set18::Gnar.id, 70.0f},
        {Set18::Ivern.id, 98.0f},
        {Set18::Kennen.id, 70.0f},
        {Set18::Lux.id, 80.0f},
        {Set18::Maokai.id, 100.0f},
        {Set18::Taric.id, 85.0f}
    };

    const std::vector<Champion>& roster = (engine.gamestate.activeSet == SetId::Set17) ? Set17::ALL_CHAMPIONS : Set18::ALL_CHAMPIONS;

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
        float modelHeight = bounds.max.y - bounds.min.y;
        float baseScale = 1.2f / modelHeight;

        float scaleMultiplier = 1.0f;

        auto scaleIt = set18ScalePercent.find(champ.id);
        if (scaleIt != set18ScalePercent.end()) {
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
    return true;
}

Vector3 App::HexCenter(int row, int col) {
    float w = sqrtf(3.0f) * r;
    float cx = (col - 3) * w + (row % 2 == 0 ? -w / 4.0f : w / 4.0f);
    float cz = (row - 1.5f) * 1.5f * r;
    return { cx, 0.0f, cz };
}

Vector3 App::BenchCenter(int i) {
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


// Rectangle App::TraitBarRect() {
//     float H = (float)GetScreenHeight();
//     float W = (float)GetScreenWidth();

//     float barH   = H * 0.54;
//     float barW   = barH * 0.145;
//     float bottom = 0;

//     return { 0.0f, (H - barH) / 2.15f, barW, barH };
// }

// void App::DrawTraitHexs() {
//     const auto* traitTable = &Set18::ALL_TRAITS;
//     const auto* nameTable = &Set18::TRAIT_ID_TO_NAME;

//     if (engine.gamestate.activeSet == SetId::Set17) {
//         // traitTable = &Set17::ALL_TRAITS;
//         // nameTable = &Set17::TRAIT_ID_TO_NAME;
//     }

//     static int scrollOffset = 0;

//     Rectangle bar = TraitBarRect();
//     const float top = bar.y + bar.height * 0.085f;
//     const float slotH = bar.height * 0.086f;
//     const float cx = bar.width * 0.92f;
//     const int total = (int)engine.gamestate.activeTraits.size();

//     int index = 0;
//     int drawn = 0;

//     for (const auto& entry : engine.gamestate.activeTraits) {
//         const int traitId    = entry.id;
//         const int traitCount = entry.count;
//         if (index < scrollOffset) {
//             index++;
//             continue;
//         }

//         if (drawn >= 9) break;

//         const float cy = top + drawn * slotH + slotH / 2.0f;

//         std::string thresholds;

//         auto tiersIt = traitTable->find(traitId);
//         if (tiersIt != traitTable->end()) {
//             for (size_t i = 0; i < tiersIt->second.first.size(); i++) {
//                 if (i > 0) thresholds += " > ";
//                 thresholds += std::to_string(tiersIt->second.first[i]);
//             }
//         }

//         auto nameIt = nameTable->find(traitId);
//         const std::string name = nameIt != nameTable->end() ? nameIt->second : "";

//         const float rowH = slotH * 0.75f;
//         const float pad = rowH * 0.2f;
//         const float hexRadius = bar.width * 0.23f;
//         const float numberH = rowH * 0.62f;
//         const float numberW = numberH * 0.9f;
//         const float numberX = cx + hexRadius + rowH * 0.16f;
//         const float textX = numberX + numberW + pad;

//         const int textW = MeasureText(name.c_str(), 14) > MeasureText(thresholds.c_str(), 12)
//             ? MeasureText(name.c_str(), 14)
//             : MeasureText(thresholds.c_str(), 12);

//         DrawRectangle((int)cx, (int)(cy - rowH / 2.0f), (int)((textX - cx) + textW + pad), (int)rowH, Fade(DARKGRAY, 0.6f));
//         DrawRectangle((int)numberX, (int)(cy - numberH / 2.0f), (int)numberW, (int)numberH, Fade(WHITE, 0.5f));

//         const char* numberText = TextFormat("%d", traitCount);
//         Vector2 numberSize = MeasureTextEx(GetFontDefault(), numberText, 14.0f, 1.0f);

//         DrawText(numberText, (int)(numberX + numberW / 2.0f - numberSize.x / 2.0f), (int)(cy - numberSize.y / 2.0f), 14, WHITE);

//         Vector2 nameSize = MeasureTextEx(GetFontDefault(), name.c_str(), 13.0f, 1.0f);
//         Vector2 thresholdSize = MeasureTextEx(GetFontDefault(), thresholds.c_str(), 11.0f, 1.0f);

//         const float thresholdY = cy + rowH / 2.0f - rowH * 0.12f - thresholdSize.y;
//         const float nameY = thresholdY - 1.0f - nameSize.y;

//         DrawText(name.c_str(), (int)textX, (int)nameY, 13, WHITE);
//         DrawText(thresholds.c_str(), (int)textX, (int)thresholdY, 11, GRAY);
//         DrawPoly({cx, cy}, 6, hexRadius, 30.0f, SKYBLUE);

//         drawn++;
//         index++;
//     }

//     const int remaining = total - scrollOffset - drawn;
//     const float size = bar.width * 0.21f;
//     const float overflowY = top + 9.0f * slotH + slotH / 2.0f;

//     if (remaining > 0) {
//         Vector2 points[5] = {
//             {cx - size, overflowY + size * 0.4f},
//             {cx, overflowY + size},
//             {cx + size, overflowY + size * 0.4f},
//             {cx + size, overflowY - size},
//             {cx - size, overflowY - size}
//         };

//         DrawTriangleFan(points, 5, SKYBLUE);
//     }

//     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
//         Rectangle overflowRect = {cx - size, overflowY - size, size * 2.0f, size * 2.0f};
//         bool clickedOverflow = remaining > 0 && CheckCollisionPointRec(GetMousePosition(), overflowRect);

//         scrollOffset = clickedOverflow
//             ? (scrollOffset + 9 >= total ? 0 : scrollOffset + 9)
//             : 0;
//     }
// }


void App::HandleDragPress(int hoveredShop, int hoveredBench, int hoveredRow, int hoveredCol) {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;
    if (drag.GetState().phase != DragPhase::Idle) return;

    if (shop.HandleShopDragPress(engine.gamestate.shop, hoveredShop, drag)) {
        return;
    }

    if (hoveredBench != -1 && engine.gamestate.bench[hoveredBench] != nullChamp) {
        drag.BeginDrag(DragPayload::Champion, {Zone::Bench, hoveredBench, -1, -1});
        return;
    }

    if (hoveredRow != -1 && hoveredCol != -1 && engine.gamestate.board[hoveredRow][hoveredCol] != nullChamp) {
        drag.BeginDrag(DragPayload::Champion, {Zone::Board, -1, hoveredRow, hoveredCol});
    }
}

SlotRef App::GetDropTarget(int hoveredBench, int hoveredRow, int hoveredCol) {
    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, shop.ShopSellRect())) {
        return {Zone::SellArea, -1, -1, -1};
    }

    if (hoveredBench != -1) {
        return {Zone::Bench, hoveredBench, -1, -1};
    }

    if (hoveredRow != -1 && hoveredCol != -1) {
        return {Zone::Board, -1, hoveredRow, hoveredCol};
    }

    return {};
}

void App::HandleChampionDrop(const SlotRef& target) {
    if (shop.HandleShopDrop(engine, drag)) {
        return;
    }

    const SlotRef source = drag.GetState().source;

    if (SameSlot(source, target)) {
        return;
    }

    if (source.zone == Zone::Bench) {
        if (target.zone == Zone::SellArea) {
            engine.sellbench(source.index);
        } else if (target.zone == Zone::Bench) {
            engine.benchtobench(source.index, target.index);
        } else if (target.zone == Zone::Board) {
            engine.benchtoboard(source.index, {target.row, target.col});
        }

        return;
    }

    if (source.zone == Zone::Board) {
        std::pair<int, int> sourceCell = {source.row, source.col};

        if (target.zone == Zone::SellArea) {
            engine.sellboard(sourceCell);
        } else if (target.zone == Zone::Bench) {
            engine.boardtobench(sourceCell, target.index);
        } else if (target.zone == Zone::Board) {
            engine.boardtoboard(sourceCell, {target.row, target.col});
        }
    }
}

void App::HandleDragRelease(int hoveredBench, int hoveredRow, int hoveredCol, int hoveredItem) {
    if (drag.GetState().phase == DragPhase::Idle) return;
    if (!IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) return;

    SlotRef target = GetDropTarget(hoveredBench, hoveredRow, hoveredCol);

    if (drag.GetState().payload == DragPayload::Champion) {
        HandleChampionDrop(target);
    }

    if (drag.GetState().payload == DragPayload::Item) {
        const SlotRef source = drag.GetState().source;

        if (source.zone == Zone::Inventory && hoveredItem != -1 && hoveredItem < engine.gamestate.items.size() && hoveredItem != source.index) {
            std::swap(engine.gamestate.items[source.index], engine.gamestate.items[hoveredItem]);
        }
    }

    drag.ResetDrag();
}


const Champion* App::GetDraggedChampion() const {
    if (drag.GetState().phase != DragPhase::Dragging) return nullptr;
    if (drag.GetState().payload != DragPayload::Champion) return nullptr;

    if (drag.GetState().source.zone == Zone::Shop) {
        return &engine.gamestate.shop[drag.GetState().source.index];
    }

    if (drag.GetState().source.zone == Zone::Bench) {
        return &engine.gamestate.bench[drag.GetState().source.index];
    }

    if (drag.GetState().source.zone == Zone::Board) {
        return &engine.gamestate.board[drag.GetState().source.row][drag.GetState().source.col];
    }

    return nullptr;
}

bool App::GetMouseGroundPosition(Vector3& position) const {
    Ray ray = GetScreenToWorldRay(GetMousePosition(), camera);

    if (fabsf(ray.direction.y) < 0.0001f) {
        return false;
    }

    float t = -ray.position.y / ray.direction.y;

    if (t < 0.0f) {
        return false;
    }

    position = {
        ray.position.x + ray.direction.x * t,
        0.0f,
        ray.position.z + ray.direction.z * t
    };

    return true;
}

void App::DrawDraggedChampionModel() {
    if (drag.GetState().phase != DragPhase::Dragging) return;
    if (drag.GetState().payload != DragPayload::Champion) return;

    if (drag.GetState().source.zone != Zone::Bench && drag.GetState().source.zone != Zone::Board) {
        return;
    }

    const Champion* champion = GetDraggedChampion();

    if (champion == nullptr || champion->id == 0) {
        return;
    }

    auto modelIt = champModels.find(champion->id);

    if (modelIt == champModels.end()) {
        return;
    }

    Vector3 position;

    if (!GetMouseGroundPosition(position)) {
        return;
    }

    float scale = champScales[champion->id];
    position.y = champYOffsets[champion->id];

    DrawModelEx(modelIt->second, position, {0.0f, 1.0f, 0.0f}, 0.0f, {scale, scale, scale}, WHITE);
}

void App::run() {
    while (!WindowShouldClose()) {
        // get mouse position
        int hoveredRow = -1;
        int hoveredCol = -1;
        int hoveredBench = -1;
        int hoveredShop = -1;
        int hoveredItem = traits.GetHoveredItemSlot(GetMousePosition());

        bool hoverXp = CheckCollisionPointRec(GetMousePosition(), shop.ShopXpRect());
        bool hoverReroll = CheckCollisionPointRec(GetMousePosition(), shop.ShopRerollRect());
        bool hoverLock = CheckCollisionPointRec(GetMousePosition(), shop.LockRect());

        Ray ray = GetScreenToWorldRay(GetMousePosition(), camera);
        for (int i = 0; i < 5; i++) {
            if (CheckCollisionPointRec(GetMousePosition(), shop.ShopSlotRect(i))) {
                hoveredShop = i;
                break;
            }
        }

        if (!hoverXp && !hoverReroll && hoveredShop == -1 && ray.direction.y < 0.0f) {
            
            float t = -ray.position.y / ray.direction.y;
            float hx = ray.position.x + t * ray.direction.x;
            float hz = ray.position.z + t * ray.direction.z;

            float best = r * r;

            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 7; col++) {
                    Vector3 c = HexCenter(row, col);
                    float dx = hx - c.x;
                    float dz = hz - c.z;
                    float d2 = dx * dx + dz * dz;

                    if (d2 < best) {
                        best = d2;
                        hoveredRow = row;
                        hoveredCol = col;
                    }
                }
            }

            float hitboxPadding = 0.12f;
            float halfSq = squareSide / 2.0f + hitboxPadding;

            for (int i = 0; i < 9; i++) {
                Vector3 c = BenchCenter(i);
                if (fabsf(hx - c.x) <= halfSq && fabsf(hz - c.z) <= halfSq) {
                    hoveredBench = i;
                    break;
                }
            }
        }
        
        HandleDragPress(hoveredShop, hoveredBench, hoveredRow, hoveredCol);
        HandleDragRelease(hoveredBench, hoveredRow, hoveredCol, hoveredItem);
        
        
        BeginDrawing();
        ClearBackground(GRAY);

        DrawTexturePro(
            background,
            { 0, 0, (float)background.width, (float)background.height },
            { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
            { 0, 0 },
            0.0f,
            Fade(WHITE, 0.3f)
        );

        BeginMode3D(camera);
    //     {
    //     std::vector<Champion> sorted = Set18::ALL_CHAMPIONS;
    //     // std::sort(sorted.begin(), sorted.end(),
    //     //    [](const Champion& a, const Champion& b) { return a.name < b.name; });

    //     const int perRow = 12;
    //     const float spacing = 1.0f;

    //     rlDisableBackfaceCulling();

    //     for (int i = 0; i < (int)sorted.size(); i++) {
    //         const Champion& champ = sorted[i];

    //         int col = i % perRow;
    //         int row = i / perRow;
    //         Vector3 pos = {
    //             (col - perRow / 2) * spacing,
    //             0.0f,
    //             (row - 2) * spacing
    //         };
    //         DrawCylinder({ pos.x, 0.0f, pos.z }, 0.35f, 0.35f, 0.02f, 12, Fade(BLACK, 0.4f));

    //         if (champModels.count(champ.id)) {
    //             int id = champ.id;
    //             Model& model = champModels[id];

    //             ModelAnimation* anims = champAnims[id];
    //             int count = champAnimCounts[id];
    //             if (anims != nullptr && count > 0 && anims[0].keyframeCount > 1) {
    //                 float endFrame = (float)anims[0].keyframeCount - 2.0f;
    //                 champAnimFrame[id] += GetFrameTime() * 30.0f * champAnimDir[id];
    //                 if (champAnimFrame[id] >= endFrame) { champAnimFrame[id] = endFrame; champAnimDir[id] = -1.0f; }
    //                 else if (champAnimFrame[id] <= 0.0f) { champAnimFrame[id] = 0.0f; champAnimDir[id] = 1.0f; }
    //                 UpdateModelAnimation(model, anims[0], (int)champAnimFrame[id]);
    //             }

    //             Vector3 drawPos = { pos.x, 0.0f, pos.z };
    //             float scale = champScales[id];
    //             DrawModelEx(model, drawPos, {0, 1, 0}, 0.0f, {scale, scale, scale}, WHITE);
    //         } else {
    //             DrawCube(pos, 0.4f, 0.4f, 0.4f, MAROON);
    //             DrawCubeWires(pos, 0.4f, 0.4f, 0.4f, WHITE);
    //         }
    //     }

    //     rlEnableBackfaceCulling();
    // }

        // {
        //     static const char* path = "assets/Set18_Ahri.glb";
        //     static Model testModel = LoadModel(path);
        //     static int animationCount = 0;
        //     static ModelAnimation* animations = LoadModelAnimations(path, &animationCount);
        //     static BoundingBox bounds = GetModelBoundingBox(testModel);
        //     static float scale = 1.2f / (bounds.max.y - bounds.min.y);
        //     static float animationFrame = 0.0f;
        //     static float animationDirection = 1.0f;

        //     if (animations != nullptr && animationCount > 0 && animations[0].keyframeCount > 1) {
        //         float endFrame = (float)animations[0].keyframeCount - 2.0f;
        //         animationFrame += GetFrameTime() * 30.0f * animationDirection;

        //         if (animationFrame >= endFrame) {
        //             animationFrame = endFrame;
        //             animationDirection = -1.0f;
        //         } else if (animationFrame <= 0.0f) {
        //             animationFrame = 0.0f;
        //             animationDirection = 1.0f;
        //         }

        //         UpdateModelAnimation(testModel, animations[0], (int)animationFrame);
        //     }

        //     Vector3 position = HexCenter(2, 3);
        //     position.y = -bounds.min.y * scale;

        //     rlEnableBackfaceCulling();
        //     rlSetCullFace(RL_CULL_FACE_FRONT);
        //     DrawModelEx(testModel, position, {0.0f, 1.0f, 0.0f}, 0.0f, {scale, scale, scale}, WHITE);
        //     rlSetCullFace(RL_CULL_FACE_BACK);
        // }
        // {
        //     static const char* path = "assets/Set18_Ahri.glb";
        //     static Model testModel = LoadModel(path);
        //     static int animationCount = 0;
        //     static ModelAnimation* animations = LoadModelAnimations(path, &animationCount);
        //     static BoundingBox bounds = GetModelBoundingBox(testModel);
        //     static float scale = 1.2f / (bounds.max.y - bounds.min.y);
        //     static float animationFrame = 0.0f;
        //     static float animationDirection = 1.0f;

        //     if (animations != nullptr && animationCount > 0 && animations[0].keyframeCount > 1) {
        //         float endFrame = (float)animations[0].keyframeCount - 2.0f;
        //         animationFrame += GetFrameTime() * 30.0f * animationDirection;

        //         if (animationFrame >= endFrame) {
        //             animationFrame = endFrame;
        //             animationDirection = -1.0f;
        //         } else if (animationFrame <= 0.0f) {
        //             animationFrame = 0.0f;
        //             animationDirection = 1.0f;
        //         }

        //         UpdateModelAnimation(testModel, animations[0], (int)animationFrame);
        //     }

        //     rlEnableBackfaceCulling();
        //     rlSetCullFace(RL_CULL_FACE_FRONT);

        //     for (int row = 0; row < 4; row++) {
        //         for (int col = 0; col < 7; col++) {
        //             Champion& champ = engine.gamestate.board[row][col];

        //             if (champ.id == Set18::Ahri.id) {
        //                 Vector3 position = HexCenter(row, col);
        //                 position.y = -bounds.min.y * scale;
        //                 DrawModelEx(testModel, position, {0.0f, 1.0f, 0.0f}, 0.0f, {scale, scale, scale}, WHITE);
        //             }
        //         }
        //     }

        //     for (int i = 0; i < 9; i++) {
        //         Champion& champ = engine.gamestate.bench[i];

        //         if (champ.id == Set18::Ahri.id) {
        //             Vector3 position = BenchCenter(i);
        //             position.y = -bounds.min.y * scale;
        //             DrawModelEx(testModel, position, {0.0f, 1.0f, 0.0f}, 0.0f, {scale, scale, scale}, WHITE);
        //         }
        //     }

        //     rlSetCullFace(RL_CULL_FACE_BACK);
        // }
        rlDisableBackfaceCulling();

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

        DrawDraggedChampionModel();

        rlEnableBackfaceCulling();

        // DRAWING HEXAGONS
        float w = sqrtf(3.0f) * r;

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
        
        // DRAWING BOARD SQUARES
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

        EndMode3D();
        // shop
        shop.DrawShop(engine, hoveredShop, hoverXp, hoverReroll, drag.GetState(), GetDraggedChampion());

        // trait bar
        // DrawRectangleLinesEx(TraitBarRect(), 2.0f, SKYBLUE);

        // Rectangle bar = TraitBarRect();
        // DrawTraitHexs();

        traits.DrawTraits(engine, drag);

        // xp and reroll mechanics
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoverXp) {
            engine.levelup();
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoverReroll) {
            engine.roll();
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoverLock) {
            engine.gamestate.shoplocked = !engine.gamestate.shoplocked;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoveredItem != -1 && hoveredItem < engine.gamestate.items.size()) {
            SlotRef source = {Zone::Inventory, hoveredItem, -1, -1};
            drag.BeginDrag(DragPayload::Item, source, {0, 0});
        }

        if (IsKeyPressed(KEY_D)) engine.roll();
        if (IsKeyPressed(KEY_F)) engine.levelup();

        if (IsKeyPressed(KEY_E)) {
            if (hoveredBench != -1 && engine.gamestate.bench[hoveredBench].id != 0) {
                engine.sellbench(hoveredBench);
            } else if (hoveredRow != -1 && hoveredCol != -1 && engine.gamestate.board[hoveredRow][hoveredCol].id != 0) {
                engine.sellboard(std::make_pair(hoveredRow, hoveredCol));
            }
        }

        if (IsKeyPressed(KEY_W) && hoveredBench != -1 && engine.gamestate.bench[hoveredBench].id != 0) {
            bool moved = false;

            for (int row = 0; row < 4 && !moved; row++) {
                for (int col = 0; col < 7; col++) {
                    if (engine.gamestate.board[3 - row][col].id == 0) {
                        engine.benchtoboard(hoveredBench, std::make_pair(3 - row, col));
                        moved = true;
                        break;
                    }
                }
            }
        }

        else if (IsKeyPressed(KEY_W) && hoveredRow != -1 && hoveredCol != -1 &&
                engine.gamestate.board[hoveredRow][hoveredCol].id != 0) {
            for (int i = 0; i < 9; i++) {
                if (engine.gamestate.bench[i].id == 0) {
                    engine.boardtobench(std::make_pair(hoveredRow, hoveredCol), i);
                    break;
                }
            }
        }

        // debugger
        GameState& gs = engine.gamestate;
        int y = 10;
        auto line = [&](const char* s) { DrawText(s, 10, y, 18, WHITE); y += 20; };
        line(TextFormat("gold:%d lvl:%d xp:%d stage:%d time:%.1f", gs.gold, gs.level, gs.xp, gs.stage, gs.time));
        line(TextFormat("boardUnits:%d locked:%d", gs.boardUnitCount, gs.shoplocked));
        line(TextFormat("hover row:%d col:%d bench:%d shop:%d", hoveredRow, hoveredCol, hoveredBench, hoveredShop));
        line(TextFormat("drag state:%d zone:%d index:%d row:%d col:%d", (int)drag.GetState().phase, (int)drag.GetState().source.zone, drag.GetState().source.index, drag.GetState().source.row, drag.GetState().source.col));

        // for (int i = 0; i < 5; i++) {
        //     Champion& c = gs.shop[i];
        //     line(c.id == 0 ? TextFormat("shop %d: -", i)
        //                 : TextFormat("shop %d: %s $%d *%d", i, c.name.c_str(), c.cost, c.starLevel));
        // }

        // for (int i = 0; i < 9; i++) {
        //     Champion& c = gs.bench[i];
        //     if (c.id != 0) line(TextFormat("bench %d: %s $%d *%d", i, c.name.c_str(), c.cost, c.starLevel));
        // }

        // for (int rw = 0; rw < 4; rw++) {
        //     for (int cl = 0; cl < 7; cl++) {
        //         Champion& c = gs.board[rw][cl];
        //         if (c.id != 0) line(TextFormat("board %d,%d: %s *%d", rw, cl, c.name.c_str(), c.starLevel));
        //     }
        // }
        EndDrawing();

    }

    return;
}

void App::shutdown() {
    for (auto& e : champModels) UnloadModel(e.second);
    for (auto& e : champAnims)  UnloadModelAnimations(e.second, champAnimCounts[e.first]);
    champModels.clear();
    champAnims.clear();

    shop.shutdown();
    traits.shutdown();

    if (background.id != 0) {
        UnloadTexture(background);
        background = Texture2D{};
    }

    if (IsWindowReady()) {
        CloseWindow();
    }

    
}
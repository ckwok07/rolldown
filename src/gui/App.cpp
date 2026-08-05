#include "App.h"
#include <iostream>
#include "raylib.h"
#include <cmath>
#include "rlgl.h"
#include "raymath.h"
#include "../SetId.h"

static Color CostTierColor(int cost) {
    switch (cost) {
        case 1: return Color{ 145, 145, 145, 255 }; // grey
        case 2: return Color{  50, 205, 100, 255 }; // green
        case 3: return Color{  65, 135, 255, 255 }; // blue
        case 4: return Color{ 175,  75, 235, 255 }; // purple
        case 5: return Color{ 245, 185,  45, 255 }; // gold
        default: return DARKGRAY;
    }
}

App::App() {}

App::~App() {
    shutdown();
}

void App::DrawShopIcon(Rectangle rect, Champion& champion, Color tierColor, int i, bool highlighted) {
    float border = 3.0f;
    float infoHeight = rect.height * 0.18f;

    Rectangle innerRect = { rect.x + border, rect.y + border, rect.width - border * 2.0f, rect.height - border * 2.0f};

    // spash art rectangle
    Rectangle artRect = {innerRect.x, innerRect.y, innerRect.width, innerRect.height - infoHeight};

    // name rectangle
    Rectangle infoRect = {innerRect.x, artRect.y + artRect.height, innerRect.width, infoHeight};

    if (champion.id != 0 && !(drag.phase == DragPhase::Dragging && drag.sourceZone == Zone::Shop && drag.sourceIndex == i)) {
        Texture2D* splash = GetChampionSplash(champion.name);

        if (splash != nullptr) {
            DrawTextureCover(*splash, artRect, WHITE);
        }

        DrawRectangleRec(infoRect, Fade(tierColor, 0.85f));

        int fontSize = (int)(infoRect.height * 0.60f);
        int textY = (int)(infoRect.y + (infoRect.height - fontSize)/ 1.5);

        // name
        DrawText(champion.name.c_str(), (int)(infoRect.x + 6.0f), textY, fontSize, WHITE );

        // cost
        const char* costText = TextFormat("%d", champion.cost);

        int costWidth =MeasureText(costText, fontSize);

        DrawText( costText,(int)(infoRect.x + infoRect.width - costWidth -6.0f), textY, fontSize, WHITE);
    }

    if (champion.id != 0 && !(drag.phase == DragPhase::Dragging && drag.sourceZone == Zone::Shop && drag.sourceIndex == i)) {
        if (highlighted) {
            DrawRectangleRec(rect, Fade(WHITE, 0.25f));
        }

        DrawRectangleLinesEx(rect, 3.0f, tierColor);
    }
}

bool App::init() {
    engine.initGameState(SetId::Set18);

    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_TOPMOST);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(1792, 1008, "Rolldown Simulator");

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

Texture2D* App::GetChampionSplash(const std::string& championName) {
    if (championName.empty()) {
        return nullptr;
    }

    auto existing = splashTextures.find(championName);

    if (existing != splashTextures.end()) {
        return existing->second.id != 0
            ? &existing->second
            : nullptr;
    }

    std::string assetName;

    for (char ch : championName) {
        if (ch != ' ' && ch != '\'') {
            assetName += ch;
        }
    }

    std::string prefix;
    std::string suffix; 

    if (engine.gamestate.activeSet == SetId::Set17) {
        prefix = "Set17";
        suffix = "TFT17_";
    } else {
        prefix = "Set18";
        suffix = "TFT18_";
    }

    std::string path = "assets/" + prefix + "/" + suffix + assetName + ".png";

    if (!FileExists(path.c_str())) {
        TraceLog(
            LOG_WARNING,
            "Missing champion splash: %s",
            path.c_str()
        );

        splashTextures[championName] = Texture2D{};
        return nullptr;
    }

    Texture2D texture = LoadTexture(path.c_str());

    SetTextureFilter(
        texture,
        TEXTURE_FILTER_BILINEAR
    );

    splashTextures[championName] = texture;

    return &splashTextures[championName];
}

void App::DrawTextureCover( Texture2D texture, Rectangle destination, Color tint) {
    float textureAspect =
        (float)texture.width / (float)texture.height;

    float destinationAspect =
        destination.width / destination.height;

    Rectangle source = {
        0.0f,
        0.0f,
        (float)texture.width,
        (float)texture.height
    };

    if (textureAspect > destinationAspect) {
        source.width =
            (float)texture.height * destinationAspect;

        source.x =
            ((float)texture.width - source.width) / 2.0f;
    } else {
        source.height =
            (float)texture.width / destinationAspect;

        source.y =
            ((float)texture.height - source.height) / 2.0f;
    }

    DrawTexturePro( texture, source, destination, { 0.0f, 0.0f }, 0.0f, tint);
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

Rectangle App::ShopBarRect() {
    float H = (float)GetScreenHeight();
    float W = (float)GetScreenWidth();

    float barH   = H * 0.1565f;
    float barW   = barH * 6.8f;
    float bottom = 0;

    return { (W - barW) / 2.0f + W * -0.042f, H - bottom - barH, barW, barH };
}

Rectangle App::ShopXpRect() {
    Rectangle bar = ShopBarRect();
    return {
        bar.x + bar.width  * 0.009f,
        bar.y + bar.height * 0.08f,
        bar.width  * 0.155f,
        bar.height * 0.39f
    };
}

Rectangle App::ShopRerollRect() {
    Rectangle bar = ShopBarRect();
    return {
        bar.x + bar.width  * 0.009f,
        bar.y + bar.height * 0.52f,
        bar.width  * 0.155f,
        bar.height * 0.39f
    };
}

Rectangle App::ShopSlotRect(int i) {
    Rectangle bar = ShopBarRect();

    float left  = bar.width * 0.18f;
    float right = bar.width * 0.99f;
    float gap   = bar.width * 0.007f;
    float cardH = bar.height * 0.86f;

    float stripW = right - left;
    float cardW  = (stripW - 4.0f * gap) / 5.0f;

    return {
        bar.x + left + i * (cardW + gap),
        bar.y + (bar.height - cardH) / 2.0f,
        cardW,
        cardH
    };
}

void App::run() {
    while (!WindowShouldClose()) {
        // get mouse position
        int hoveredRow = -1;
        int hoveredCol = -1;
        int hoveredBench = -1;
        int hoveredShop = -1;

        bool hoverXp = CheckCollisionPointRec(GetMousePosition(), ShopXpRect());
        bool hoverReroll = CheckCollisionPointRec(GetMousePosition(), ShopRerollRect());

        Ray ray = GetScreenToWorldRay(GetMousePosition(), camera);
        for (int i = 0; i < 5; i++) {
            if (CheckCollisionPointRec(GetMousePosition(), ShopSlotRect(i))) {
                hoveredShop = i;
                break;
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoveredShop != -1 && engine.gamestate.shop[hoveredShop] != nullChamp) {
            drag.phase = DragPhase::Pending;
            drag.sourceZone = Zone::Shop;
            drag.sourceIndex = hoveredShop;
            drag.pressPos = GetMousePosition();

            Rectangle card = ShopSlotRect(hoveredShop);
            drag.grabOffset = { GetMousePosition().x - card.x, GetMousePosition().y - card.y };
        }

        if (drag.phase == DragPhase::Pending && Vector2Distance(drag.pressPos, GetMousePosition()) > 5.0f) {
            drag.phase = DragPhase::Dragging;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && drag.sourceZone == Zone::Shop) {
            if (drag.phase == DragPhase::Pending) {
                engine.buy(drag.sourceIndex);
            } else if (drag.phase == DragPhase::Dragging) {
                float cardW = ShopSlotRect(drag.sourceIndex).width;
                if (Vector2Distance(drag.pressPos, GetMousePosition()) >= 0.5f * cardW) {
                    engine.buy(drag.sourceIndex);
                }
                // under half a card → cancel, do nothing
            }
            drag.phase = DragPhase::Idle;
            drag.sourceZone = Zone::None;
            drag.sourceIndex = -1;
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


        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && drag.sourceZone == Zone::Bench) {
            // sell unit
            if (CheckCollisionPointRec(GetMousePosition(), ShopBarRect())) {
                engine.sellbench(drag.sourceIndex);
            } else if (hoveredBench != -1) { // swap with bench
                engine.benchtobench(drag.sourceIndex, hoveredBench);
            } else if (hoveredRow != -1 && hoveredCol != -1) { // swap with board
                engine.benchtoboard(drag.sourceIndex, make_pair(hoveredRow, hoveredCol));
            } else { // no move

            }

            drag.phase = DragPhase::Idle;
            drag.sourceZone = Zone::None;
            drag.sourceIndex = -1;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && drag.sourceZone == Zone::Board) {
            // sell unit
            if (CheckCollisionPointRec(GetMousePosition(), ShopBarRect())) {
                engine.sellboard(drag.sourceBoard);
            } else if (hoveredBench != -1) { // swap with bench
                engine.boardtobench(drag.sourceBoard, hoveredBench);
            } else if (hoveredRow != -1 && hoveredCol != -1) { // swap with board
                engine.boardtoboard(drag.sourceBoard, make_pair(hoveredRow, hoveredCol));
            } else { // no move
                
            }

            drag.phase = DragPhase::Idle;
            drag.sourceZone = Zone::None;
            drag.sourceIndex = -1;
            drag.sourceBoard = make_pair(-1,-1);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoveredBench != -1) {
            drag.phase = DragPhase::Pending;
            drag.sourceZone = Zone::Bench;
            drag.sourceIndex = hoveredBench;
            drag.pressPos = GetMousePosition();
        }  

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoveredRow != -1 && hoveredCol != -1) {
            drag.phase = DragPhase::Pending;
            drag.sourceZone = Zone::Board;
            drag.sourceBoard = make_pair(hoveredRow, hoveredCol);
            drag.pressPos = GetMousePosition();
        }

        if (drag.phase != DragPhase::Idle && !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            drag.phase = DragPhase::Idle;
            drag.sourceZone = Zone::None;
            drag.sourceIndex = -1;
        }

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
                        Vector3 pos = HexCenter(row, col);
                        pos.y = yOff;
                        DrawModelEx(model, pos, {0,1,0}, 0.0f, {scale,scale,scale}, WHITE);
                    }
                }
            }

            for (int i = 0; i < 9; i++) {
                if (engine.gamestate.bench[i].id == id) {
                    Vector3 pos = BenchCenter(i);
                    pos.y = yOff;
                    DrawModelEx(model, pos, {0,1,0}, 0.0f, {scale,scale,scale}, WHITE);
                }
            }
        }

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
        DrawRectangleLinesEx(ShopBarRect(), 3.0f, SKYBLUE);
        DrawRectangleLinesEx(ShopXpRect(), 3.0f, hoverXp ? YELLOW : SKYBLUE);
        DrawRectangleLinesEx(ShopRerollRect(), 3.0f, hoverReroll ? YELLOW : SKYBLUE);

        for (int i = 0; i < 5; i++) {
            Rectangle rect = ShopSlotRect(i);
            Champion& champion = engine.gamestate.shop[i];
            Color tierColor = CostTierColor(champion.cost);
            DrawShopIcon(rect, champion, tierColor,i, i == hoveredShop);
        }

        if (drag.phase == DragPhase::Dragging && drag.sourceZone == Zone::Shop) {
            Rectangle card = ShopSlotRect(drag.sourceIndex);
            Vector2 m = GetMousePosition();
            Rectangle ghost = { m.x - drag.grabOffset.x, m.y - drag.grabOffset.y, card.width, card.height};
            Champion& champion = engine.gamestate.shop[drag.sourceIndex];
            Color tierColor = CostTierColor(champion.cost);

            DrawShopIcon(ghost, champion, tierColor, -1, true);
        }

        // xp and reroll mechanics
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoverXp) {
            engine.levelup();
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoverReroll) {
            engine.roll();
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
        line(TextFormat("drag state:%d src:%d", (int)drag.phase, drag.sourceIndex));

        for (int i = 0; i < 5; i++) {
            Champion& c = gs.shop[i];
            line(c.id == 0 ? TextFormat("shop %d: -", i)
                        : TextFormat("shop %d: %s $%d *%d", i, c.name.c_str(), c.cost, c.starLevel));
        }

        for (int i = 0; i < 9; i++) {
            Champion& c = gs.bench[i];
            if (c.id != 0) line(TextFormat("bench %d: %s $%d *%d", i, c.name.c_str(), c.cost, c.starLevel));
        }

        for (int rw = 0; rw < 4; rw++) {
            for (int cl = 0; cl < 7; cl++) {
                Champion& c = gs.board[rw][cl];
                if (c.id != 0) line(TextFormat("board %d,%d: %s *%d", rw, cl, c.name.c_str(), c.starLevel));
            }
        }
        EndDrawing();

    }

    return;
}

void App::shutdown() {
    for (auto& entry : splashTextures) {
        Texture2D& texture = entry.second;

        if (texture.id != 0) {
            UnloadTexture(texture);
        }
    }

    for (auto& e : champModels) UnloadModel(e.second);
    for (auto& e : champAnims)  UnloadModelAnimations(e.second, champAnimCounts[e.first]);
    champModels.clear();
    champAnims.clear();

    splashTextures.clear();

    if (background.id != 0) {
        UnloadTexture(background);
        background = Texture2D{};
    }

    if (IsWindowReady()) {
        CloseWindow();
    }

    
}
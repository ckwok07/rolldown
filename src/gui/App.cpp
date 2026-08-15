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
    board.init(SetId::Set18);

    background = LoadTexture("assets/image2.png");
    camera.position = { 0.2f, 12.0f, 9.0f };
    camera.target   = { 0.2f, 0.0f, 0.0f };
    camera.up       = { 0.0f, 1.0f, 0.0f };
    camera.fovy     = 35.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    return true;
}


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
            if (drag.GetState().holdReady) {
                if (!engine.combine(source.index, hoveredItem)) {
                    std::swap(engine.gamestate.items[source.index], engine.gamestate.items[hoveredItem]);
                }
            } else {
                std::swap(engine.gamestate.items[source.index], engine.gamestate.items[hoveredItem]);
            }
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
                    Vector3 c = board.HexCenter(row, col);
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
                Vector3 c = board.BenchCenter(i);
                if (fabsf(hx - c.x) <= halfSq && fabsf(hz - c.z) <= halfSq) {
                    hoveredBench = i;
                    break;
                }
            }
        }
        SlotRef hoveredChampion = board.getHoveredChampion(ray, engine);

        if (hoveredChampion.zone == Zone::Board) {
            hoveredRow = hoveredChampion.row;
            hoveredCol = hoveredChampion.col;
        }

        if (hoveredChampion.zone == Zone::Bench) {
            hoveredBench = hoveredChampion.index;
        }
        
        HandleDragPress(hoveredShop, hoveredBench, hoveredRow, hoveredCol);
        if (drag.GetState().phase == DragPhase::Dragging && drag.GetState().payload == DragPayload::Item) {
            SlotRef holdTarget = {};

            if (hoveredItem != -1 && hoveredItem < engine.gamestate.items.size() && hoveredItem != drag.GetState().source.index) {
                holdTarget = {Zone::Inventory, hoveredItem, -1, -1};
            }

            drag.UpdateHold(holdTarget, 1.0f);
        }
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

        Vector3 mousePosition = {0.0f, 0.0f, 0.0f};
        GetMouseGroundPosition(mousePosition);

        const Champion* draggedChampion = GetDraggedChampion();
        BeginMode3D(camera);

        rlDisableBackfaceCulling();

        board.drawVisuals(engine, drag, camera, mousePosition, hoveredRow, hoveredCol, hoveredBench, draggedChampion);
        rlEnableBackfaceCulling();

        EndMode3D();

        // shop
        shop.DrawShop(engine, hoveredShop, hoverXp, hoverReroll, drag.GetState(), GetDraggedChampion());
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
        line(TextFormat("hold time: %.2f ready: %d target: %d", drag.GetState().holdTime, drag.GetState().holdReady, drag.GetState().holdTarget.index));
        EndDrawing();

    }

    return;
}

void App::shutdown() {
    shop.shutdown();
    traits.shutdown();
    board.shutdown();

    if (background.id != 0) {
        UnloadTexture(background);
        background = Texture2D{};
    }

    if (IsWindowReady()) {
        CloseWindow();
    }

    
}
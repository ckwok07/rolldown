#include "App.h"
#include <iostream>
#include "raylib.h"
#include <cmath>
#include "rlgl.h"
#include "raymath.h"

App::App() {}

App::~App() {
    shutdown();
}

bool App::init() {
    engine.initGameState();

    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_TOPMOST);
    InitWindow(1800, 1012, "Rolldown Simulator");
    background = LoadTexture("assets/image2.png");
    camera.position = { 0.2f, 12.0f, 9.0f };
    camera.target   = { 0.2f, 0.0f, 0.0f };
    camera.up       = { 0.0f, 1.0f, 0.0f };
    camera.fovy     = 35.0f;
    camera.projection = CAMERA_PERSPECTIVE;
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

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoveredShop != -1) {
            drag.phase = DragPhase::Pending;
            drag.sourceZone = Zone::Shop;
            drag.sourceIndex = hoveredShop;
            drag.pressPos = GetMousePosition();

            Rectangle card = ShopSlotRect(hoveredShop);
            drag.grabOffset = { GetMousePosition().x - card.x, GetMousePosition().y - card.y };
        }

        if (drag.phase == DragPhase::Pending &&
            Vector2Distance(drag.pressPos, GetMousePosition()) > 5.0f) {
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

            float halfSq = squareSide / 2.0f;

            for (int i = 0; i < 9; i++) {
                Vector3 c = BenchCenter(i);
                if (fabsf(hx - c.x) <= halfSq && fabsf(hz - c.z) <= halfSq) {
                    hoveredBench = i;
                    break;
                }
            }
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
            Color c = (i == hoveredShop) ? YELLOW : SKYBLUE;
            DrawRectangleLinesEx(rect, 3.0f, c);
        }

        if (drag.phase == DragPhase::Dragging && drag.sourceZone == Zone::Shop) {
            Rectangle card = ShopSlotRect(drag.sourceIndex);
            Vector2 m = GetMousePosition();
            Rectangle ghost = { m.x - drag.grabOffset.x, m.y - drag.grabOffset.y, card.width, card.height };
            DrawRectangleRec(ghost, Fade(SKYBLUE, 0.5f));
            DrawRectangleLinesEx(ghost, 3.0f, YELLOW);
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
    CloseWindow();
    return;
}
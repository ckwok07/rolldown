#include "App.h"
#include <iostream>
#include "raylib.h"
#include <cmath>
#include "rlgl.h"

App::App() {}

App::~App() {
    shutdown();
}

bool App::init() {
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_TOPMOST);
    InitWindow(1600, 900, "Rolldown Simulator");
    background = LoadTexture("assets/image.png");
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

void App::run() {
    while (!WindowShouldClose()) {
        // get mouse position
        int hoveredRow = -1;
        int hoveredCol = -1;
        int hoveredBench = -1;

        Ray ray = GetScreenToWorldRay(GetMousePosition(), camera);

        if (ray.direction.y < 0.0f) {
            
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
        float tilt = -1.0f * DEG2RAD;

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
        EndDrawing();

    }

    return;
}

void App::shutdown() {
    CloseWindow();
    return;
}
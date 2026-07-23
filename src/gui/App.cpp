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
    InitWindow(1280, 720, "Rolldown Simulator");
    background = LoadTexture("assets/image.png");
    camera.position = { 0.2f, 10.0f, 9.0f };
    camera.target   = { 0.2f, 0.0f, 0.0f };
    camera.up       = { 0.0f, 1.0f, 0.0f };
    camera.fovy     = 43.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    return true;
}

void App::run() {
    while (!WindowShouldClose()) {
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

        float r = 0.65f;              // spacing radius
        float drawR = r * 0.90f;      // actual drawn radius; smaller = larger gaps
        float w = sqrtf(3.0f) * r;

        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 7; col++) {
                float cx = (col - 3) * w
                        + (row % 2 == 0 ? -w / 4.0f : w / 4.0f);

                float cz = (row - 1.5f) * 1.5f * r;

                Vector3 pts[6];

                for (int i = 0; i < 6; i++) {
                    float a = (60.0f * i - 90.0f) * DEG2RAD;

                    pts[i] = {
                        cx + drawR * cosf(a),
                        0.0f,
                        cz + drawR * sinf(a)
                    };
                }

                for (int i = 0; i < 6; i++) {
                    DrawCylinderEx(
                        pts[i],
                        pts[(i + 1) % 6],
                        0.018f,
                        0.018f,
                        6,
                        SKYBLUE
                    );
                }
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
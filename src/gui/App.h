#pragma once

#include "raylib.h"

class App
{
private:
    Texture2D background;

    Camera3D camera = { 0 };
public:
    App(/* args */);
    ~App();

    bool init();
    void run();
    void shutdown();
};
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

    float r = 0.55f;
    float drawR = r * 0.90f; 
    float squareSide = r * 1.2f;


    Vector3 HexCenter(int row, int col);
    Vector3 BenchCenter(int i);
    
    bool init();
    void run();
    void shutdown();
};
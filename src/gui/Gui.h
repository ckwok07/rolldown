#pragma once

#include <GLFW/glfw3.h>
#include "../Engine.h"

class GUI {
public:
    GUI();
    ~GUI();

    bool init();
    void run();
    void shutdown();

private:
    GLFWwindow* window = nullptr;
    Engine engine;

    int width = 1920;
    int height = 1040;

    bool draggingShop = false;
    int draggedShopIndex = -1;
    Champion draggedShopChamp = nullChamp;
    float dragOffsetX = 0.0f;
    float dragOffsetY = 0.0f;

    bool draggingBench = false;
    int draggedBenchIndex = -1;
    Champion draggedBenchChamp = nullChamp;
    float benchDragOffsetX = 0.0f;
    float benchDragOffsetY = 0.0f;
};
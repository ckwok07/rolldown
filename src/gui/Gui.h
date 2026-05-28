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
};
#include "Engine.h"
#include <iostream>

int main() {
    Engine engine;
    engine.init();

    // print shop
    for (int i = 0; i < 5; i++) {
        std::cout << engine.gamestate.shop[i].name << std::endl;
    }

    return 0;
}
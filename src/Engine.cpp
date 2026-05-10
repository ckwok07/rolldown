#include "Engine.h"
#include "Globals.h"
#include <random>
#include "Champion.h"

Engine::Engine() {
}

Engine::~Engine() {
    shutdown();
}

bool Engine::init() {
    initGameState();
    initChampPool();
    initShop();

    return true;
}

void Engine::shutdown() {
}

// setup
// init game state
void Engine::initGameState() {

}

// init pool
void Engine::initChampPool() {
    for (const Champion champ : ALL_CHAMPIONS) {
        if (champ.cost == 1) gamestate.pool[champ.id] = 30;
        else if (champ.cost == 2) gamestate.pool[champ.id] = 25;
        else if (champ.cost == 3) gamestate.pool[champ.id] = 18;
        else if (champ.cost == 4) gamestate.pool[champ.id] = 10;
        else if (champ.cost == 5) {
            if (champ.name == "Zed" && !gamestate.zed) {
                gamestate.pool[champ.id] = 0;
            } else {
                gamestate.pool[champ.id] = 9;
            }
        }
    }
}
// init shop
void Engine::initShop() {
    mt19937 rng(1);
    vector<int> distribution = shopodds[gamestate.level - 2];

    for (int i = 0; i < 5; i++) {
        uniform_int_distribution<int> dist(0, 99);
        int roll = dist(rng);
        
        if (roll < distribution[0]) {
            // add one cost
        } else if (roll < distribution[0] + distribution [1]) {
            // add two cost
        } else if (roll < distribution[0] + distribution [1] + distribution [2]) {
            // add three cost
        } else if (roll < distribution[0] + distribution [1] + distribution [2]+ distribution[3]) {
            // add four cost
        } else {
            // add five cost
        }
    }
}

Champion Engine::getChamp(int cost) {
    for (auto champ : gamestate.pool) {
        if (cost == 1) {
            // 1 cost
        } else if (cost == 2) {

        } else if (cost == 3) {

        } else if (cost == 4) {

        } else {
            
        }
    }
    return {41, "Aurelion Sol", 4, 1, {11, 25}, {}};
}
// reset
void Engine::reset() {

}

// economy
// level up
void Engine::levelup() {

}
// roll shop
void Engine::roll() {

}
// buy unit
void Engine::buy(int shopindex) {

}
// sell unit
void Engine::sellboard(pair<int, int> index) {

}
void Engine::sellbench(int index) {

}
// lock shop
void Engine::lockshop() {

}

// movements
// bench to bench
void Engine::benchtobench(int from, int to) {

}
// bench to board
void Engine::benchtoboard(int from, pair<int,int> to) {

}
// board to board
void Engine::boardtoboard(pair<int,int> from, pair<int,int> to) {

}
// board to bench
void Engine::boardtobench(pair<int,int> from, int to) {

}
// swap units


// items
// slam item
void Engine::slamBoard(int index, pair<int,int> position) {

}
void Engine::slamBench(int index, int position) {

}
// combine items
void Engine::combine(int index1, int index2) {

}
// use reforger
void Engine::reforgerBench(int reforger, int index) {

}
void Engine::reforgerBoard(int reforger, pair<int,int> index) {

}
// use remover
void Engine::removerBench(int remover, int index) {

}
void Engine::removerBoard(int remover, pair<int,int> index) {

}
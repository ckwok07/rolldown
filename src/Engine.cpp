#include "Engine.h"
#include "Globals.h"
#include <random>
#include "Champion.h"

Engine::Engine() {
    rng = mt19937(random_device{}());
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
    vector<int> distribution = shopodds[gamestate.level - 2];

    for (int i = 0; i < 5; i++) {
        uniform_int_distribution<int> dist(0, 99);
        int roll = dist(rng);
        
        if (roll < distribution[0]) {
            gamestate.shop[i] = getChamp(1);
        } else if (roll < distribution[0] + distribution [1]) {
            gamestate.shop[i] = getChamp(2);
        } else if (roll < distribution[0] + distribution [1] + distribution [2]) {
            gamestate.shop[i] = getChamp(3);
        } else if (roll < distribution[0] + distribution [1] + distribution [2]+ distribution[3]) {
           gamestate.shop[i] = getChamp(4);
        } else {
            gamestate.shop[i] = getChamp(5);
        }
    }
}

Champion Engine::getChamp(int cost) {
    int total = 0;
    for (const Champion& champ : ALL_CHAMPIONS) {
        if (champ.cost == cost) {
            total += gamestate.pool[champ.id];
        }
    }

    uniform_int_distribution<int> dist(0, total - 1);
    int roll = dist(rng);

    int cumulative = 0;
    for (const Champion& champ : ALL_CHAMPIONS) {
        if (champ.cost == cost) {
            cumulative += gamestate.pool[champ.id];
            if (roll < cumulative) {
                gamestate.pool[champ.id]--;
                return champ;
            }
        }
    }
    return ALL_CHAMPIONS[0];
}
// reset
void Engine::reset() {

}

// economy
// level up
void Engine::levelup() {
    if (gamestate.gold >= 4) {
        gamestate.gold -= 4;
        gamestate.xp += 4;
        if (gamestate.xp >= levelthresholds[gamestate.level]) {
            gamestate.xp -= levelthresholds[gamestate.level];
            gamestate.level++;
        }
    }
}
// roll shop
void Engine::roll() {
    if (gamestate.shoplocked) return;
    if (gamestate.gold < 2) return;
    vector<int> distribution = shopodds[gamestate.level - 2];
    gamestate.gold -= 2;

    for (int i = 0; i < 5; i++) {
        Champion temp = gamestate.shop[i];
        uniform_int_distribution<int> dist(0, 99);
        int roll = dist(rng);
        
        if (roll < distribution[0]) {
            gamestate.shop[i] = getChamp(1);
        } else if (roll < distribution[0] + distribution [1]) {
            gamestate.shop[i] = getChamp(2);
        } else if (roll < distribution[0] + distribution [1] + distribution [2]) {
            gamestate.shop[i] = getChamp(3);
        } else if (roll < distribution[0] + distribution [1] + distribution [2]+ distribution[3]) {
           gamestate.shop[i] = getChamp(4);
        } else {
            gamestate.shop[i] = getChamp(5);
        }
        
        if (temp.id == 0) {
            continue;
        } else {
            gamestate.pool[temp.id]++;
        }
    }
}
// buy unit
void Engine::buy(int shopindex) {

    if (gamestate.gold < gamestate.shop[shopindex].cost) return;

    int emptySlot = -1;
    for (int i = 0; i < 9; i++) {
        if (gamestate.bench[i] == nullChamp) {
            emptySlot = i;
            break;
        }
    }
    if (emptySlot == -1) return;

    Champion bought = gamestate.shop[shopindex];
    gamestate.gold -= bought.cost;
    gamestate.shop[shopindex] = nullChamp;
    gamestate.bench[emptySlot] = bought;
}

// sell unit
void Engine::sellboard(pair<int, int> index) {
    if (gamestate.board[index.first][index.second] == nullChamp) return;

    Champion sold = gamestate.board[index.first][index.second];

    // gold
    int gold = 0;
    if (sold.starLevel == 1) gold = sold.cost;
    else if (sold.starLevel == 2) gold = sold.cost == 1 ? 3 : (sold.cost * 3) - 1;
    else if (sold.starLevel == 3) gold = sold.cost == 1 ? 9 : (sold.cost * 9) - 1;
    gamestate.gold += gold;

    // return to pool
    int maxCopies = 0;
    if (sold.cost == 1) maxCopies = 30;
    else if (sold.cost == 2) maxCopies = 25;
    else if (sold.cost == 3) maxCopies = 18;
    else if (sold.cost == 4) maxCopies = 10;
    else if (sold.cost == 5) maxCopies = 9;

    int copies = 1;
    if (sold.starLevel == 2) copies = 3;
    else if (sold.starLevel == 3) copies = 9;

    gamestate.pool[sold.id] = min(gamestate.pool[sold.id] + copies, maxCopies);

    gamestate.board[index.first][index.second] = nullChamp;
    gamestate.boardUnitCount--;
}

void Engine::sellbench(int index) {
    if (gamestate.bench[index] == nullChamp) return;

    Champion sold = gamestate.bench[index];

    // gold
    int gold = 0;
    if (sold.starLevel == 1) gold = sold.cost;
    else if (sold.starLevel == 2) gold = sold.cost == 1 ? 3 : (sold.cost * 3) - 1;
    else if (sold.starLevel == 3) gold = sold.cost == 1 ? 9 : (sold.cost * 9) - 1;
    gamestate.gold += gold;

    // return to pool
    int maxCopies = 0;
    if (sold.cost == 1) maxCopies = 30;
    else if (sold.cost == 2) maxCopies = 25;
    else if (sold.cost == 3) maxCopies = 18;
    else if (sold.cost == 4) maxCopies = 10;
    else if (sold.cost == 5) maxCopies = 9;

    int copies = 1;
    if (sold.starLevel == 2) copies = 3;
    else if (sold.starLevel == 3) copies = 9;

    gamestate.pool[sold.id] = min(gamestate.pool[sold.id] + copies, maxCopies);

    gamestate.bench[index] = nullChamp;
}

// lock shop
void Engine::lockshop() {
    gamestate.shoplocked = !gamestate.shoplocked;
}

// movements
// bench to bench
void Engine::benchtobench(int from, int to) {
    if (gamestate.bench[from] == nullChamp) return;

    Champion temp = gamestate.bench[to];
    gamestate.bench[to] = gamestate.bench[from];
    gamestate.bench[from] = temp;
}
// bench to board
void Engine::benchtoboard(int from, pair<int,int> to) {
    if (gamestate.boardUnitCount >= gamestate.level) return;
    if (gamestate.bench[from] == nullChamp) return;
    if (gamestate.board[to.first][to.second] == nullChamp) gamestate.boardUnitCount++;

    Champion temp = gamestate.board[to.first][to.second];
    gamestate.board[to.first][to.second] = gamestate.bench[from];
    gamestate.bench[from] = temp;
}
// board to board
void Engine::boardtoboard(pair<int,int> from, pair<int,int> to) {
    if (gamestate.board[from.first][from.second] == nullChamp) return;

    Champion temp = gamestate.board[to.first][to.second];
    gamestate.board[to.first][to.second] = gamestate.board[from.first][from.second];
    gamestate.board[from.first][from.second] = temp;
}

void Engine::boardtobench(pair<int,int> from, int to) {
    if (gamestate.board[from.first][from.second] == nullChamp) return;
    if (gamestate.bench[to] == nullChamp) gamestate.boardUnitCount--;

    Champion temp = gamestate.bench[to];
    gamestate.bench[to] = gamestate.board[from.first][from.second];
    gamestate.board[from.first][from.second] = temp;
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
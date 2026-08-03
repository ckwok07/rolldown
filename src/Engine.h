#pragma once

#include "Gamestate.h"
#include "Champion.h"
#include <random>
#include "SetId.h"

class Engine {
public:
    Engine();
    ~Engine();

    bool init();
    void shutdown();

    // setup
    // init game state
    void initGameState(SetId set);
    // init pool
    void initChampPool();
    // init shop
    void initShop();
    Champion getChamp(int cost);
    // reset
    void reset();

    // economy
    // level up
    void levelup();
    // roll shop
    void roll();
    // buy unit
    void buy(int shopindex);
    // sell unit
    void sellboard(pair<int, int> index);
    void sellbench(int index);
    // lock shop
    void lockshop();

    // movements
    // bench to bench
    void benchtobench(int from, int to);
    // bench to board
    void benchtoboard(int from, pair<int,int> to);
    // board to board
    void boardtoboard(pair<int,int> from, pair<int,int> to);
    // board to bench
    void boardtobench(pair<int,int> from, int to);
    // swap units
    void updateShopHighlights();

    // items
    // slam item
    void slamBoard(int index, pair<int,int> position);
    void slamBench(int index, int position);
    // combine items
    void combine(int index1, int index2);
    // use reforger
    void reforgerBench(int reforger, int index);
    void reforgerBoard(int reforger, pair<int,int> index);
    // use remover
    void removerBench(int remover, int index);
    void removerBoard(int remover, pair<int,int> index);

    void updateGamestate();

    // can equip items
    bool canEquipItems(Champion& champ, Item item);

    // automatic under update game state or some thing
    // check traits
    void checkTraits();
    // combine items
    void combineItems(Champion& champ, Item item);
    GameState gamestate;


private:
    mt19937 rng;

    bool isEmblem(pair<int,int> item);
    bool isTacTrio(pair<int,int> item);
    pair<int,int> reforgeItem(pair<int,int> old, mt19937& rng);
    int reforgeComponent(int old, mt19937& rng);
    void checkStarUp();

    bool wouldStarUp(Champion champ);
    bool highlight(Champion champ);
};

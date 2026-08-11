#include "Scripts.h"
#include "Engine.h"
#include "Globals.h"
#include <iostream>
#include <fstream>
#include <random>
#include <vector>

using namespace std;

struct MonteCarloTest {
    const Champion* target;
    vector<int> levels;
};

static void ReturnShopToPool(Engine& engine) {
    for (Champion& champ : engine.gamestate.shop) {
        if (champ != nullChamp) {
            engine.gamestate.pool[champ.id]++;
            champ = nullChamp;
        }
    }
}

static int GetCostPoolSize(Engine& engine, int cost) {
    int total = 0;

    for (const Champion& champ : *engine.gamestate.activeChampions) {
        if (champ.cost == cost) {
            total += engine.gamestate.pool[champ.id];
        }
    }

    return total;
}

static void SetCostPoolSize(Engine& engine, const Champion& target, int totalPool) {
    int currentTotal = GetCostPoolSize(engine, target.cost);
    int targetCopies = engine.gamestate.pool[target.id];

    if (totalPool < targetCopies || totalPool > currentTotal) return;

    int toRemove = currentTotal - totalPool;

    for (const Champion& champ : *engine.gamestate.activeChampions) {
        if (champ.cost != target.cost || champ.id == target.id) continue;

        int& copies = engine.gamestate.pool[champ.id];
        int remove = copies < toRemove ? copies : toRemove;

        copies -= remove;
        toRemove -= remove;

        if (toRemove == 0) break;
    }
}

static Champion DrawChampion(Engine& engine, int cost, mt19937& rng) {
    int total = GetCostPoolSize(engine, cost);

    if (total == 0) {
        return nullChamp;
    }

    uniform_int_distribution<int> dist(0, total - 1);
    int roll = dist(rng);

    int cumulative = 0;

    for (const Champion& champ : *engine.gamestate.activeChampions) {
        if (champ.cost != cost) continue;

        cumulative += engine.gamestate.pool[champ.id];

        if (roll < cumulative) {
            engine.gamestate.pool[champ.id]--;
            return champ;
        }
    }

    return nullChamp;
}

static int RunThreeStarTrial(const Champion& target, int level, int startingGold, int trial, int totalPool) {
    unsigned int seed = 123 + trial;

    Engine engine(seed);
    engine.initGameState(SetId::Set18, startingGold, level);

    ReturnShopToPool(engine);
    SetCostPoolSize(engine, target, totalPool);

    mt19937 rng(seed);

    int copies = 0;
    bool firstShop = true;

    while (copies < 9) {
        if (!firstShop) {
            engine.gamestate.gold -= 2;
        }

        firstShop = false;

        vector<Champion> shop;

        for (int i = 0; i < 5; i++) {
            uniform_int_distribution<int> dist(0, 99);
            int roll = dist(rng);

            const vector<int>& odds = shopodds[level - 2];

            int cost;

            if (roll < odds[0]) cost = 1;
            else if (roll < odds[0] + odds[1]) cost = 2;
            else if (roll < odds[0] + odds[1] + odds[2]) cost = 3;
            else if (roll < odds[0] + odds[1] + odds[2] + odds[3]) cost = 4;
            else cost = 5;

            shop.push_back(DrawChampion(engine, cost, rng));
        }

        for (const Champion& champ : shop) {
            if (champ == nullChamp) continue;

            if (champ.id == target.id && copies < 9) {
                copies++;
                engine.gamestate.gold -= target.cost;
            } else {
                engine.gamestate.pool[champ.id]++;
            }
        }
    }

    return startingGold - engine.gamestate.gold;
}

void RunMonteCarloTests() {
    const int trials = 1000;
    const int startingGold = 1000000;

    const vector<MonteCarloTest> tests = {
        {&Set18::Akali,   {4, 5, 6}},
        {&Set18::Alistar, {5, 6, 7}},
        {&Set18::Azir,    {6, 7, 8}},
        {&Set18::Ahri,    {7, 8, 9}},
        {&Set18::Alune,   {9, 10}}
    };

    ofstream csv("monte_carlo2.csv");
    csv << "cost,level,total_x_cost_in_pool,gold,trial\n";

    for (const MonteCarloTest& test : tests) {
        for (int level : test.levels) {
            Engine probe(123);
            probe.initGameState(SetId::Set18, startingGold, level);

            ReturnShopToPool(probe);

            int maxPool = GetCostPoolSize(probe, test.target->cost);
            int minPool;

            if (test.target->cost <= 3) {
                minPool = maxPool - 27;
            } else {
                minPool = probe.gamestate.pool[test.target->id];
            }

            cout << "\n" << test.target->name
                 << " | " << test.target->cost << "-cost"
                 << " | Level " << level
                 << " | Pool range " << maxPool << " -> " << minPool
                 << "\n\n";

            for (int totalPool = maxPool; totalPool >= minPool; totalPool--) {
                for (int trial = 1; trial <= trials; trial++) {
                    int goldSpent = RunThreeStarTrial(*test.target, level, startingGold, trial, totalPool);

                    cout << test.target->name
                         << " | Level " << level
                         << " | Total " << test.target->cost << "-cost pool: " << totalPool
                         << " | Gold: " << goldSpent
                         << " | Trial: " << trial
                         << '\n';

                    csv << test.target->cost << ","
                        << level << ","
                        << totalPool << ","
                        << goldSpent << ","
                        << trial << "\n";
                }
            }
        }
    }

    csv.close();

    cout << "\nSimulation complete.\n";
}
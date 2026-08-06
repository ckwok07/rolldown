#include "Engine.h"
#include "Globals.h"
#include <random>
#include <map>
#include "Champion.h"
#include "SetId.h"
#include <unordered_set>

Engine::Engine() {
    rng = mt19937(random_device{}());
}

Engine::~Engine() {
    shutdown();
}

bool Engine::init() {
    initGameState(SetId::Set18);
    return true;
}

void Engine::shutdown() {
}

// setup
// init game state
void Engine::initGameState(SetId set) {
    gamestate = GameState{};
    
    gamestate.activeSet = set;
    if (set == SetId::Set17) {
        gamestate.activeChampions = &Set17::ALL_CHAMPIONS;
        gamestate.setState = Set17State{};
    } else {
        gamestate.activeChampions = &Set18::ALL_CHAMPIONS;
        gamestate.setState = Set18State{};
    }

    gamestate.gold = 500;
    gamestate.level = 8;
    gamestate.time = 30.0;
    gamestate.stage = 2;
    initChampPool();
    initShop();
}

// init pool
void Engine::initChampPool() {
    for (const Champion champ : *gamestate.activeChampions) {
        if (champ.cost == 1) gamestate.pool[champ.id] = 30;
        else if (champ.cost == 2) gamestate.pool[champ.id] = 25;
        else if (champ.cost == 3) gamestate.pool[champ.id] = 18;
        else if (champ.cost == 4) gamestate.pool[champ.id] = 10;
        else if (champ.cost == 5) gamestate.pool[champ.id] = 9;
    }
}

// init shop
void Engine::initShop() {
    vector<int> distribution = shopodds[gamestate.level - 2];

    auto hasChamps = [&](int c) {
        int total = 0;
        for (const Champion& champ : *gamestate.activeChampions) {
            if (champ.cost == c) total += gamestate.pool[champ.id];
        }
        return total > 0;
    };

    auto getCost = [&](int cost) -> int {
        if (hasChamps(cost)) return cost;
        if (cost == 1) {
            for (int c = 2; c <= 5; c++) if (hasChamps(c)) return c;
            return 0;
        }
        if (cost == 5) {
            for (int c = 4; c >= 1; c--) if (hasChamps(c)) return c;
            return 0;
        }
        uniform_int_distribution<int> coin(0, 1);
        int dir = coin(rng) == 0 ? -1 : 1;
        for (int c = cost + dir; c >= 1 && c <= 5; c += dir) {
            if (hasChamps(c)) return c;
        }
        dir = -dir;
        for (int c = cost + dir; c >= 1 && c <= 5; c += dir) {
            if (hasChamps(c)) return c;
        }
        return 0;
    };

    for (int i = 0; i < 5; i++) {
        uniform_int_distribution<int> dist(0, 99);
        int roll = dist(rng);

        int rolledCost = 0;
        if (roll < distribution[0]) rolledCost = 1;
        else if (roll < distribution[0] + distribution[1]) rolledCost = 2;
        else if (roll < distribution[0] + distribution[1] + distribution[2]) rolledCost = 3;
        else if (roll < distribution[0] + distribution[1] + distribution[2] + distribution[3]) rolledCost = 4;
        else rolledCost = 5;

        int cost = getCost(rolledCost);
        gamestate.shop[i] = cost == 0 ? nullChamp : getChamp(cost);
    }

    for (int i = 0; i < 5; i++) {
        gamestate.shopSameChampion[i] = highlight(gamestate.shop[i]);
        gamestate.shopStarUppable[i] = wouldStarUp(gamestate.shop[i]);
    }
}

Champion Engine::getChamp(int cost) {
    int total = 0;
    for (const Champion& champ : *gamestate.activeChampions) {
        if (champ.cost == cost) {
            total += gamestate.pool[champ.id];
        }
    }

    if (total == 0) return nullChamp;

    uniform_int_distribution<int> dist(0, total - 1);
    int roll = dist(rng);

    int cumulative = 0;
    for (const Champion& champ : *gamestate.activeChampions) {
        if (champ.cost == cost) {
            cumulative += gamestate.pool[champ.id];
            if (roll < cumulative) {
                gamestate.pool[champ.id]--;
                return champ;
            }
        }
    }
    return nullChamp;
}
// reset
void Engine::reset() {
    initGameState(gamestate.activeSet);
}

// economy
// level up
void Engine::levelup() {
    if (gamestate.level >= 10) return;
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

    auto hasChamps = [&](int c) {
        int total = 0;
        for (const Champion& champ : *gamestate.activeChampions) {
            if (champ.cost == c) total += gamestate.pool[champ.id];
        }
        return total > 0;
    };

    auto getCost = [&](int cost) -> int {
        if (hasChamps(cost)) return cost;
        if (cost == 1) {
            for (int c = 2; c <= 5; c++) if (hasChamps(c)) return c;
            return 0;
        }
        if (cost == 5) {
            for (int c = 4; c >= 1; c--) if (hasChamps(c)) return c;
            return 0;
        }
        uniform_int_distribution<int> coin(0, 1);
        int dir = coin(rng) == 0 ? -1 : 1;
        for (int c = cost + dir; c >= 1 && c <= 5; c += dir) {
            if (hasChamps(c)) return c;
        }
        dir = -dir;
        for (int c = cost + dir; c >= 1 && c <= 5; c += dir) {
            if (hasChamps(c)) return c;
        }
        return 0;
    };

    for (int i = 0; i < 5; i++) {
        Champion temp = gamestate.shop[i];

        if (!(temp == nullChamp)) {
            gamestate.pool[temp.id]++;
        }

        uniform_int_distribution<int> dist(0, 99);
        int roll = dist(rng);

        int rolledCost = 0;
        if (roll < distribution[0]) rolledCost = 1;
        else if (roll < distribution[0] + distribution[1]) rolledCost = 2;
        else if (roll < distribution[0] + distribution[1] + distribution[2]) rolledCost = 3;
        else if (roll < distribution[0] + distribution[1] + distribution[2] + distribution[3]) rolledCost = 4;
        else rolledCost = 5;

        int cost = getCost(rolledCost);
        gamestate.shop[i] = cost == 0 ? nullChamp : getChamp(cost);
    }

    for (int i = 0; i < 5; i++) {
        gamestate.shopSameChampion[i] = highlight(gamestate.shop[i]);
        gamestate.shopStarUppable[i] = wouldStarUp(gamestate.shop[i]);
    }
}

// buy unit
void Engine::buy(int shopindex) {
    if (shopindex < 0 || shopindex > 4) return;
    if (gamestate.shop[shopindex] == nullChamp) return;
    if (gamestate.gold < gamestate.shop[shopindex].cost) return;

    int emptySlot = -1;
    for (int i = 0; i < 9; i++) {
        if (gamestate.bench[i] == nullChamp) {
            emptySlot = i;
            break;
        }
    }
    if (emptySlot == -1 && !wouldStarUp(gamestate.shop[shopindex])) return;

    

    Champion bought = gamestate.shop[shopindex];
    if (bought == nullChamp) return;

    if (emptySlot != -1) {
        gamestate.bench[emptySlot] = bought;
    } else {
        gamestate.tempSlot = bought;
    }
    gamestate.gold -= bought.cost;
    gamestate.shop[shopindex] = nullChamp;
    updateGamestate();
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

    for (int i = 0; i < sold.items.size(); i++) {
        gamestate.items.push_back(sold.items[i]);
    }

    int copies = 1;
    if (sold.starLevel == 2) copies = 3;
    else if (sold.starLevel == 3) copies = 9;

    gamestate.pool[sold.id] = min(gamestate.pool[sold.id] + copies, maxCopies);

    gamestate.board[index.first][index.second] = nullChamp;
    gamestate.boardUnitCount--;
    updateGamestate();
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

    for (int i = 0; i < sold.items.size(); i++) {
        gamestate.items.push_back(sold.items[i]);
    }

    int copies = 1;
    if (sold.starLevel == 2) copies = 3;
    else if (sold.starLevel == 3) copies = 9;

    gamestate.pool[sold.id] = min(gamestate.pool[sold.id] + copies, maxCopies);

    gamestate.bench[index] = nullChamp;
    updateGamestate();
}

// lock shop
void Engine::lockshop() {
    gamestate.shoplocked = !gamestate.shoplocked;
}

// movements
// bench to bench
void Engine::benchtobench(int from, int to) {
    if (from > 8 || from < 0) return;
    if (to > 8 || to < 0) return;
    if (gamestate.bench[from] == nullChamp) return;

    Champion temp = gamestate.bench[to];
    gamestate.bench[to] = gamestate.bench[from];
    gamestate.bench[from] = temp;
    updateGamestate();
}
// bench to board
void Engine::benchtoboard(int from, pair<int,int> to) {
    if (gamestate.bench[from] == nullChamp) return;
    if (gamestate.board[to.first][to.second] == nullChamp && gamestate.boardUnitCount >= gamestate.level) return;
    if (gamestate.board[to.first][to.second] == nullChamp && gamestate.boardUnitCount < gamestate.level) {
        gamestate.boardUnitCount++;
    }

    Champion temp = gamestate.board[to.first][to.second];
    gamestate.board[to.first][to.second] = gamestate.bench[from];
    gamestate.bench[from] = temp;
    updateGamestate();
}
// board to board
void Engine::boardtoboard(pair<int,int> from, pair<int,int> to) {
    if (gamestate.board[from.first][from.second] == nullChamp) return;

    Champion temp = gamestate.board[to.first][to.second];
    gamestate.board[to.first][to.second] = gamestate.board[from.first][from.second];
    gamestate.board[from.first][from.second] = temp;
    updateGamestate();
}
// board to bench
void Engine::boardtobench(pair<int,int> from, int to) {
    if (gamestate.board[from.first][from.second] == nullChamp) return;
    if (gamestate.bench[to] == nullChamp) gamestate.boardUnitCount--;

    Champion temp = gamestate.bench[to];
    gamestate.bench[to] = gamestate.board[from.first][from.second];
    gamestate.board[from.first][from.second] = temp;
    updateGamestate();
}
// swap units

// bench unit
void Engine::benchUnit(pair<int,int> from) {
    return;
}
// field unit 
void Engine::fieldUnit(int from) {
    return;
}


// items
// slam item
void Engine::slamBoard(int index, pair<int,int> position) {
    if (holds_alternative<int>(gamestate.items[index]) && get<int>(gamestate.items[index]) == -1) return;
    if (gamestate.board[position.first][position.second] == nullChamp) return;
    if (gamestate.board[position.first][position.second].items.size() == 3) return;

    gamestate.board[position.first][position.second].items.push_back(gamestate.items[index]);
    gamestate.items.erase(gamestate.items.begin() + index);
    updateGamestate();
}
void Engine::slamBench(int index, int position) {
    if (holds_alternative<int>(gamestate.items[index]) && get<int>(gamestate.items[index]) == -1) return;
    if (gamestate.bench[position] == nullChamp) return;
    if (gamestate.bench[position].items.size() == 3) return;

    gamestate.bench[position].items.push_back(gamestate.items[index]);
    gamestate.items.erase(gamestate.items.begin() + index);
}
// combine items
void Engine::combine(int index1, int index2) {
    if (holds_alternative<int>(gamestate.items[index1]) && get<int>(gamestate.items[index1]) == -1) return;
    if (holds_alternative<int>(gamestate.items[index2]) && get<int>(gamestate.items[index2]) == -1) return;

    // both must be components
    if (!holds_alternative<int>(gamestate.items[index1])) return;
    if (!holds_alternative<int>(gamestate.items[index2])) return;

    int a = get<int>(gamestate.items[index1]);
    int b = get<int>(gamestate.items[index2]);
    if (a > b) swap(a, b);

    pair<int,int> combined = {a, b};
    if (completedItems.find(combined) == completedItems.end()) return;

    gamestate.items[index1] = combined;
    gamestate.items[index2] = -1;
}
// use reforger
void Engine::reforgerBench(int reforger, int index) {
    if (gamestate.bench[index] == nullChamp) return;
    if (gamestate.bench[index].items.empty()) return;

    uniform_int_distribution<int> itemDist(0, gamestate.bench[index].items.size() - 1);
    int itemIndex = itemDist(rng);
    auto& oldItem = gamestate.bench[index].items[itemIndex];

    if (holds_alternative<int>(oldItem)) {
        oldItem = reforgeComponent(get<int>(oldItem), rng);
    } else {
        oldItem = reforgeItem(get<pair<int,int>>(oldItem), rng);
    }

    gamestate.items.erase(gamestate.items.begin() + reforger);
}

void Engine::reforgerBoard(int reforger, pair<int,int> index) {
    if (gamestate.board[index.first][index.second] == nullChamp) return;
    if (gamestate.board[index.first][index.second].items.empty()) return;

    uniform_int_distribution<int> itemDist(0, gamestate.board[index.first][index.second].items.size() - 1);
    int itemIndex = itemDist(rng);
    auto& oldItem = gamestate.board[index.first][index.second].items[itemIndex];

    if (holds_alternative<int>(oldItem)) {
        oldItem = reforgeComponent(get<int>(oldItem), rng);
    } else {
        oldItem = reforgeItem(get<pair<int,int>>(oldItem), rng);
    }

    gamestate.items.erase(gamestate.items.begin() + reforger);
}

bool Engine::isEmblem(pair<int,int> item) {
    if (item == make_pair(3,3) || item == make_pair(3,9) || item == make_pair(9,9)) return false;
    return item.first == 3 || item.first == 9 || item.second == 3 || item.second == 9;
}

bool Engine::isTacTrio(pair<int,int> item) {
    return item == make_pair(3,3) || item == make_pair(3,9) || item == make_pair(9,9);
}

pair<int,int> Engine::reforgeItem(pair<int,int> old, mt19937& rng) {
    // tac trio
    if (isTacTrio(old)) {
        vector<pair<int,int>> options = {{3,3}, {3,9}, {9,9}};
        options.erase(remove(options.begin(), options.end(), old), options.end());
        uniform_int_distribution<int> dist(0, options.size() - 1);
        return options[dist(rng)];
    }

    // emblem
    if (isEmblem(old)) {
        vector<pair<int,int>> emblems;
        for (auto& item : completedItems) {
            if (isEmblem(item.first) && item.first != old) {
                emblems.push_back(item.first);
            }
        }
        uniform_int_distribution<int> dist(0, emblems.size() - 1);
        return emblems[dist(rng)];
    }

    // regular completed
    vector<pair<int,int>> regulars;
    for (auto& item : completedItems) {
        if (!isEmblem(item.first) && !isTacTrio(item.first) && item.first != old) {
            regulars.push_back(item.first);
        }
    }
    uniform_int_distribution<int> dist(0, regulars.size() - 1);
    return regulars[dist(rng)];
}

int Engine::reforgeComponent(int old, mt19937& rng) {
    if (old == 3) return 9;
    if (old == 9) return 3;
    vector<int> components = {1, 2, 4, 5, 6, 7, 8, 10};
    components.erase(remove(components.begin(), components.end(), old), components.end());
    uniform_int_distribution<int> dist(0, components.size() - 1);
    return components[dist(rng)];
}
// use remover
void Engine::removerBench(int remover, int index) {
    if (gamestate.bench[index] == nullChamp) return;
    if (gamestate.bench[index].items.empty()) return;

    for (auto& item : gamestate.bench[index].items) {
        gamestate.items.push_back(item);
    }
    gamestate.bench[index].items.clear();
    gamestate.items.erase(gamestate.items.begin() + remover);
}

void Engine::removerBoard(int remover, pair<int,int> index) {
    if (gamestate.board[index.first][index.second] == nullChamp) return;
    if (gamestate.board[index.first][index.second].items.empty()) return;

    for (auto& item : gamestate.board[index.first][index.second].items) {
        gamestate.items.push_back(item);
    }
    gamestate.board[index.first][index.second].items.clear();
    gamestate.items.erase(gamestate.items.begin() + remover);
}

void Engine::updateGamestate() {
    checkStarUp();

    if (!(gamestate.tempSlot == nullChamp)) {
        for (int i = 0; i < 9; i++) {
            if (gamestate.bench[i] == nullChamp) {
                gamestate.bench[i] = gamestate.tempSlot;
                gamestate.tempSlot = nullChamp;
                break;
            }
        }
    }

    unordered_map<int, int> activeTraits;
    unordered_set<int> countedChampions;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 7; col++) {
            const Champion& champion = gamestate.board[row][col];

            if (champion == nullChamp) continue;

            // Duplicate copies of the same champion do not add traits again.
            if (countedChampions.count(champion.id)) continue;

            countedChampions.insert(champion.id);

            for (int trait : champion.traits) {
                activeTraits[trait]++;
            }
        }
    }

    gamestate.activeTraits = activeTraits;
}

void Engine::checkStarUp() {
    bool starredUp = false;

    // key = {champion id, star level}
    map<pair<int, int>, vector<pair<int, bool>>> champLocations;

    for (int i = 0; i < gamestate.bench.size(); i++) {
        if (!(gamestate.bench[i] == nullChamp)) {
            Champion& champ = gamestate.bench[i];

            if (champ.starLevel < 3) {
                champLocations[{champ.id, champ.starLevel}].push_back({i, true});
            }
        }
    }

    if (!(gamestate.tempSlot == nullChamp)) {
        Champion& champ = gamestate.tempSlot;
        if (champ.starLevel < 3) {
            champLocations[{champ.id, champ.starLevel}].push_back({-1, true}); // -1 = tempSlot
        }
    }

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 7; col++) {
            if (!(gamestate.board[row][col] == nullChamp)) {
                Champion& champ = gamestate.board[row][col];

                if (champ.starLevel < 3) {
                    int flatIndex = row * 7 + col;
                    champLocations[{champ.id, champ.starLevel}].push_back({flatIndex, false});
                }
            }
        }
    }

    auto getChampAt = [&](int idx, bool isBench) -> Champion& {
        if (idx == -1) return gamestate.tempSlot;
        if (isBench) return gamestate.bench[idx];
        return gamestate.board[idx / 7][idx % 7];
    };

    auto score = [&](pair<int, bool> location) {
        int idx = location.first;
        bool isBench = location.second;
        if (idx == -1) return -1;

        Champion& c = getChampAt(idx, isBench);

        int s = 0;

        for (auto& item : c.items) {
            bool completed = holds_alternative<pair<int, int>>(item);

            if (!isBench) {
                s += completed ? 3 : 1;
            } else {
                s += completed ? 2 : 0;
            }
        }

        if (!isBench) s += 100;

        return s;
    };

    for (auto& [key, locations] : champLocations) {
        if (locations.size() < 3) continue;

        // priotise 1 copy
        sort(locations.begin(), locations.end(), [&](pair<int, bool> a, pair<int, bool> b) {
            return score(a) > score(b);
        });

        // takes 3 copies
        vector<pair<int, bool>> consumed = {
            locations[0],
            locations[1],
            locations[2]
        };

        pair<int, bool> bestLocation = consumed[0];

        vector<variant<int, pair<int, int>>> completedBoard;
        vector<variant<int, pair<int, int>>> completedBench;
        vector<variant<int, pair<int, int>>> componentBoard;
        vector<variant<int, pair<int, int>>> componentBench;

        // collect items from 3 copies
        for (auto& location : consumed) {
            int idx = location.first;
            bool isBench = location.second;

            Champion& c = getChampAt(idx, isBench);

            for (auto& item : c.items) {
                bool completed = holds_alternative<pair<int, int>>(item);

                if (!isBench && completed) {
                    completedBoard.push_back(item);
                } else if (isBench && completed) {
                    completedBench.push_back(item);
                } else if (!isBench) {
                    componentBoard.push_back(item);
                } else {
                    componentBench.push_back(item);
                }
            }
        }

        vector<variant<int, pair<int, int>>> newItems;

        for (auto& item : completedBoard) {
            if (newItems.size() >= 3) break;
            newItems.push_back(item);
        }

        for (auto& item : completedBench) {
            if (newItems.size() >= 3) break;
            newItems.push_back(item);
        }

        for (auto& item : componentBoard) {
            if (newItems.size() >= 3) break;
            newItems.push_back(item);
        }

        for (auto& item : componentBench) {
            if (newItems.size() >= 3) break;
            newItems.push_back(item);
        }

        Champion& best = getChampAt(bestLocation.first, bestLocation.second);
        best.starLevel++;
        best.items = newItems;

        // delete 2 other copies
        for (int i = 1; i < consumed.size(); i++) {
            int idx = consumed[i].first;
            bool isBench = consumed[i].second;

            getChampAt(idx, isBench) = nullChamp;

            if (!isBench) {
                gamestate.boardUnitCount--;
            }
        }

        starredUp = true;
        break;
    }

    if (starredUp) {
        checkStarUp();
    }
}

bool Engine::wouldStarUp(Champion champ) {
    if (champ == nullChamp) return false;
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if (gamestate.bench[i].id == champ.id && gamestate.bench[i].starLevel == champ.starLevel) count++;
    }
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 7; col++) {
            if (gamestate.board[row][col].id == champ.id && gamestate.board[row][col].starLevel == champ.starLevel) count++;
        }
    }
    return count == 2;
}

bool Engine::highlight(Champion champ) {
    if (champ == nullChamp) return false;
    for (int i = 0; i < 9; i++) {
        if (gamestate.bench[i].id == champ.id) return true;
    }
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 7; col++) {
            if (gamestate.board[row][col].id == champ.id) return true;
        }
    }
    return false;
}

bool Engine::canEquipItems(Champion& champ, Item item) {
    return false;
}

void Engine::checkTraits() {
    return;
}

void Engine::combineItems(Champion& champ, Item item) {
    return;
}

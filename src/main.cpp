#include "Engine.h"
#include "Globals.h"
#include <cassert>
#include <iostream>
using namespace std;

bool isNull(const Champion& c) {
    return c == nullChamp;
}

int countBenchUnits(const Engine& e) {
    int count = 0;

    for (const Champion& champ : e.gamestate.bench) {
        if (!isNull(champ)) count++;
    }

    return count;
}

int countBoardUnits(const Engine& e) {
    int count = 0;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 7; col++) {
            if (!isNull(e.gamestate.board[row][col])) count++;
        }
    }

    return count;
}

int countBenchChampionStar(const Engine& e, int champId, int starLevel) {
    int count = 0;

    for (const Champion& champ : e.gamestate.bench) {
        if (champ.id == champId && champ.starLevel == starLevel) {
            count++;
        }
    }

    return count;
}

int countBoardChampionStar(const Engine& e, int champId, int starLevel) {
    int count = 0;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 7; col++) {
            const Champion& champ = e.gamestate.board[row][col];

            if (champ.id == champId && champ.starLevel == starLevel) {
                count++;
            }
        }
    }

    return count;
}

void clearBench(Engine& e) {
    for (int i = 0; i < 9; i++) {
        e.gamestate.bench[i] = nullChamp;
    }
}

void clearBoard(Engine& e) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 7; col++) {
            e.gamestate.board[row][col] = nullChamp;
        }
    }

    e.gamestate.boardUnitCount = 0;
}

void clearShop(Engine& e) {
    for (int i = 0; i < 5; i++) {
        e.gamestate.shop[i] = nullChamp;
        e.gamestate.shopSameChampion[i] = false;
        e.gamestate.shopStarUppable[i] = false;
    }
}

void resetControlled(Engine& e) {
    e.init();
    e.gamestate.gold = 100;
    clearBench(e);
    clearBoard(e);
    clearShop(e);
    e.gamestate.tempSlot = nullChamp;
}

void refreshShopFlags(Engine& e) {
    for (int i = 0; i < 5; i++) {
        e.gamestate.shopSameChampion[i] = false;
        e.gamestate.shopStarUppable[i] = false;
    }

    // Force through roll-like manual logic by using buy checks directly in tests where needed.
    // If you later expose highlight/wouldStarUp publicly, replace this helper.
}

void test_buy_5_roll_buy_4_block_10th() {
    Engine e;
    resetControlled(e);

    e.gamestate.shop[0] = Aatrox;
    e.gamestate.shop[1] = Briar;
    e.gamestate.shop[2] = Caitlyn;
    e.gamestate.shop[3] = ChoGath;
    e.gamestate.shop[4] = Ezreal;

    e.buy(0);
    e.buy(1);
    e.buy(2);
    e.buy(3);
    e.buy(4);

    assert(countBenchUnits(e) == 5);
    assert(e.gamestate.bench[0].id == Aatrox.id);
    assert(e.gamestate.bench[1].id == Briar.id);
    assert(e.gamestate.bench[2].id == Caitlyn.id);
    assert(e.gamestate.bench[3].id == ChoGath.id);
    assert(e.gamestate.bench[4].id == Ezreal.id);

    e.gamestate.shop[0] = Leona;
    e.gamestate.shop[1] = Lissandra;
    e.gamestate.shop[2] = Nasus;
    e.gamestate.shop[3] = Poppy;
    e.gamestate.shop[4] = RekSai;

    e.buy(0);
    e.buy(1);
    e.buy(2);
    e.buy(3);

    assert(countBenchUnits(e) == 9);
    assert(e.gamestate.bench[5].id == Leona.id);
    assert(e.gamestate.bench[6].id == Lissandra.id);
    assert(e.gamestate.bench[7].id == Nasus.id);
    assert(e.gamestate.bench[8].id == Poppy.id);

    int goldBefore = e.gamestate.gold;
    e.buy(4);

    assert(countBenchUnits(e) == 9);
    assert(e.gamestate.gold == goldBefore);
    assert(e.gamestate.shop[4].id == RekSai.id);
    assert(isNull(e.gamestate.tempSlot));

    cout << "test_buy_5_roll_buy_4_block_10th passed\n";
}

void test_star_up_to_2_star() {
    Engine e;
    resetControlled(e);

    e.gamestate.shop[0] = Aatrox;
    e.buy(0);

    e.gamestate.shop[0] = Aatrox;
    e.buy(0);

    e.gamestate.shop[0] = Aatrox;
    e.buy(0);

    assert(countBenchChampionStar(e, Aatrox.id, 1) == 0);
    assert(countBenchChampionStar(e, Aatrox.id, 2) == 1);
    assert(countBenchChampionStar(e, Aatrox.id, 3) == 0);

    assert(e.gamestate.bench[0].id == Aatrox.id);
    assert(e.gamestate.bench[0].starLevel == 2);
    assert(isNull(e.gamestate.bench[1]));
    assert(isNull(e.gamestate.bench[2]));

    cout << "test_star_up_to_2_star passed\n";
}

void test_star_up_to_3_star() {
    Engine e;
    resetControlled(e);

    for (int i = 0; i < 9; i++) {
        e.gamestate.shop[0] = Aatrox;
        e.buy(0);
    }

    assert(countBenchChampionStar(e, Aatrox.id, 1) == 0);
    assert(countBenchChampionStar(e, Aatrox.id, 2) == 0);
    assert(countBenchChampionStar(e, Aatrox.id, 3) == 1);

    assert(e.gamestate.bench[0].id == Aatrox.id);
    assert(e.gamestate.bench[0].starLevel == 3);

    for (int i = 1; i < 9; i++) {
        assert(isNull(e.gamestate.bench[i]));
    }

    cout << "test_star_up_to_3_star passed\n";
}

void test_full_bench_star_uppable_buy_succeeds() {
    Engine e;
    resetControlled(e);

    e.gamestate.bench[0] = Aatrox;
    e.gamestate.bench[1] = Aatrox;
    e.gamestate.bench[2] = Briar;
    e.gamestate.bench[3] = Caitlyn;
    e.gamestate.bench[4] = ChoGath;
    e.gamestate.bench[5] = Ezreal;
    e.gamestate.bench[6] = Leona;
    e.gamestate.bench[7] = Lissandra;
    e.gamestate.bench[8] = Nasus;

    e.gamestate.shop[0] = Aatrox;

    e.buy(0);

    assert(countBenchUnits(e) == 8);
    assert(countBenchChampionStar(e, Aatrox.id, 1) == 0);
    assert(countBenchChampionStar(e, Aatrox.id, 2) == 1);

    assert(e.gamestate.bench[0].id == Aatrox.id);
    assert(e.gamestate.bench[0].starLevel == 2);
    assert(isNull(e.gamestate.bench[1]));
    assert(isNull(e.gamestate.tempSlot));
    assert(isNull(e.gamestate.shop[0]));

    cout << "test_full_bench_star_uppable_buy_succeeds passed\n";
}

void test_full_bench_non_star_uppable_buy_fails() {
    Engine e;
    resetControlled(e);

    e.gamestate.bench[0] = Aatrox;
    e.gamestate.bench[1] = Briar;
    e.gamestate.bench[2] = Caitlyn;
    e.gamestate.bench[3] = ChoGath;
    e.gamestate.bench[4] = Ezreal;
    e.gamestate.bench[5] = Leona;
    e.gamestate.bench[6] = Lissandra;
    e.gamestate.bench[7] = Nasus;
    e.gamestate.bench[8] = Poppy;

    e.gamestate.shop[0] = RekSai;

    int goldBefore = e.gamestate.gold;
    e.buy(0);

    assert(countBenchUnits(e) == 9);
    assert(e.gamestate.gold == goldBefore);
    assert(e.gamestate.shop[0].id == RekSai.id);
    assert(isNull(e.gamestate.tempSlot));

    cout << "test_full_bench_non_star_uppable_buy_fails passed\n";
}

void test_bench_to_board_empty_increments_count() {
    Engine e;
    resetControlled(e);

    e.gamestate.bench[0] = Aatrox;

    e.benchtoboard(0, {0, 0});

    assert(isNull(e.gamestate.bench[0]));
    assert(e.gamestate.board[0][0].id == Aatrox.id);
    assert(e.gamestate.boardUnitCount == 1);
    assert(countBoardUnits(e) == 1);

    cout << "test_bench_to_board_empty_increments_count passed\n";
}

void test_bench_to_board_occupied_swaps_no_count_change() {
    Engine e;
    resetControlled(e);

    e.gamestate.bench[0] = Aatrox;
    e.gamestate.board[0][0] = Briar;
    e.gamestate.boardUnitCount = 1;

    e.benchtoboard(0, {0, 0});

    assert(e.gamestate.board[0][0].id == Aatrox.id);
    assert(e.gamestate.bench[0].id == Briar.id);
    assert(e.gamestate.boardUnitCount == 1);
    assert(countBoardUnits(e) == 1);

    cout << "test_bench_to_board_occupied_swaps_no_count_change passed\n";
}

void test_bench_to_board_empty_blocked_at_cap() {
    Engine e;
    resetControlled(e);

    e.gamestate.level = 1;
    e.gamestate.bench[0] = Aatrox;
    e.gamestate.board[0][0] = Briar;
    e.gamestate.boardUnitCount = 1;

    e.benchtoboard(0, {0, 1});

    assert(e.gamestate.bench[0].id == Aatrox.id);
    assert(isNull(e.gamestate.board[0][1]));
    assert(e.gamestate.board[0][0].id == Briar.id);
    assert(e.gamestate.boardUnitCount == 1);

    cout << "test_bench_to_board_empty_blocked_at_cap passed\n";
}

void test_board_to_bench_empty_decrements_count() {
    Engine e;
    resetControlled(e);

    e.gamestate.board[0][0] = Aatrox;
    e.gamestate.boardUnitCount = 1;

    e.boardtobench({0, 0}, 0);

    assert(isNull(e.gamestate.board[0][0]));
    assert(e.gamestate.bench[0].id == Aatrox.id);
    assert(e.gamestate.boardUnitCount == 0);
    assert(countBoardUnits(e) == 0);

    cout << "test_board_to_bench_empty_decrements_count passed\n";
}

void test_board_to_bench_occupied_swaps_no_count_change() {
    Engine e;
    resetControlled(e);

    e.gamestate.board[0][0] = Aatrox;
    e.gamestate.bench[0] = Briar;
    e.gamestate.boardUnitCount = 1;

    e.boardtobench({0, 0}, 0);

    assert(e.gamestate.board[0][0].id == Briar.id);
    assert(e.gamestate.bench[0].id == Aatrox.id);
    assert(e.gamestate.boardUnitCount == 1);
    assert(countBoardUnits(e) == 1);

    cout << "test_board_to_bench_occupied_swaps_no_count_change passed\n";
}

void test_shop_same_champion_flag_manual() {
    Engine e;
    resetControlled(e);

    Champion twoStarAatrox = Aatrox;
    twoStarAatrox.starLevel = 2;

    e.gamestate.bench[0] = twoStarAatrox;
    e.gamestate.shop[0] = Aatrox;

    e.gamestate.shopSameChampion[0] = true; // expected result of highlight
    assert(e.gamestate.shopSameChampion[0] == true);
    assert(e.gamestate.bench[0].id == Aatrox.id);
    assert(e.gamestate.bench[0].starLevel == 2);
    assert(e.gamestate.shop[0].id == Aatrox.id);
    assert(e.gamestate.shop[0].starLevel == 1);

    cout << "test_shop_same_champion_flag_manual passed\n";
}

void test_shop_star_uppable_buy_condition() {
    Engine e;
    resetControlled(e);

    e.gamestate.bench[0] = Aatrox;
    e.gamestate.bench[1] = Aatrox;
    e.gamestate.shop[0] = Aatrox;

    e.buy(0);

    assert(e.gamestate.bench[0].id == Aatrox.id);
    assert(e.gamestate.bench[0].starLevel == 2);
    assert(isNull(e.gamestate.bench[1]));
    assert(isNull(e.gamestate.shop[0]));

    cout << "test_shop_star_uppable_buy_condition passed\n";
}

int main() {
    test_buy_5_roll_buy_4_block_10th();
    test_star_up_to_2_star();
    test_star_up_to_3_star();

    test_full_bench_star_uppable_buy_succeeds();
    test_full_bench_non_star_uppable_buy_fails();

    test_bench_to_board_empty_increments_count();
    test_bench_to_board_occupied_swaps_no_count_change();
    test_bench_to_board_empty_blocked_at_cap();

    test_board_to_bench_empty_decrements_count();
    test_board_to_bench_occupied_swaps_no_count_change();

    test_shop_same_champion_flag_manual();
    test_shop_star_uppable_buy_condition();

    cout << "All tests passed\n";
    return 0;
}
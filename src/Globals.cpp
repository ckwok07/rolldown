#include "Globals.h"
#include "Champion.h"
#include <unordered_map>

const vector<vector<int>> shopodds = {{100, 0, 0, 0, 0}, // 2
                                    {75, 25, 0, 0, 0}, // 3
                                    {55, 30, 15, 0, 0}, // 4
                                    {45, 33, 20, 2, 0}, // 5
                                    {30, 40, 25, 5, 0}, // 6
                                    {19, 30, 40, 10, 1}, // 7
                                    {15, 20, 32, 30, 3}, // 8
                                    {10, 17, 25, 33, 15}, // 9
                                    {5, 10, 20, 40, 25}}; // 10

const vector<int> levelthresholds = {0,0,2, 6, 10, 20, 36, 60, 68, 68};

const Champion nullChamp = {0, "null", 0, 0, {}, {}};

const Champion Aatrox = {1, "Aatrox", 1, 1, {13, 22}, {}};
const Champion Briar = {2, "Briar", 1, 1, {1, 16, 30}, {}};
const Champion Caitlyn = {3, "Caitlyn", 1, 1, {13, 27}, {}};
const Champion ChoGath = {4, "Cho'Gath", 1, 1, {6, 23}, {}};
const Champion Ezreal = {5, "Ezreal", 1, 1, {21, 32}, {}};
const Champion Leona = {6, "Leona", 1, 1, {2, 33}, {}};
const Champion Lissandra = {7, "Lissandra", 1, 1, {6, 31, 29}, {}};
const Champion Nasus = {8, "Nasus", 1, 1, {19, 33}, {}};
const Champion Poppy = {9, "Poppy", 1, 1, {12, 22}, {}};
const Champion RekSai = {10, "Rek'Sai", 1, 1, {16, 23}, {}};
const Champion Talon = {11, "Talon", 1, 1, {20, 30}, {}};
const Champion Teemo = {12, "Teemo", 1, 1, {19, 31}, {}};
const Champion TF = {13, "Twisted Fate", 1, 1, {20, 27}, {}};
const Champion Veigar = {14, "Veigar", 1, 1, {12, 29}, {}};

const Champion Akali = {15, "Akali", 2, 1, {13, 28}, {}};
const Champion Belveth = {16, "Bel'veth", 2, 1, {16, 24, 28}, {}};
const Champion Gnar = {17, "Gnar", 2, 1, {12, 32}, {}};
const Champion Gragas = {18, "Gragas", 2, 1, {17, 23}, {}};
const Champion Gwen = {19, "Gwen", 2, 1, {19, 30}, {}};
const Champion Jax = {20, "Jax", 2, 1, {20, 22}, {}};
const Champion Jinx = {21, "Jinx", 2, 1, {1, 24}, {}};
const Champion Meepsie = {22, "Meepsie", 2, 1, {12, 31, 34}, {}};
const Champion Milio = {23, "Milio", 2, 1, {21, 27}, {}};
const Champion Mordekaiser = {24, "Mordekaiser", 2, 1, {6, 25, 33}, {}};
const Champion Pantheon = {25, "Pantheon", 2, 1, {21, 23, 29}, {}};
const Champion Pyke = {26, "Pyke", 2, 1, {17, 34}, {}};
const Champion Zoe = {27, "Zoe", 2, 1, {2, 25}, {}};

const Champion Aurora = {28, "Aurora", 3, 1, {1, 34}, {}};
const Champion Diana = {29, "Diana", 3, 1, {2, 24}, {}};
const Champion Fizz = {30, "Fizz", 3, 1, {12, 30}, {}};
const Champion Illaoi = {31, "Illaoi", 3, 1, {1, 33, 31}, {}};
const Champion Kaisa = {32, "Kai'Sa", 3, 1, {6, 30}, {}};
const Champion Lulu = {33, "Lulu", 3, 1, {20, 29}, {}};
const Champion Maokai = {34, "Maokai", 3, 1, {13, 23}, {}};
const Champion MF = {35, "Miss Fortune", 3, 1, {35, 36}, {}};
const Champion Ornn = {36, "Ornn", 3, 1, {19, 22}, {}};
const Champion Rhaast = {37, "Rhaast", 3, 1, {18}, {}};
const Champion Samira = {38, "Samira", 3, 1, {19, 32}, {}};
const Champion Urgot = {39, "Urgot", 3, 1, {11, 23, 28}, {}};
const Champion Viktor = {40, "Viktor", 3, 1, {17, 25}, {}};

const Champion Asol = {41, "Aurelion Sol", 4, 1, {11, 25}, {}};
const Champion Corki = {42, "Corki", 4, 1, {12, 27}, {}};
const Champion Karma = {43, "Karma", 4, 1, {6, 34}, {}};
const Champion Kindred = {44, "Kindred", 4, 1, {13, 24}, {}};
const Champion Leblanc = {45, "LeBlanc", 4, 1, {2, 31}, {}};
const Champion Yi = {46, "Master Yi", 4, 1, {17, 28}, {}};
const Champion Nami = {47, "Nami", 4, 1, {19, 29}, {}};
const Champion Nunu = {48, "Nunu", 4, 1, {20, 33}, {}};
const Champion Rammus = {49, "Rammus", 4, 1, {12, 22}, {}};
const Champion Riven = {50, "Riven", 4, 1, {21, 30}, {}};
const Champion TK = {51, "Tahm Kench", 4, 1, {14, 23}, {}};
const Champion Mech = {52, "The Mighty Mech", 4, 1, {11, 34}, {}};
const Champion Xayah = {53, "Xayah", 4, 1, {20, 32}, {}};

const Champion Bard = {54, "Bard", 5, 1, {12, 25}, {}};
const Champion Blitz = {55, "Blitzcrank", 5, 1, {15, 19, 33}, {}};
const Champion Fiora = {56, "Fiora", 5, 1, {7, 1, 28}, {}};
const Champion Graves = {57, "Graves", 5, 1, {9}, {}};
const Champion Jhin = {58, "Jhin", 5, 1, {6, 26, 32}, {}};
const Champion Morgana = {59, "Morgana", 5, 1, {5}, {}};
const Champion Shen = {60, "Shen", 5, 1, {3, 22}, {}};
const Champion Sona = {61, "Sona", 5, 1, {4, 17, 31}, {}};
const Champion Vex = {62, "Vex", 5, 1, {8}, {}};
const Champion Zed = {63, "Zed", 5, 1, {10}, {}};

const vector<Champion> ALL_CHAMPIONS = {
    // 1-cost
    Aatrox, Briar, Caitlyn, ChoGath, Ezreal, Leona, Lissandra,
    Nasus, Poppy, RekSai, Talon, Teemo, TF, Veigar,

    // 2-cost
    Akali, Belveth, Gnar, Gragas, Gwen, Jax, Jinx, Meepsie,
    Milio, Mordekaiser, Pantheon, Pyke, Zoe,

    // 3-cost
    Aurora, Diana, Fizz, Illaoi, Kaisa, Lulu, Maokai, MF,
    Ornn, Rhaast, Samira, Urgot, Viktor,

    // 4-cost
    Asol, Corki, Karma, Kindred, Leblanc, Yi, Nami, Nunu,
    Rammus, Riven, TK, Mech, Xayah,

    // 5-cost
    Bard, Blitz, Fiora, Graves, Jhin, Morgana, Shen, Sona, Vex, Zed
};
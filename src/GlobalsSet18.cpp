#include "GlobalsSet18.h"
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

const vector<int> levelthresholds = {0, 0, 2, 6, 10, 20, 36, 60, 68, 68, 0};

const Champion nullChamp = {0, "null", 0, 0, {}, {}};

const unordered_map<int, string> itemComponents = {{1, "BF Sword"}, 
                                                {2, "Chain Vest"}, 
                                                {3, "Frying Pan"}, 
                                                {4, "Giant's Belt"}, 
                                                {5, "Needlessly Large Rod"}, 
                                                {6, "Negatron Cloak"}, 
                                                {7, "Recurve Bow"}, 
                                                {8, "Sparring Gloves"}, 
                                                {9, "Spatula"}, 
                                                {10, "Tear of the Goddess"}};
const map<pair<int, int>, string> completedItems = {{{1, 1}, "Deathblade"},
                                                {{1, 2}, "Edge of Night"},
                                                {{1, 3}, "Marauder Emblem"},
                                                {{1, 4}, "Sterak's Gage"},
                                                {{1, 5}, "Hextech Gunblade"},
                                                {{1, 6}, "Bloodthirster"},
                                                {{1, 7}, "Giant Slayer"},
                                                {{1, 8}, "Infinity Edge"},
                                                {{1, 9}, "Dark Star Emblem"},
                                                {{1, 10}, "Spear of Shojin"},

                                                {{2, 2}, "Bramble Vest"},
                                                {{2, 3}, "Bastion Emblem"},
                                                {{2, 4}, "Sunfire Cape"},
                                                {{2, 5}, "Crownguard"},
                                                {{2, 6}, "Gargoyle Stoneplate"},
                                                {{2, 7}, "Titan's Resolve"},
                                                {{2, 8}, "Steadfast Heart"},
                                                {{2, 9}, "Arbiter Emblem"},
                                                {{2, 10}, "Protector's Vow"},

                                                {{3, 3}, "Tactician's Shield"},
                                                {{3, 4}, "Brawler Emblem"},
                                                {{3, 5}, "Conduit Emblem"},
                                                {{3, 6}, "Voyager Emblem"},
                                                {{3, 7}, "Challenger Emblem"},
                                                {{3, 8}, "Rogue Emblem"},
                                                {{3, 9}, "Tactician's Cape"},
                                                {{3, 10}, "Fateweaver Emblem"},

                                                {{4, 4}, "Warmog's Armor"},
                                                {{4, 5}, "Morellonomicon"},
                                                {{4, 6}, "Spirit Visage"},
                                                {{4, 7}, "Red Buff"},
                                                {{4, 8}, "Guardbreaker"},
                                                {{4, 9}, "Vanguard Emblem"},
                                                {{4, 10}, "Redemption"},

                                                {{5, 5}, "Rabadon's Deathcap"},
                                                {{5, 6}, "Ionic Spark"},
                                                {{5, 7}, "Guinsoo's Rageblade"},
                                                {{5, 8}, "Jeweled Gauntlet"},
                                                {{5, 9}, "N.O.V.A. Emblem"},
                                                {{5, 10}, "Archangel's Staff"},

                                                {{6, 6}, "Dragon's Claw"},
                                                {{6, 7}, "Adaptive Helm"},
                                                {{6, 8}, "Quicksilver"},
                                                {{6, 9}, "Primordian Emblem"},
                                                {{6, 10}, "Evenshroud"},

                                                {{7, 7}, "Kraken's Fury"},
                                                {{7, 8}, "Last Whisper"},
                                                {{7, 9}, "Stargazer Emblem"},
                                                {{7, 10}, "Statikk Shiv"},

                                                {{8, 8}, "Thief's Gloves"},
                                                {{8, 9}, "Meeple Emblem"},
                                                {{8, 10}, "Hand of Justice"},

                                                {{9, 9}, "Tactician's Crown"},
                                                {{9, 10}, "Timebreaker Emblem"},

                                                {{10, 10}, "Blue Buff"}};

const Champion Akali = {1, "Akali", 1, 1, {20, 1, 28}, {}};
const Champion Camille = {2, "Camille", 1, 1, {10, 28}, {}};
const Champion Cinderling = {3, "Cinderling", 1, 1, {29, 19}, {}};
const Champion Karma = {4, "Karma", 1, 1, {6, 32}, {}};
const Champion Kobuko = {5, "Kobuko", 1, 1, {33, 8}, {}};
const Champion Leona = {6, "Leona", 1, 1, {31, 11}, {}};
const Champion Ornn = {7, "Ornn", 1, 1, {13, 21}, {}};
const Champion Pebbles = {8, "Pebbles", 1, 1, {19, 33}, {}};
const Champion Rakan = {9, "Rakan", 1, 1, {16, 22, 36}, {}};
const Champion RekSai = {10, "Rek'Sai", 1, 1, {5, 8}, {}};
const Champion Varus = {11, "Varus", 1, 1, {20, 27}, {}};
const Champion Veigar = {12, "Veigar", 1, 1, {5, 33, 32}, {}};
const Champion Xayah = {13, "Xayah", 1, 1, {13, 16, 27}, {}};
const Champion Yorick = {14, "Yorick", 1, 1, {6, 22, 34}, {}};

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

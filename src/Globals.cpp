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

const vector<int> levelthresholds = {0,0,2, 6, 10, 20, 36, 60, 68, 68, 0};

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
namespace Set17 {
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
    const Champion LeBlanc = {45, "LeBlanc", 4, 1, {2, 31}, {}};
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
        Asol, Corki, Karma, Kindred, LeBlanc, Yi, Nami, Nunu,
        Rammus, Riven, TK, Mech, Xayah,

        // 5-cost
        Bard, Blitz, Fiora, Graves, Jhin, Morgana, Shen, Sona, Vex, Zed
    };
}

namespace Set18 {
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

    const Champion Alistar = {15, "Alistar", 2, 1, {13, 28}, {}};
    const Champion Caitlyn = {16, "Caitlyn", 2, 1, {16, 24, 28}, {}};
    const Champion Elise = {17, "Elise", 2, 1, {12, 32}, {}};
    const Champion Gromp = {18, "Gromp", 2, 1, {17, 23}, {}};
    const Champion Kayle = {19, "Kayle", 2, 1, {19, 30}, {}};
    const Champion LeBlanc = {20, "LeBlanc", 2, 1, {20, 22}, {}};
    const Champion Murkwolf = {21, "Murkwolf", 2, 1, {1, 24}, {}};
    const Champion Scuttlecrab = {22, "Scuttlecrab", 2, 1, {12, 31, 34}, {}};
    const Champion Sejuani = {23, "Sejuani", 2, 1, {21, 27}, {}};
    const Champion Shen = {24, "Shen", 2, 1, {6, 25, 33}, {}};
    const Champion Teemo = {25, "Teemo", 2, 1, {21, 23, 29}, {}};
    const Champion Warwick = {26, "Warwick", 2, 1, {17, 34}, {}};
    const Champion Yunara = {27, "Yunara", 2, 1, {2, 25}, {}};

    const Champion Azir = {28, "Azir", 3, 1, {1, 34}, {}};
    const Champion Cassiopeia = {29, "Cassiopeia", 3, 1, {2, 24}, {}};
    const Champion Diana = {30, "Diana", 3, 1, {12, 30}, {}};
    const Champion Fiddlesticks = {31, "Fiddlesticks", 3, 1, {1, 33, 31}, {}};
    const Champion Hecarim = {32, "Hecarim", 3, 1, {6, 30}, {}};
    const Champion KhaZix = {33, "Kha'Zix", 3, 1, {20, 29}, {}};
    const Champion KogMaw = {34, "Kog'Maw", 3, 1, {13, 23}, {}};
    const Champion Krug = {35, "Krug", 3, 1, {35, 36}, {}};
    const Champion MamaBeak = {36, "Mama Beak", 3, 1, {19, 22}, {}};
    const Champion MasterYi = {37, "Master Yi", 3, 1, {18}, {}};
    const Champion Rammus = {38, "Rammus", 3, 1, {19, 32}, {}};
    const Champion Rengar = {39, "Rengar", 3, 1, {11, 23, 28}, {}};
    const Champion Tristana = {40, "Tristana", 3, 1, {17, 25}, {}};
    const Champion Vi = {41, "Vi", 3, 1, {0, 0}, {}};

    const Champion Ahri = {42, "Ahri", 4, 1, {11, 25}, {}};
    const Champion Amumu = {43, "Amumu", 4, 1, {12, 27}, {}};
    const Champion Aphelios = {44, "Aphelios", 4, 1, {6, 34}, {}};
    const Champion Brambleback = {45, "Brambleback", 4, 1, {13, 24}, {}};
    const Champion Ezreal = {46, "Ezreal", 4, 1, {2, 31}, {}};
    const Champion Lillia = {47, "Lillia", 4, 1, {17, 28}, {}};
    const Champion Malphite = {48, "Malphite", 4, 1, {19, 29}, {}};
    const Champion Morgana = {49, "Morgana", 4, 1, {20, 33}, {}};
    const Champion Nidalee = {50, "Nidalee", 4, 1, {12, 22}, {}};
    const Champion Sentinel = {51, "Sentinel", 4, 1, {21, 30}, {}};
    const Champion Sett = {52, "Sett", 4, 1, {14, 23}, {}};
    const Champion Sivir = {53, "Sivir", 4, 1, {11, 34}, {}};
    const Champion Soraka = {54, "Soraka", 4, 1, {20, 32}, {}};
    const Champion Zyra = {55, "Zyra", 4, 1, {0,0}, {}};

    const Champion Alune = {56, "Alune", 5, 1, {12, 25}, {}};
    const Champion Ashe = {57, "Ashe", 5, 1, {15, 19, 33}, {}};
    const Champion Draven = {58, "Draven", 5, 1, {7, 1, 28}, {}};
    const Champion ElderDragon = {59, "Elder Dragon", 5, 1, {9}, {}};
    const Champion Gnar = {60, "Gnar", 5, 1, {6, 26, 32}, {}};
    const Champion Ivern = {61, "Ivern", 5, 1, {5}, {}};
    const Champion Kennen = {62, "Kennen", 5, 1, {3, 22}, {}};
    const Champion Lux = {63, "Lux", 5, 1, {4, 17, 31}, {}};
    const Champion Maokai = {64, "Maokai", 5, 1, {8}, {}};
    const Champion Taric = {65, "Taric", 5, 1, {10}, {}};

    const vector<Champion> ALL_CHAMPIONS = {
        // 1-cost
        Akali, Camille, Cinderling, Karma, Kobuko, Leona, Ornn,
        Pebbles, Rakan, RekSai, Varus, Veigar, Xayah, Yorick,

        // 2-cost
        Alistar, Caitlyn, Elise, Gromp, Kayle, LeBlanc, Murkwolf,
        Scuttlecrab, Sejuani, Shen, Teemo, Warwick, Yunara,

        // 3-cost
        Azir, Cassiopeia, Diana, Fiddlesticks, Hecarim, KhaZix,
        KogMaw, Krug, MamaBeak, MasterYi, Rammus, Rengar,
        Tristana, Vi,

        // 4-cost
        Ahri, Amumu, Aphelios, Brambleback, Ezreal, Lillia,
        Malphite, Morgana, Nidalee, Sentinel, Sett, Sivir,
        Soraka, Zyra,

        // 5-cost
        Alune, Ashe, Draven, ElderDragon, Gnar, Ivern, Kennen,
        Lux, Maokai, Taric
    };

  const unordered_map<int, vector<int>> ALL_TRAITS = {
    {1, {2, 3, 4}},
    {2, {1}},
    {3, {1}},
    {4, {1}},
    {5, {2,4,6}},
    {6, {3,5,7,9,11}},
    {7, {1}},
    {8, {2,4,6}},
    {9, {1}},
    {10, {3,4,5,7}},
    {11, {2,4,6}},
    {12, {}},
    {13, {3,5,7,9,11}},
    {14, {1}},
    {15, {2,3,4}},
    {16, {2,4}},
    {17, {1,2}},
    {18, {1}},
    {19, {2,3,4,5}},
    {20, {2,3,5,7}},
    {21, {2,3,4,5}},
    {22, {2,4,6}},
    {23, {2,3,4,5}},
    {24, {1}},
    {25, {1}},
    {26, {2,4}},
    {27, {2,3,4,5}},
    {28, {2,4,6}},
    {29, {3,5,7,10}},
    {30, {1,2}},
    {31, {3}},
    {32, {2,4,6}},
    {33, {3,5,7}},
    {34, {2,3}},
    {35, {1}},
    {36, {2,3,6}},
  }
}

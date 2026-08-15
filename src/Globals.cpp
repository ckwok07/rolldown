#include "Globals.h"
#include "Champion.h"
#include <unordered_map>
#include <utility>

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

    const Champion Alistar = {15, "Alistar", 2, 1, {13, 8}, {}};
    const Champion Caitlyn = {16, "Caitlyn", 2, 1, {10,19}, {}};
    const Champion Elise = {17, "Elise", 2, 1, {10, 36}, {}};
    const Champion Gromp = {18, "Gromp", 2, 1, {29, 1}, {}};
    const Champion Kayle = {19, "Kayle", 2, 1, {31, 27}, {}};
    const Champion LeBlanc = {20, "LeBlanc", 2, 1, {13, 32}, {}};
    const Champion Murkwolf = {21, "Murkwolf", 2, 1, {29, 28}, {}};
    const Champion Scuttlecrab = {22, "Scuttlecrab", 2, 1, {29, 22}, {}};
    const Champion Sejuani = {23, "Sejuani", 2, 1, {31, 22}, {}};
    const Champion Shen = {24, "Shen", 2, 1, {20, 11}, {}};
    const Champion Teemo = {25, "Teemo", 2, 1, {33, 21}, {}};
    const Champion Warwick = {26, "Warwick", 2, 1, {5, 28}, {}};
    const Champion Yunara = {27, "Yunara", 2, 1, {6, 15}, {}};

    const Champion Azir = {28, "Azir", 3, 1, {5 ,15, 34}, {}};
    const Champion Cassiopeia = {29, "Cassiopeia", 3, 1, {10, 32}, {}};
    const Champion Diana = {30, "Diana", 3, 1, {23, 28, 36}, {}};
    const Champion Fiddlesticks = {31, "Fiddlesticks", 3, 1, {17, 11, 32}, {}};
    const Champion Hecarim = {32, "Hecarim", 3, 1, {13, 36}, {}};
    const Champion KhaZix = {33, "Kha'Zix", 3, 1, {30}, {}};
    const Champion KogMaw = {34, "Kog'Maw", 3, 1, {9, 1, 21}, {}};
    const Champion Krug = {35, "Krug", 3, 1, {29, 8}, {}};
    const Champion MamaBeak = {36, "Mama Beak", 3, 1, {29, 34, 27}, {}};
    const Champion MasterYi = {37, "Master Yi", 3, 1, {6, 1}, {}};
    const Champion Rammus = {38, "Rammus", 3, 1, {33, 11}, {}};
    const Champion Rengar = {39, "Rengar", 3, 1, {30}, {}};
    const Champion Tristana = {40, "Tristana", 3, 1, {16, 33, 19}, {}};
    const Champion Vi = {41, "Vi", 3, 1, {26, 22}, {}};

    const Champion Ahri = {42, "Ahri", 4, 1, {6, 32}, {}};
    const Champion Amumu = {43, "Amumu", 4, 1, {20, 22}, {}};
    const Champion Aphelios = {44, "Aphelios", 4, 1, {23, 27}, {}};
    const Champion Brambleback = {45, "Brambleback", 4, 1, {29, 28}, {}};
    const Champion Ezreal = {46, "Ezreal", 4, 1, {13,15}, {}};
    const Champion Lillia = {47, "Lillia", 4, 1, {16, 11}, {}};
    const Champion Malphite = {48, "Malphite", 4, 1, {5, 24}, {}};
    const Champion Morgana = {49, "Morgana", 4, 1, {10, 21}, {}};
    const Champion Nidalee = {50, "Nidalee", 4, 1, {26, 1}, {}};
    const Champion Sentinel = {51, "Sentinel", 4, 1, {29, 36 ,21}, {}};
    const Champion Sett = {52, "Sett", 4, 1, {6, 8}, {}};
    const Champion Sivir = {53, "Sivir", 4, 1, {26, 19}, {}};
    const Champion Soraka = {54, "Soraka", 4, 1, {17,15}, {}};
    const Champion Zyra = {55, "Zyra", 4, 1, {35, 34}, {}};

    const Champion Alune = {56, "Alune", 5, 1, {3, 23, 32}, {}};
    const Champion Ashe = {57, "Ashe", 5, 1, {6, 19}, {}};
    const Champion Draven = {58, "Draven", 5, 1, {7}, {}};
    const Champion ElderDragon = {59, "Elder Dragon", 5, 1, {2, 29}, {}};
    const Champion Gnar = {60, "Gnar", 5, 1, {13, 33, 8}, {}};
    const Champion Ivern = {61, "Ivern", 5, 1, {18}, {}};
    const Champion Kennen = {62, "Kennen", 5, 1, {20, 15}, {}};
    const Champion Lux = {63, "Lux", 5, 1, {4}, {}};
    const Champion Maokai = {64, "Maokai", 5, 1, {25,22}, {}};
    const Champion Taric = {65, "Taric", 5, 1, {14, 36}, {}};

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

    const unordered_map<int, pair<vector<int>, vector<int>>> ALL_TRAITS = {
        {1, {{2, 3, 4}, {1,2,3}}},
        {2, {{1}, {-1}}},
        {3, {{1}, {-1}}},
        {4, {{1}, {-1}}},
        {5, {{2,4,6}, {1, 2, 3}}},
        {6, {{3,5,7,9,11}, {1,2,3,3,4}}},
        {7, {{1},{-1}}},
        {8, {{2,4,6}, {1,2,3}}},
        {9, {{1}, {-1}}},
        {10, {{3,4,5,7}, {1,2,3,3}}},
        {11, {{2,4,6}, {1,2,3}}},
        {12, {{}, {-1}}},
        {13, {{3,5,7,9,11}, {1,2,3,3,4}}},
        {14, {{1}, {-1}}},
        {15, {{2,3,4}, {1,2,3}}},
        {16, {{2,4}, {1,3}}},
        {17, {{1,2}, {1,3}}},
        {18, {{1}, {-1}}},
        {19, {{2,3,4,5}, {1,2,3,3}}},
        {20, {{2,3,5,7}, {1,2,3,3}}},
        {21, {{2,3,4,5}, {1,2,3,3}}},
        {22, {{2,4,6}, {1,2,3}}},
        {23, {{2,3,4,5}, {1,2,3,3}}},
        {24, {{1}, {-1}}},
        {25, {{1}, {-1}}},
        {26, {{2,4}, {1,2}}},
        {27, {{2,3,4,5}, {1,2,3,3}}},
        {28, {{2,4,6}, {1,2,3}}},
        {29, {{3,5,7,10}, {1,2,3,3}}},
        {30, {{1,2}, {1}}},
        {31, {{3}, {2}}},
        {32, {{2,4,6}, {1,2,3}}},
        {33, {{3,5,7}, {1,2,3}}},
        {34, {{2,3}, {1,3}}},
        {35, {{1}, {-1}}},
        {36, {{2,3,6}, {1,2,3}}},
    };

    const std::unordered_map<int, string> TRAIT_ID_TO_NAME = {
        {1, "Adaptor"}, {2, "Apex Predator"}, {3, "Attuned"}, {4, "Avatar"},
        {5, "Blackthorn"}, {6, "Blossom"}, {7, "Bounty Seeker"}, {8, "Brawler"},
        {9, "Caustic"}, {10, "Coven"}, {11, "Defender"}, {12, "Eclipse"},
        {13, "Elderwood"}, {14, "Emerald Aspect"}, {15, "Executioner"}, {16, "Fae"},
        {17, "Flora Fatalis"}, {18, "Greenfather"}, {19, "Hunter"}, {20, "Inferno"},
        {21, "Invoker"}, {22, "Juggernaut"}, {23, "Lunar"}, {24, "Monolith"},
        {25, "Old Growth"}, {26, "Primal"}, {27, "Rapidfire"}, {28, "Ravager"},
        {29, "Riftbeast"}, {30, "Rival"}, {31, "Solar"}, {32, "Spellweaver"},
        {33, "Sprykin"}, {34, "Summoner"}, {35, "Thornmaiden"}, {36, "Vanguard"}
    };

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
    const unordered_map<int, float> scales = {
                                            {Set18::Akali.id, 85.0f},
                                            {Set18::Camille.id, 100.0f},
                                            {Set18::Cinderling.id, 40.0f},
                                            {Set18::Karma.id, 100.0f},
                                            {Set18::Kobuko.id, 65.0f},
                                            {Set18::Leona.id, 100.0f},
                                            {Set18::Ornn.id, 100.0f},
                                            {Set18::Pebbles.id, 100.0f},
                                            {Set18::Rakan.id, 100.0f},
                                            {Set18::RekSai.id, 130.0f},
                                            {Set18::Varus.id, 105.0f},
                                            {Set18::Veigar.id, 80.0f},
                                            {Set18::Xayah.id, 95.0f},
                                            {Set18::Yorick.id, 110.0f},

                                            {Set18::Alistar.id, 100.0f},
                                            {Set18::Caitlyn.id, 85.0f},
                                            {Set18::Elise.id, 230.0f},
                                            {Set18::Gromp.id, 75.0f},
                                            {Set18::Kayle.id, 120.0f},
                                            {Set18::LeBlanc.id, 95.0f},
                                            {Set18::Murkwolf.id, 40.0f},
                                            {Set18::Scuttlecrab.id, 50.0f},
                                            {Set18::Sejuani.id, 100.0f},
                                            {Set18::Shen.id, 85.0f},
                                            {Set18::Teemo.id, 55.0f},
                                            {Set18::Warwick.id, 85.0f},
                                            {Set18::Yunara.id, 90.0f},

                                            {Set18::Azir.id, 160.0f},
                                            {Set18::Cassiopeia.id, 85.0f},
                                            {Set18::Diana.id, 95.0f},
                                            {Set18::Fiddlesticks.id, 200.0f},
                                            {Set18::Hecarim.id, 140.0f},
                                            {Set18::KhaZix.id, 75.0f},
                                            {Set18::KogMaw.id, 80.0f},
                                            {Set18::Krug.id, 60.0f},
                                            {Set18::MamaBeak.id, 60.0f},
                                            {Set18::MasterYi.id, 90.0f},
                                            {Set18::Rammus.id, 200.0f},
                                            {Set18::Rengar.id, 90.0f},
                                            {Set18::Tristana.id, 60.0f},
                                            {Set18::Vi.id, 95.0f},

                                            {Set18::Ahri.id, 100.0f},
                                            {Set18::Amumu.id, 75.0f},
                                            {Set18::Aphelios.id, 150.0f},
                                            {Set18::Brambleback.id, 75.0f},
                                            {Set18::Ezreal.id, 80.0f},
                                            {Set18::Lillia.id, 100.0f},
                                            {Set18::Malphite.id, 100.0f},
                                            {Set18::Morgana.id, 90.0f},
                                            {Set18::Nidalee.id, 95.0f},
                                            {Set18::Sentinel.id, 75.0f},
                                            {Set18::Sett.id, 100.0f},
                                            {Set18::Sivir.id, 80.0f},
                                            {Set18::Soraka.id, 240.0f},
                                            {Set18::Zyra.id, 87.0f},

                                            {Set18::Alune.id, 100.0f},
                                            {Set18::Ashe.id, 102.0f},
                                            {Set18::Draven.id, 85.0f},
                                            {Set18::ElderDragon.id, 140.0f},
                                            {Set18::Gnar.id, 70.0f},
                                            {Set18::Ivern.id, 105.0f},
                                            {Set18::Kennen.id, 70.0f},
                                            {Set18::Lux.id, 80.0f},
                                            {Set18::Maokai.id, 100.0f},
                                            {Set18::Taric.id, 85.0f}};

    const unordered_map<int, Vector3> hitboxScales = {
            {Set18::Akali.id, {0.25f, 1.0f, 1.0f}},
            {Set18::Camille.id, {1.0f, 1.0f, 1.0f}},
            {Set18::Cinderling.id, {1.0f, 1.0f, 1.0f}},
            {Set18::Karma.id, {1.0f, 1.0f, 1.0f}},
            {Set18::Kobuko.id, {0.8f, 1.0f, 1.0f}},
            {Set18::Leona.id, {0.3f, 0.7f, 1.0f}},
            {Set18::Ornn.id, {0.25f, 1.0f, 0.8f}},
            {Set18::Pebbles.id, {1.0f, 1.0f, 1.0f}},
            {Set18::Rakan.id, {0.25f, 0.8f, 0.6f}},
            {Set18::RekSai.id, {0.65f, 0.3f, 0.8f}},
            {Set18::Varus.id, {0.6f, 0.6f, 1.0f}},
            {Set18::Veigar.id, {0.7f, 0.7f, 0.7f}},
            {Set18::Xayah.id, {0.25f, 1.0f, 0.5f}},
            {Set18::Yorick.id, {0.55f, 1.0f, 0.4f}},

            {Set18::Alistar.id, {0.65f, 1.0f, 0.7f}},
            {Set18::Caitlyn.id, {0.6f, 1.0f, 0.6f}},
            {Set18::Elise.id, {0.25f, 0.3f, 1.0f}},
            {Set18::Gromp.id, {0.8f, 0.8f, 0.6f}},
            {Set18::Kayle.id, {0.3f, 0.7f, 1.0f}},
            {Set18::LeBlanc.id, {0.35f, 0.8f, 0.35f}},
            {Set18::Murkwolf.id, {1.0f, 1.0f, 0.9f}},
            {Set18::Scuttlecrab.id, {0.8f, 0.8f, 0.8f}},
            {Set18::Sejuani.id, {1.0f, 0.7f, 0.5f}},
            {Set18::Shen.id, {0.5f, 1.0f, 1.3f}},
            {Set18::Teemo.id, {1.0f, 1.0f, 1.0f}},
            {Set18::Warwick.id, {0.65f, 0.75f, 0.30f}},
            {Set18::Yunara.id, {0.7f, 1.0f, 0.7f}},

            {Set18::Azir.id, {0.2f, 0.6f, 0.2f}},
            {Set18::Cassiopeia.id, {0.5f, 0.7f, 0.25f}},
            {Set18::Diana.id, {0.4f, 0.8f, 0.8f}},
            {Set18::Fiddlesticks.id, {0.2f, 0.5f, 0.7f}},
            {Set18::Hecarim.id, {0.7f, 0.6f, 0.7f}},
            {Set18::KhaZix.id, {0.5f, 0.8f, 1.0f}},
            {Set18::KogMaw.id, {0.5f, 0.6f, 0.6f}},
            {Set18::Krug.id, {1.0f, 1.0f, 1.0f}},
            {Set18::MamaBeak.id, {1.0f, 1.0f, 1.0f}},
            {Set18::MasterYi.id, {0.8f, 0.8f, 1.0f}},
            {Set18::Rammus.id, {0.3f, 0.3f, 0.4f}},
            {Set18::Rengar.id, {0.7f, 0.7f, 0.5f}},
            {Set18::Tristana.id, {0.7f, 1.0f, 0.6f}},
            {Set18::Vi.id, {0.3f, 0.9f, 0.8f}},

            {Set18::Ahri.id, {0.3f, 0.7f, 0.3f}},
            {Set18::Amumu.id, {0.8f, 0.8f, 1.0f}},
            {Set18::Aphelios.id, {0.4f, 0.6f, 0.4f}},
            {Set18::Brambleback.id, {0.7f, 1.0f, 1.0f}},
            {Set18::Ezreal.id, {1.0f, 1.0f, 1.0f}},
            {Set18::Lillia.id, {0.3f, 0.6f, 0.7f}},
            {Set18::Malphite.id, {0.6f, 0.8f, 0.7f}},
            {Set18::Morgana.id, {0.4f, 0.8f, 0.8f}},
            {Set18::Nidalee.id, {0.6f, 0.8f, 1.0f}},
            {Set18::Sentinel.id, {0.7f, 1.0f, 1.0f}},
            {Set18::Sett.id, {0.7f, 0.8f, 0.3f}},
            {Set18::Sivir.id, {1.0f, 1.0f, 1.0f}},
            {Set18::Soraka.id, {0.2f, 0.4f, 0.8f}},
            {Set18::Zyra.id, {0.5f, 1.0f, 1.0f}},

            {Set18::Alune.id, {1.0f, 1.0f, 1.0f}},
            {Set18::Ashe.id, {0.4f, 0.8f, 1.0f}},
            {Set18::Draven.id, {0.3f, 1.0f, 1.0f}},
            {Set18::ElderDragon.id, {0.3f, 0.6f, 0.7f}},
            {Set18::Gnar.id, {0.6f, 0.7f, 0.5f}},
            {Set18::Ivern.id, {0.3f, 1.1f, 0.2f}},
            {Set18::Kennen.id, {0.4f, 1.0f, 1.0f}},
            {Set18::Lux.id, {0.5f, 1.0f, 1.0f}},
            {Set18::Maokai.id, {0.4f, 1.0f, 0.7f}},
            {Set18::Taric.id, {0.5f, 1.0f, 0.9f}}
        };

    const unordered_map<int, Vector3> hitboxOffsets = {
        {Set18::Akali.id, {0.32f, 0.0f, 0.0f}},
        {Set18::Camille.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Cinderling.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Karma.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Kobuko.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Leona.id, {-0.3f, 0.0f, 0.0f}},
        {Set18::Ornn.id, {0.15f, 0.0f, 0.0f}},
        {Set18::Pebbles.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Rakan.id, {0.1f, 0.0f, 0.0f}},
        {Set18::RekSai.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Varus.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Veigar.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Xayah.id, {-0.1f, 0.0f, 0.1f}},
        {Set18::Yorick.id, {0.0f, 0.0f, 0.3f}},

        {Set18::Alistar.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Caitlyn.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Elise.id, {0.3f, 0.0f, 0.0f}},
        {Set18::Gromp.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Kayle.id, {0.0f, 0.2f, 0.2f}},
        {Set18::LeBlanc.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Murkwolf.id, {0.0f, 0.0f, 0.1f}},
        {Set18::Scuttlecrab.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Sejuani.id, {0.0f, 0.0f, 0.2f}},
        {Set18::Shen.id, {0.2f, 0.0f, 0.2f}},
        {Set18::Teemo.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Warwick.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Yunara.id, {0.0f, 0.0f, 0.0f}},

        {Set18::Azir.id, {-0.05f, 0.0f, 0.3f}},
        {Set18::Cassiopeia.id, {0.0f, 0.0f, 0.1f}},
        {Set18::Diana.id, {-0.1f, 0.0f, 0.1f}},
        {Set18::Fiddlesticks.id, {0.1f, 0.0f, 0.0f}},
        {Set18::Hecarim.id, {0.0f, 0.0f, 0.1f}},
        {Set18::KhaZix.id, {0.0f, 0.0f, 0.0f}},
        {Set18::KogMaw.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Krug.id, {0.0f, 0.0f, 0.0f}},
        {Set18::MamaBeak.id, {0.0f, 0.0f, 0.0f}},
        {Set18::MasterYi.id, {0.0f, 0.0f, 0.1f}},
        {Set18::Rammus.id, {-0.1f, 0.0f, 0.0f}},
        {Set18::Rengar.id, {0.0f, 0.0f, 0.1f}},
        {Set18::Tristana.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Vi.id, {0.0f, 0.0f, 0.0f}},

        {Set18::Ahri.id, {0.0f, 0.0f, 0.15f}},
        {Set18::Amumu.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Aphelios.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Brambleback.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Ezreal.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Lillia.id, {-0.05f, 0.0f, 0.0f}},
        {Set18::Malphite.id, {0.0f, 0.0f, 0.1f}},
        {Set18::Morgana.id, {0.0f, 0.0f, 0.1f}},
        {Set18::Nidalee.id, {-0.15f, 0.0f, 0.1f}},
        {Set18::Sentinel.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Sett.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Sivir.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Soraka.id, {0.35f, 0.0f, 0.0f}},
        {Set18::Zyra.id, {0.0f, 0.0f, 0.0f}},

        {Set18::Alune.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Ashe.id, {-0.3f, 0.0f, 0.0f}},
        {Set18::Draven.id, {0.0f, 0.0f, 0.2f}},
        {Set18::ElderDragon.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Gnar.id, {-0.25f, 0.0f, 0.0f}},
        {Set18::Ivern.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Kennen.id, {0.0f, 0.0f, 0.0f}},
        {Set18::Lux.id, {0.1f, 0.0f, 0.0f}},
        {Set18::Maokai.id, {0.1f, 0.0f, 0.0f}},
        {Set18::Taric.id, {-0.15f, 0.0f, 0.0f}}
    };

    const unordered_map<int, float> groundOffsets = {
        {Set18::Akali.id, 0.0f},
        {Set18::Camille.id, 0.0f},
        {Set18::Cinderling.id, 0.0f},
        {Set18::Karma.id, 0.0f},
        {Set18::Kobuko.id, 0.0f},
        {Set18::Leona.id, 0.0f},
        {Set18::Ornn.id, 0.0f},
        {Set18::Pebbles.id, 0.0f},
        {Set18::Rakan.id, 0.0f},
        {Set18::RekSai.id, 0.0f},
        {Set18::Varus.id, 0.0f},
        {Set18::Veigar.id, 0.0f},
        {Set18::Xayah.id, 0.0f},
        {Set18::Yorick.id, 0.0f},

        {Set18::Alistar.id, 0.0f},
        {Set18::Caitlyn.id, 0.0f},
        {Set18::Elise.id, 0.0f},
        {Set18::Gromp.id, 0.0f},
        {Set18::Kayle.id, 0.0f},
        {Set18::LeBlanc.id, 0.0f},
        {Set18::Murkwolf.id, 0.0f},
        {Set18::Scuttlecrab.id, 0.0f},
        {Set18::Sejuani.id, 0.0f},
        {Set18::Shen.id, 0.0f},
        {Set18::Teemo.id, 0.0f},
        {Set18::Warwick.id, 0.0f},
        {Set18::Yunara.id, 0.0f},

        {Set18::Azir.id, 0.0f},
        {Set18::Cassiopeia.id, 0.0f},
        {Set18::Diana.id, 0.0f},
        {Set18::Fiddlesticks.id, -0.2f},
        {Set18::Hecarim.id, 0.0f},
        {Set18::KhaZix.id, 0.0f},
        {Set18::KogMaw.id, 0.0f},
        {Set18::Krug.id, 0.0f},
        {Set18::MamaBeak.id, 0.0f},
        {Set18::MasterYi.id, 0.0f},
        {Set18::Rammus.id, 0.0f},
        {Set18::Rengar.id, 0.0f},
        {Set18::Tristana.id, 0.0f},
        {Set18::Vi.id, 0.0f},

        {Set18::Ahri.id, 0.0f},
        {Set18::Amumu.id, 0.0f},
        {Set18::Aphelios.id, -0.5f},
        {Set18::Brambleback.id, 0.0f},
        {Set18::Ezreal.id, 0.0f},
        {Set18::Lillia.id, 0.0f},
        {Set18::Malphite.id, 0.0f},
        {Set18::Morgana.id, 0.0f},
        {Set18::Nidalee.id, 0.0f},
        {Set18::Sentinel.id, 0.0f},
        {Set18::Sett.id, 0.0f},
        {Set18::Sivir.id, 0.0f},
        {Set18::Soraka.id, 0.0f},
        {Set18::Zyra.id, 0.0f},

        {Set18::Alune.id, 0.0f},
        {Set18::Ashe.id, 0.0f},
        {Set18::Draven.id, 0.0f},
        {Set18::ElderDragon.id, -0.3f},
        {Set18::Gnar.id, 0.0f},
        {Set18::Ivern.id, 0.0f},
        {Set18::Kennen.id, 0.0f},
        {Set18::Lux.id, 0.0f},
        {Set18::Maokai.id, 0.0f},
        {Set18::Taric.id, 0.0f}
    };
}

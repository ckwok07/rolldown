#pragma once
#include "Champion.h"
#include <string>
#include <unordered_map>
using namespace std;

const Champion AATROX = {1, "Aatrox", 1, 1, {2, 7}, {}};
const Champion Briar = {2, "Briar", 1, 1, {2, 7}, {}};
const Champion ChoGath = {3, "Cho'Gath", 1, 1, {2, 7}, {}};
const Champion Ezreal = {4, "Ezreal", 1, 1, {2, 7}, {}};
const Champion Leona = {5, "Leona", 1, 1, {2, 7}, {}};
const Champion Lissandra = {6, "Lissandra", 1, 1, {2, 7}, {}};
const Champion Nasus = {7, "Nasus", 1, 1, {2, 7}, {}};
const Champion Poppy = {8, "Poppy", 1, 1, {2, 7}, {}};
const Champion RekSai = {9, "Rek'Sai", 1, 1, {2, 7}, {}};
const Champion Talon = {10, "Talon", 1, 1, {2, 7}, {}};
const Champion Teemo = {11, "Teemo", 1, 1, {2, 7}, {}};
const Champion TF = {12, "Twisted Fate", 1, 1, {2, 7}, {}};
const Champion Veigar = {13, "Veigar", 1, 1, {2, 7}, {}};

const Champion Akali = {14, "Akali", 2, 1, {2, 7}, {}};
const Champion Belveth = {15, "Bel'veth", 2, 1, {2, 7}, {}};
const Champion Gnar = {16, "Gnar", 2, 1, {2, 7}, {}};
const Champion Gragas = {17, "Gragas", 2, 1, {2, 7}, {}};
const Champion Gwen = {18, "Gwen", 2, 1, {2, 7}, {}};
const Champion Jax = {19, "Jax", 2, 1, {2, 7}, {}};
const Champion Jinx = {20, "Jinx", 2, 1, {2, 7}, {}};
const Champion Meepsie = {21, "Meepsie", 2, 1, {2, 7}, {}};
const Champion Milio = {22, "Milio", 2, 1, {2, 7}, {}};
const Champion Mordekaiser = {23, "Mordekaiser", 2, 1, {2, 7}, {}};
const Champion Pantheon = {24, "Pantheon", 2, 1, {2, 7}, {}};
const Champion Pyke = {25, "Pyke", 2, 1, {2, 7}, {}};
const Champion Zoe = {26, "Zoe", 2, 1, {2, 7}, {}};

const Champion Aurora = {27, "Akali", 3, 1, {2, 7}, {}};
const Champion Diana = {28, "Diana", 3, 1, {2, 7}, {}};
const Champion Fizz = {29, "Fizz", 3, 1, {2, 7}, {}};
const Champion Illaoi = {30, "Illaoi", 3, 1, {2, 7}, {}};
const Champion Kaisa = {31, "Kaisa", 3, 1, {2, 7}, {}};
const Champion Lulu = {32, "Lulu", 3, 1, {2, 7}, {}};
const Champion Maokai = {33, "Maokai", 3, 1, {2, 7}, {}};
const Champion MF = {34, "Miss Fortune", 3, 1, {2, 7}, {}};
const Champion Ornn = {35, "Ornn", 3, 1, {2, 7}, {}};
const Champion Rhaast = {36, "Rhaast", 3, 1, {2, 7}, {}};
const Champion Samira = {37, "Samira", 3, 1, {2, 7}, {}};
const Champion Urgot = {38, "Urgot", 3, 1, {2, 7}, {}};
const Champion Viktor = {39, "Viktor", 3, 1, {2, 7}, {}};

const Champion Asol = {40, "Aurelion Sol", 4, 1, {2, 7}, {}};
const Champion Corki = {41, "Corki", 4, 1, {2, 7}, {}};
const Champion Karma = {42, "Karma", 4, 1, {2, 7}, {}};
const Champion Kindred = {43, "Kinded", 4, 1, {2, 7}, {}};
const Champion Leblanc = {44, "Leblanc", 4, 1, {2, 7}, {}};
const Champion Yi = {45, "Master Yi", 4, 1, {2, 7}, {}};
const Champion Nami = {46, "Nami", 4, 1, {2, 7}, {}};
const Champion Nunu = {47, "Nunu", 4, 1, {2, 7}, {}};
const Champion Rammus = {48, "Rammus", 4, 1, {2, 7}, {}};
const Champion Riven = {49, "Riven", 4, 1, {2, 7}, {}};
const Champion TK = {50, "Tahm Kench", 4, 1, {2, 7}, {}};
const Champion Mech = {51, "The Mighty Mech", 4, 1, {2, 7}, {}};
const Champion Xayah = {52, "Xayah", 4, 1, {2, 7}, {}};

const Champion Bard = {53, "Bard", 5, 1, {2, 7}, {}};
const Champion Blitz = {54, "Blitzcrank", 5, 1, {2, 7}, {}};
const Champion Fiora = {55, "Fiora", 5, 1, {2, 7}, {}};
const Champion Graves = {56, "Graves", 5, 1, {2, 7}, {}};
const Champion Jhin = {57, "Jhin", 5, 1, {2, 7}, {}};
const Champion Morgana = {58, "Morgana", 5, 1, {2, 7}, {}};
const Champion Shen = {59, "Shen", 5, 1, {2, 7}, {}};
const Champion Sona = {60, "Sona", 5, 1, {2, 7}, {}};
const Champion Vex = {61, "Vex", 5, 1, {2, 7}, {}};
const Champion Zed = {62, "Zed", 5, 1, {2, 7}, {}};

// 1 Anima
// 2 Arbiter
// 3 Bulwark
// 4 Commander
// 5 Dark Lady
// 6 Dark Star 
// 7 Divine Duelist 
// 8 Doomer
// 9 Factory New 
// 10 Galaxy Hunter 
// 11 Mecha 
// 12 Meeple
// 13 N.O.V.A 
// 14 Oracle 
// 15 Party Animal 
// 16 Primordian 
// 17 Psionic
// 18 Redeemer
// 19 Space Groove
// 20 Stargazer 
// 21 Timebreaker 
// 22 Bastion 
// 23 Brawler 
// 24 Challenger
// 25 Conduit 
// 26 Eradicator
// 27 Fateweaver 
// 28 Marauder
// 29 Replicator 
// 30 Rogue 
// 31 Sepherd 
// 32 Sniper 
// 33 Vangaurd 
// 34 Voyager 
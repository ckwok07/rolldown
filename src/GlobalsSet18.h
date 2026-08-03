#pragma once
#include "Champion.h"
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <set>
using namespace std;

extern const vector<vector<int>> shopodds;
extern const vector<int> levelthresholds;

extern const Champion nullChamp;

extern const unordered_map<int, string> itemComponents;
extern const map<pair<int,int>, string> completedItems;

extern const Champion Akali;
extern const Champion Camille;
extern const Champion Cinderling;
extern const Champion Karma;
extern const Champion Kobuko;
extern const Champion Leona;
extern const Champion Ornn;
extern const Champion Pebbles;
extern const Champion Rakan;
extern const Champion RekSai;
extern const Champion Varus;
extern const Champion Veigar;
extern const Champion Xayah;
extern const Champion Yorick;

extern const Champion Alistar;
extern const Champion Caitlyn;
extern const Champion Elise;
extern const Champion Gromp;
extern const Champion Kayle;
extern const Champion LeBlanc;
extern const Champion Murkwolf;
extern const Champion Scuttlecrab;
extern const Champion Sejuani;
extern const Champion Shen;
extern const Champion Teemo;
extern const Champion Warwick;
extern const Champion Yunara;

extern const Champion Azir;
extern const Champion Cassiopeia;
extern const Champion Diana;
extern const Champion Fiddlesticks;
extern const Champion Hecarim;
extern const Champion KhaZix;
extern const Champion KogMaw;
extern const Champion Krug;
extern const Champion MamaBeak;
extern const Champion MasterYi;
extern const Champion Rammus;
extern const Champion Rengar;
extern const Champion Tristana;
extern const Champion Vi;

extern const Champion Ahri;
extern const Champion Amumu;
extern const Champion Aphelios;
extern const Champion Brambleback;
extern const Champion Ezreal;
extern const Champion Lillia;
extern const Champion Malphite;
extern const Champion Morgana;
extern const Champion Nidalee;
extern const Champion Sentinal;
extern const Champion Sett;
extern const Champion Sivir;
extern const Champion Soraka;
extern const Champion Zyra;

extern const Champion Alune;
extern const Champion Ashe;
extern const Champion Draven;
extern const Champion ElderDragon;
extern const Champion Gnar;
extern const Champion Ivern;
extern const Champion Kennen;
extern const Champion Lux;
extern const Champion Maokai;
extern const Champion Taric;

extern const vector<Champion> ALL_CHAMPIONS;

// 1 Adaptor
// 2 Apex Predator
// 3 Attuned
// 4 Avatar
// 5 Blackthorn
// 6 Blossom
// 7 Bounty Seeker 
// 8 Brawler
// 9 Caustic 
// 10 Coven 
// 11 Defender 
// 12 Eclipse
// 13 Elderwood 
// 14 Emerald Aspect
// 15 Executioner
// 16 Fae
// 17 Flora Fatalis
// 18 Greenfather
// 19 Hunter
// 20 Inferno 
// 21 Invoker 
// 22 Juggernaut 
// 23 Lunar
// 24 Monolith
// 25 Old Growth
// 26 Primal
// 27 Rapidfire 
// 28 Ravager
// 29 Riftbeast 
// 30 Rival
// 31 Solar 
// 32 Spellweaver 
// 33 Sprykin
// 34 Summoner 
// 35 Thornmaiden
// 36 Vanguard

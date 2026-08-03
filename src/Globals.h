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
namespace Set17 {
  extern const Champion Aatrox;
  extern const Champion Briar;
  extern const Champion Caitlyn;
  extern const Champion ChoGath;
  extern const Champion Ezreal;
  extern const Champion Leona;
  extern const Champion Lissandra;
  extern const Champion Nasus;
  extern const Champion Poppy;
  extern const Champion RekSai;
  extern const Champion Talon;
  extern const Champion Teemo;
  extern const Champion TF;
  extern const Champion Veigar;
  
  extern const Champion Akali;
  extern const Champion Belveth;
  extern const Champion Gnar;
  extern const Champion Gragas;
  extern const Champion Gwen;
  extern const Champion Jax;
  extern const Champion Jinx;
  extern const Champion Meepsie;
  extern const Champion Milio;
  extern const Champion Mordekaiser;
  extern const Champion Pantheon;
  extern const Champion Pyke;
  extern const Champion Zoe;
  
  extern const Champion Aurora;
  extern const Champion Diana;
  extern const Champion Fizz;
  extern const Champion Illaoi;
  extern const Champion Kaisa;
  extern const Champion Lulu;
  extern const Champion Maokai;
  extern const Champion MF;
  extern const Champion Ornn;
  extern const Champion Rhaast;
  extern const Champion Samira;
  extern const Champion Urgot;
  extern const Champion Viktor;
  
  extern const Champion Asol;
  extern const Champion Corki;
  extern const Champion Karma;
  extern const Champion Kindred;
  extern const Champion Leblanc;
  extern const Champion Yi;
  extern const Champion Nami;
  extern const Champion Nunu;
  extern const Champion Rammus;
  extern const Champion Riven;
  extern const Champion TK;
  extern const Champion Mech;
  extern const Champion Xayah;
  
  extern const Champion Bard;
  extern const Champion Blitz;
  extern const Champion Fiora;
  extern const Champion Graves;
  extern const Champion Jhin;
  extern const Champion Morgana;
  extern const Champion Shen;
  extern const Champion Sona;
  extern const Champion Vex;
  extern const Champion Zed;
  
  extern const vector<Champion> ALL_CHAMPIONS;
  
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
  // 35 Gun Goddess
  // 36 Gun Goddess -1
}

namespace Set18 {
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
}

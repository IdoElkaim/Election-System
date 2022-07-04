#pragma once
#include "global.h"
#define PARTY 1
#define DISTRICT 2

using namespace std;
using namespace elections;
void PrintMainMenu();
enum class Options {
	addDistrict=1, addCitizen, addParty, addRepresentor, printDistricts,
	printCitizens, printParties, vote, showResult, saveRound, loadRound, exit
};

enum class MainOpt {
	newRound = 1, loadRound, exit
};

void addDistrict(electionsDay* eDay);
void addCitizen(electionsDay* eDay);
void addParty(electionsDay* eDay);
void addCitizenToParty(electionsDay* eDay);
void Vote(electionsDay* eDay);
void MainMenu(electionsDay* eDay);
bool Check(int day, int month, int year);
void read(const char* filename);
void write(const char* filename);
void newRound();
void loadRound(const char* filename);


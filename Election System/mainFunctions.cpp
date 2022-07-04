#pragma once
#include "global.h"

void PrintMainMenu() {
	cout << "-----Main Menu-----" << endl
		<< "Please enter a number between 1-10 to perform the following:" << endl
		<< "1. Add a District" << endl << "2. Add a Citizen" << endl
		<< "3. Add a Party" << endl << "4. Add a party member" << endl
		<< "5. Show Districts list" << endl << "6. Show Citizen list" << endl
		<< "7. Show Parties list" << endl << "8. Vote" << endl
		<< "9. Show elections results" << endl
		<< "10. Save" << endl
		<< "11. Load" << endl
		<< "12. Exit" << endl;
}

bool Check(int day, int month, int year) {
	if (year < 1) throw "Date not valid !";
	if (day < 1 || day >31) throw "Date not valid !";
	if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
		throw "Date not valid !";
	if (month == 2 && day > 28)
		throw "Date not valid !";

	if (month < 1 || month >12) 	throw "Date not valid !";
}

void addDistrict(electionsDay* eDay)
{
	SimpleEDay* check = new SimpleEDay();
	if (typeid(*check).name() == typeid(*eDay).name()) {
		cout << "Sorry moshe, this is a simple elections day !" << endl;
		delete check;
		return;
	}

	string name;
	int numOfElectors, type;
	cout << "Enter the name of the district" << endl;
	cin >> name;
	cout << "Enter the number of electors in the district" << endl;
	cin >> numOfElectors;
	cout << "Enter district type (1 = special, 0 = simple)" << endl;
	cin >> type;
    if (numOfElectors < 1)
		throw  "Number of electors not valid. ";
	eDay->addDistrict(name, numOfElectors, type);
}

void addCitizen(electionsDay* eDay)
{
	string name;
	char* id = new char[10];
	int yob, disNum;
	cout << "Enter citizen name:" << endl;
	cin >> name;
	cout << "Enter citizen ID:" << endl;
	cin >> id;
	if (strlen(id) != 9)
		throw "ID not valid.";
	
	cout << "Enter citizen year of birth:" << endl;
	cin >> yob;
	if (eDay->getYear() - yob < 18)
		throw "Citizen is minor.";

	SimpleEDay* check = new SimpleEDay();
	if (typeid(*check).name() == typeid(*eDay).name()) {
		eDay->addCitizen(name, id, yob, -1);
	}
	else
	{
		cout << "Enter citizen district:" << endl;
		cin >> disNum;
		disNum = eDay->checkValidParDis(disNum, DISTRICT);
		eDay->addCitizen(name, id, yob, disNum);
	}

}

void addParty(electionsDay* eDay)
{
	char id[10];
	string name;
	cout << "Enter party name:" << endl;
	cin >> name;
	cout << "Enter candidate ID:" << endl;
	cin >> id;
	if (strlen(id) != 9)
		throw "ID not valid.";
	const char* validIDp = eDay->checkValidID(id);

	eDay->addParty(name, validIDp); 
	SimpleEDay* check = new SimpleEDay();
	if (typeid(*check).name() == typeid(*eDay).name())
	{
		district* dis = eDay->findDistrict(1);
		party* par = eDay->findParty(eDay->getPartyCount());
		citizen* cit = eDay->findCitizen(id);
		dis->addRep(cit, par);
	}
		
}

void addCitizenToParty(electionsDay* eDay)
{
	citizen* cit;
	district* dis;
	char id[10];
	int partyNum, disNum;
	cout << "Enter citizen ID: " << endl;
	cin >> id;
	if (strlen(id) != 9)
		throw "ID not valid.";
	const char* validIDp = eDay->checkValidID(id);

	SimpleEDay* check = new SimpleEDay();
	if (typeid(*check).name() == typeid(*eDay).name()) {
		cout << "Enter party number: " << endl;
		cin >> partyNum;
		partyNum = eDay->checkValidParDis(partyNum, PARTY);
		cit = eDay->findCitizen(validIDp);
		dis = eDay->findDistrict(1);
		dis->addRep(cit, eDay->findParty(partyNum));
	}
	else
	{
		cout << "Enter district number: " << endl;
		cin >> disNum;

		disNum = eDay->checkValidParDis(disNum, DISTRICT);

		cout << "Enter party number: " << endl;
		cin >> partyNum;
		partyNum = eDay->checkValidParDis(partyNum, PARTY);

		cit = eDay->findCitizen(validIDp);
		dis = eDay->findDistrict(disNum);
		dis->addRep(cit, eDay->findParty(partyNum));
	}
}

void MainMenu(electionsDay* eDay)
{
	int choice;
	char filename[128];
	ofstream out;
	ifstream in;
	Options c=static_cast<Options>(1);
	while (c != Options::exit) {
		try
		{
			PrintMainMenu();
			cin >> choice;
			c = static_cast<Options>(choice);

			switch (c) {
			case Options::addDistrict:					//1
				addDistrict(eDay);
				break;

			case Options::addCitizen:					//2
				if (eDay->DisListIsEmpty())
					throw "Moshe I'm sorry, there are no districts.";
				else
					addCitizen(eDay);
				break;

			case Options::addParty:						//3
				if (eDay->CitListIsEmpty())
					throw "Add citizen first";
				else
					addParty(eDay);
				break;

			case Options::addRepresentor:				//4
				if (eDay->ParListIsEmpty())
					throw "Moshe I'm sorry, there are no parties.";
				else
					addCitizenToParty(eDay);
				break;

			case Options::printDistricts:				//5
				eDay->printDistrictsList();
				break;

			case Options::printCitizens:				//6
				eDay->printCitizenList();
				break;

			case Options::printParties:					//7
				eDay->printPartiesList();
				break;

			case Options::vote:							//8
				if (eDay->getPartyCount() < 1)
					throw "Moshe I'm sorry, there are no parties.";
				Vote(eDay);
				break;

			case Options::showResult:					//9
				if (!eDay->didPeopleVote())
					throw "People did not vote in all the districts";
				else if (!eDay->enoughRep())
					throw "There are not enough representors in one or more district";
				else
					eDay->calcResults();
				break;

			case Options::saveRound:					//10
				cout << "Enter file name: " << endl;
				cin >> filename;
				out.open(filename, ios::binary | ios::trunc);
				eDay->Save(out);
				out.close();
				cout << "Elections round saved successfully !" << endl << endl;
				break;

			case Options::loadRound:					//11
				cout << "Enter file name: " << endl;
				cin >> filename;
				loadRound(filename);
				break;

			case Options::exit:							//12
				exit(-1);
			}
		}
		catch (bad_alloc& er) {
			cout << er.what() << endl;
		}
		catch (const char* msg) {
			cout << "ERROR:" << msg << endl <<endl;
		}
	}
}

void Vote(electionsDay* eDay)
{
	char id[10];
	int partyNum;
	cout << "Enter citizen ID:" << endl;
	cin >> id;
	if (strlen(id) != 9)
		throw "ID not valid.";
	const char* validIDp = eDay->checkValidID(id);
	cout << "Enter voting party number:" << endl;
	cin >> partyNum;
	partyNum = eDay->checkValidParDis(partyNum, PARTY);
	party* par = eDay->findParty(partyNum);
	eDay->vote(id, par);
	}

void newRound() {
	int day = -1, month = -1, year = -1;
	cout << "Enter round type (0 = regular, 1 = simple): " << endl;
	int type;
	cin >> type;

	cout << "Enter date for elections day (dd mm yyyy): " << endl;
	cin >> day >> month >> year;
	Check(day, month, year);
	
	if (type == 0){
		electionsDay eDay(day, month, year);
		MainMenu(&eDay);
	}
	else
	{
		cout << "Enter electors number: " << endl;
		int electors;
		cin >> electors;
		if (electors < 1)
			throw "Electors number is not valid !";
		SimpleEDay eDay(day, month, year, electors);
		MainMenu(&eDay);
	}

}

void loadRound(const char* filename) {
	int type;
	SimpleEDay* seDay;
	electionsDay* eDay;
	ifstream in(filename, ios::binary);
	if (!in.is_open())
	{
		throw "No such file.";
	}
	in.read(rcastc(&type), sizeof(type));
	if (type) {
		seDay = new SimpleEDay(in);
		seDay->Load(in);
		in.close();
		MainMenu(seDay);
	}
	else {
		eDay = new electionsDay(in);
		eDay->Load(in);
		in.close();
		MainMenu(eDay);
	}
}
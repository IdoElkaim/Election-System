#include "global.h"
#include "mainFunctions.h"
using namespace std;
using namespace elections;

int main()
{
	char filename[128];
	cout << "Menu: " << endl;
	cout << "1.Create new round of elections" << endl
		<< "2.Load an existing round of elections" << endl
		<< "3.Exit" << endl;
	electionsDay eDay;
	int  choice;
	cin >> choice;
	MainOpt opt = static_cast<MainOpt>(choice);
	try
	{
		switch (opt) {
		case MainOpt::newRound:
			newRound();
			break;
		case MainOpt::loadRound:
			cout << "Enter file name:" << endl;
			cin >> filename;
			loadRound(filename);
			break;
		case MainOpt::exit:
			exit(-1);
			break;
		}
	}
	catch (bad_alloc& er) {
		cout << er.what() << endl;
	}
	catch (const char* msg) {
		cout << "ERROR: " << msg << endl << endl;
	}
}
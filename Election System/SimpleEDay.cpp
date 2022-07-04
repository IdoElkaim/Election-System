#include "global.h"

namespace elections {
	SimpleEDay::SimpleEDay() {
		_day = -1;
		_month = -1;
		_year = -1;
		_citList.clear();
		_parList.clear();
		_disList.clear();
	}

	SimpleEDay::SimpleEDay(int day, int month, int year, int electors) : 
		electionsDay(day, month,year) {
		
		district* dis = new specialDistrict("General", electors);
		_disList.push_back(dis);
	}
	
	void SimpleEDay::printPartiesList()
	{
		for (party* i : _parList)
			cout << *i << endl;
		cout << endl;
	}

	bool SimpleEDay::addCitizen( const char* name,  const char* id, int yob, int disNum) {
		
		citizen* newCit = new citizen(name, id, yob, this->_disList.front());
		_citList.push_back(newCit);
		_disList.front()->addVoter(newCit);
		return true;
	}

	void SimpleEDay::calcResults() {
		district* dis = _disList.front();
		dis->sortVotes();
		dis->setPercent(static_cast<float>(static_cast<float>(dis->_numVotes)
			/ static_cast<float>(dis->_votersList.size())));
		cout << "Percentage of voting: " << dis->_percentage << endl;
		int i = 0;
		float per;
		representors* repList;
		while (dis->_votes[i].getElec() > 0)
		{
			per = static_cast<float>(dis->_votes[i].getVotes()) / static_cast<float>(dis->_numVotes);
			cout << "Party name: " << dis->_votes[i].getParty()->getName()
				<< " won " << dis->_votes[i].getElec() << " electors, with "
				<< dis->_votes[i].getVotes() << " votes, which is " << per*100 << " percent of the votes." << endl;
			cout << "Party's candidate for presidency is: " << dis->_votes[i].getParty()->getPrimeName()<<"." << endl;
			cout << "Party's list of representors: " << endl;

			repList = dis->_repListHead;
			while (repList->getParty() != dis->_votes[i].getParty())
				repList = repList->nextParty;
			for (int j = 0; j < dis->_votes[i].getElec()-1; j++)
			{
				cout << repList->nextRep->getRep()->getName() << endl;
				repList = repList->nextRep;
			}
			i++;
		}
	}
}
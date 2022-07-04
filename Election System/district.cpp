#include "global.h"

namespace elections {
	int district::counter = 0;

	district::district(): 
		_name(""), _serial(0), _electors(0),
		_numVotes(0), _percentage(0), _votersList(), 
		_repListHead(nullptr), _repListTail(nullptr), _votes(){
	}

	district::district(const string name, int electors) : // district ctor
		_name(name),
		_electors(electors),
		_serial(++counter),
		_numVotes(0), _percentage(0), _votes(), _votersList()
	{
		_repListHead = nullptr;
		_repListTail = nullptr;
	}

	district::~district() // district dtor
	{		
		representors* tempParty = _repListHead;
		representors* tempRep;
		representors* tempRep2;

		while (_repListHead != nullptr)
		{
			tempRep = _repListHead->nextRep;
			while (tempRep != nullptr)
			{
				tempRep2 = tempRep;
				tempRep = tempRep->nextRep;
				delete tempRep2;
			}
			tempParty = _repListHead;
			_repListHead = _repListHead->nextParty;
			delete tempParty;	
		}
	}


	bool district::addRep(citizen* mem, party* par) // adds represntor to a party
	{
		representors* newRep = new representors(mem, par);
		if (_repListHead == nullptr)
		{
			newRep->nextParty = nullptr;
			newRep->nextRep = nullptr;
			_repListTail = newRep;
			_repListHead = newRep;
			votes tempVote(par);
			_votes.push_back(tempVote);
			par->addMember(mem, this->getSerial());
		}
		else
		{
			representors* temp = _repListHead;
			while (temp->_par != par && temp->nextParty!=nullptr)
				temp = temp->nextParty;
			if (temp->_par == par)
			{
				while(temp->nextRep!=nullptr)
					temp = temp->nextRep;
				temp->nextRep = newRep;
			}
			else 
			{
				votes tempVote(par);
				_votes.push_back(tempVote);
				temp->nextParty = newRep;
				_repListTail = newRep;
			}
			par->addMember(mem, this->getSerial());
		}
		return true;

	}

	bool district::addVoter(citizen* voter) // adds voter to the district
	{
		_votersList.push_back(voter);
		return true;
	}

	bool district::addVote(party* par) // adds vote of a citizen
	{
		DynamicArray<votes>::iterator itr = _votes.begin();
		DynamicArray<votes>::iterator end = _votes.end();

		for(;itr!=end ; ++itr)
		{
			if (itr->_par == par)
				itr->addVote();
		}
		_numVotes++;
		return true;
	}
	 
	void district::print(std::ostream& os) const {  // necessary to apply << operator
		os << "District Number: " << this->getSerial() << ", Name: " << this->getName() <<
			" Type: Regular" << ", Number Of Representatives: " << this->getElectors() << endl;;
	}
	std::ostream& operator<<(std::ostream& os, const district& dis) // << operator
	{
		dis.print(os);
		return os;
	}

	int district::getPartyVotes(party* par) {
		DynamicArray<votes>::iterator itr = _votes.begin();
		DynamicArray<votes>::iterator end = _votes.end();

		for (; itr != end; ++itr)
		{
			if (itr->_par == par)
				return itr->_numVotes;
		}
		return -1;
	}

	party* district::getWinnerParty()
	{
		int max = 0;
		party* par;
		DynamicArray<votes>::iterator itr = _votes.begin();
		DynamicArray<votes>::iterator end = _votes.end();
			for (; itr!= end; ++itr)
			{
				if (itr->_numVotes > max)
				{
					max = itr->_numVotes;
					par = itr->_par;
					return par;
				}
			}
	}

	void district::printWinner() { // prints the winner party.
		std::cout << "Head of district: " << _votes[0]._par->getPrimeName() << std::endl;
	}

	void district::printData() {
		std::cout << "District Number: " << this->_serial << std::endl;
		std::cout << "Number of electors: " << this->_electors << std::endl;
		std::cout << "Percentage of votes: " << _percentage * 100 << std::endl;
	}
	void district::calcResults(results& res) { // calculate the result of the district
		specialDistrict* check = new specialDistrict(" ", 0);
		check->reduceCounter();
		setPercent(static_cast<float>(static_cast<float>(_numVotes) 
					/ static_cast<float>( _votes.size())));

		int numOfElectors;
		representors* tempParty = _repListHead;
		representors* tempRep;

		printData();
		sortVotes();
		printWinner();

		while (tempParty!= nullptr)
		{
			float v = getPartyVotes(tempParty->_par);
			float partyPercent = (v / _numVotes);

			std::cout << "Party number: " << tempParty->_par->getSerial()
				<< " with "<< partyPercent*100 << " percents of the votes," 
				<< " Electors: " ;

			tempRep = tempParty;
			numOfElectors = getElectorsByParty(tempParty->_par);
			if (numOfElectors == 0)
				std::cout << endl <<"There are no represntors for this party";
			else
				std::cout << numOfElectors << std::endl;

			for (int i = 0; i < numOfElectors; i++)
			{
				std::cout << tempRep->_rep->getName() ;
				if (i > 0)
					std::cout << ", ";
				tempRep = tempRep->nextRep;
			}
			if (typeid(*this).name() == typeid(*check).name())
				res.updateElec(numOfElectors, tempParty->_par);
			std::cout << "." << std::endl;
			res.updateVotes(this->getPartyVotes(tempParty->_par), tempParty->_par);
			tempParty = tempParty->nextParty;
		}
		if (typeid(*this).name() != typeid(*check).name())
			res.updateElec(this->getElectors(), _votes[0]._par);
		delete check;
	}

	const int district::getCount() {
		return this->counter;
	}

	void district::calcElectors()  // calculate the electors won by the parties.
	{
		int max;
		float votesPerElec = static_cast<float>(_numVotes) /static_cast<float>( _electors);
		int electorsLeft = _electors;
		DynamicArray<votes>::iterator itr = _votes.begin();
		DynamicArray<votes>::iterator end = _votes.end();
		for (;itr!=end; ++itr)
		{
			if (electorsLeft > 0)
			{
				itr->totalElec = itr->_numVotes / votesPerElec;
				itr->_modVotes = itr->_numVotes - votesPerElec * itr->totalElec;
				electorsLeft -= itr->totalElec;
			}
		}
		while (electorsLeft > 0)
		{
			max = this->getMaxMod();
			_votes[max].totalElec++;
			_votes[max]._modVotes = 0;
			electorsLeft--;
		}
	}

	bool district::enoughRepresentors() //checks if there are enough representors compare to the electors won
	{
		int elec;
		representors* tempParty = _repListHead, *tempRep;
		while (tempParty != nullptr)
		{
			elec = getElectorsByParty(tempParty->_par);
			tempRep = tempParty;
			for (int j = 0;j < elec;j++)
			{
				if (tempRep == nullptr)
					return false;
				tempRep = tempRep->nextRep;
			}
			tempParty = tempParty->nextParty;
		}
		return true;
	}


	const int district::getElectorsByParty(party* par)
	{
		DynamicArray<votes>::iterator itr = _votes.begin();
		DynamicArray<votes>::iterator end = _votes.end();
		for (;itr!=end; ++itr)
		{
			if (itr->_par == par)
				return itr->totalElec;
		}
	}

	const int district::getMaxMod()
	{
		int max = 0;
		int index = 0;
		DynamicArray<votes>::iterator itr = _votes.begin();
		DynamicArray<votes>::iterator end = _votes.end();
		for (; itr != end; ++itr)
		{
			if (itr->_modVotes > max)
			{
				max = itr->_modVotes;
				index = itr.getIndex();
			}
		}
		return index;
	}
	
	void district::sortVotes()
	{
		int i, j;
		bool swapped;
		for (i = 0; i < _votes.size() - 1; i++)
		{
			swapped = false;
			for (j = 0; j < _votes.size() - i - 1; j++)
			{
				if (_votes[j]._numVotes < _votes[j + 1]._numVotes)
				{
					swap(_votes[j], _votes[j+1]);
					swapped = true;
				}
			}
			// IF no two elements were swapped by inner loop, then break 
			if (swapped == false)
				break;
		}
	}

	district::district(std::istream& in) : _serial(0), _electors(0), _votes(), _votersList()
	{
		in.read(rcastc(&counter), sizeof(counter));
		in.read(rcastc(&_name), sizeof(_name));
		in.read(rcastc(const_cast<int*>(&_serial)), sizeof(_serial));
		in.read(rcastc(const_cast<int*>(&_electors)), sizeof(_electors));
		in.read(rcastc(&_numVotes), sizeof(_numVotes));
		in.read(rcastc(&_percentage), sizeof(_percentage));
		_repListHead = nullptr;
		_repListTail = nullptr;
	}

	void district::saveDetails(std::ostream& out) const {
		specialDistrict check;
		int checker = 1;
		if (typeid(&check).name() == typeid(this).name())
			checker = 0;

		out.write(rcastcc(&checker), sizeof(checker));
		out.write(rcastcc(&counter), sizeof(counter));		//saves counter
		out.write(rcastcc(&_name), sizeof(_name));		//saves name
		out.write(rcastcc(&_serial), sizeof(_serial));		// saves serial number
		out.write(rcastcc(&_electors), sizeof(_electors));	// saves num of electors
		out.write(rcastcc(&_numVotes), sizeof(_numVotes));
		out.write(rcastcc(&_percentage), sizeof(_percentage));
	}
	void district::saveLists(std::ostream& out) 
	{
		const char* str;
		int counterRep,serial;						//first save how many citizens in the voters list.
		DynamicArray<citizen*>::const_iterator itr = _votersList.cbegin();
		DynamicArray<citizen*>::const_iterator end = _votersList.cend();
		int size = _votersList.size();
		out.write(rcastcc(&size), sizeof(_votersList.size()));
		for (; itr != end; ++itr)
		{
			str = (*itr)->getID();		//saves each citizen Id.
			out.write(rcastcc(str), 10);
		}
		representors* party;
		representors* rep;					
		party = _repListHead;		////first save how many reps in the reps list.
		counterRep = countRepresntors();
		out.write(rcastcc(&counterRep), sizeof(counterRep));
		while (party != nullptr)
		{
			rep = party;
			while (rep != nullptr)
			{
				str = rep->getRep()->getID();
				serial = rep->getParty()->getSerial();	
				out.write(rcastcc(str), 10);					//saves reps Id.
				out.write(rcastcc(&serial), sizeof(serial));    //saves reps party number.
				rep = rep->nextRep;
			}
			party = party->nextParty;
		}
	
	}
	void district::loadDetails(std::istream& in) {
		in.read(rcastc(&counter), sizeof(counter));
		in.read(rcastc(&_name), sizeof(_name));
		in.read(rcastc(const_cast<int*>(&_serial)), sizeof(_serial));
		in.read(rcastc(const_cast<int*>(&_electors)), sizeof(_electors));
		in.read(rcastc(&_numVotes), sizeof(_numVotes));
		in.read(rcastc(&_percentage), sizeof(_percentage));
	}

	void district::read(char* filename)
	{
		ifstream file(filename, ios::binary);
		loadDetails(file);
		file.close();
	}
	void district::write( char* filename)
	{
		ofstream file(filename, ios::binary | ios::trunc);
		saveDetails(file);
		file.close();
	}

	void district::saveVotes(std::ostream& out) 
	{
		int serial;

		DynamicArray<votes>::const_iterator itr = _votes.cbegin();
		DynamicArray<votes>::const_iterator end = _votes.cend();
		int size = _votes.size();
		out.write(rcastcc(&size), sizeof(size));
		for (; itr != end; ++itr)
		{//saves party by serial number, and saves electors and votes 
			serial = itr->_par->getSerial();
			out.write(rcastcc(&serial), sizeof(serial));
			out.write(rcastcc(&itr->_numVotes), sizeof(itr->_numVotes));
			out.write(rcastcc(&itr->totalElec), sizeof(itr->totalElec));
		}

	}

	void district::loadVotes(std::istream& in, electionsDay* eDay) {
		int serial, size;
		in.read(rcastc(&size), sizeof(size));
		votes temp;
		for (int i = 0; i < size; i++)
		{
			in.read(rcastc(&serial), sizeof(serial));
			temp._par = eDay->findParty(serial);
			in.read(rcastc(&temp._numVotes), sizeof(temp._numVotes));
			in.read(rcastc(&temp.totalElec), sizeof(temp.totalElec));
			_votes.push_back(temp);
		}
	}

	int district::countRepresntors() const
	{
		representors* rep = _repListHead;
		representors* party = _repListHead;
		int count = 0;
		while (party != nullptr)
		{
			rep = party;
			while (rep != nullptr)
			{
				count++;
				rep = rep->nextRep;
			}
			party = party->nextParty;
		}
		return count;
	}

	void district::loadLists(std::istream& in, electionsDay* eDay) {
		char id[10];
		int vSize;
		in.read(rcastc(&vSize), sizeof(vSize));
		for (int i = 0; i < vSize; i++)
		{
			in.read(rcastc(id), 10);
			this->addVoter(eDay->findCitizen(id)); 
		}
		int countRep;
		in.read(rcastc(&countRep), sizeof(countRep));
		int serial;
		for (int i = 0; i < countRep; i++)
		{
			in.read(rcastc(id), 10);
			in.read(rcastc(&serial), sizeof(serial));
			this->addRep(eDay->findCitizen(id), eDay->findParty(serial));
		}
	}
}






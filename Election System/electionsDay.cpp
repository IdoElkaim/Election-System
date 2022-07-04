#include "global.h"

namespace elections {

	electionsDay::electionsDay(std::istream& in) {
		in.read(rcastc(&_day), sizeof(_day));
		in.read(rcastc(&_month), sizeof(_month));
		in.read(rcastc(&_year), sizeof(_year));
		in.read(rcastc(&res._size), sizeof(res._size));
		res.arr = new results::PartyResByDis[res._size];
		_citList.clear();
		_parList.clear();
		_disList.clear();
	}

	electionsDay::electionsDay()
	{
		_day = -1;
		_month = -1;
		_year = -1;
		_citList.clear();
		_parList.clear();
		_disList.clear();
	} 

	electionsDay::electionsDay(int day, int month, int year) {
		_day = day;
		_month = month;
		_year = year;
		_citList.clear();
		_parList.clear();
		_disList.clear();
	}

	electionsDay::~electionsDay() {
		deleteList(_citList);
		_citList.clear();
		deleteList(_parList);
		_parList.clear();
		deleteList(_disList);
		_disList.clear();
	}

	bool electionsDay::addCitizen(const string name, const char* id, int yob, int disNum)
	{
		citizen* cit = findCitizen(id);
		if (cit != nullptr)
			throw "ID already exist.";

		district* dis = this->findDistrict(disNum);
		citizen* newCit = new citizen(name, id, yob, dis);
		if (!newCit)
			throw "Could not allocate citizen memory.";
		_citList.push_back(newCit);
		dis->addVoter(newCit);
		return true;
	}


	bool electionsDay::addDistrict(const string name, int repsNum, int isSpecial)
	{
		district* newDistrict;
		if (!isSpecial)
			newDistrict = new district(name, repsNum);
		else
			newDistrict = new specialDistrict(name, repsNum);
		
		_disList.push_back(newDistrict);
		return true;
	}

	bool electionsDay::addParty(const string name, const char* primeID)
	{
		citizen* prime = this->findCitizen(primeID);
		party* newParty = new party();
		newParty->setParty(name, prime);
		_parList.push_back(newParty);
		return true;
	}

	citizen* electionsDay::findCitizen(const char* Id) const
	{

		for (citizen* i : _citList)
			if (!strcmp(Id, i->getID()))
				return i;
		return nullptr;
	}

	void electionsDay::printCitizenList()
	{
		for (citizen* i : _citList)
			cout << *i << endl;
		cout << endl;
	}

	void electionsDay::printDistrictsList()
	{
		for (district* i : _disList)
			cout << *i << endl;
		cout << endl;
	}

	void electionsDay::printPartiesList()
	{
		for (party* i : _parList)
			cout << *i << endl;
		cout << endl;
	}

	district* electionsDay::findDistrict(int disNumber) const
	{
		for (district* i : _disList)
			if (i->getSerial() == disNumber)
				return i;
		return nullptr;
	}

	bool electionsDay::vote(const char* id, party* par)
	{
		const char* validID	= checkValidID(id);
		citizen* cit = this->findCitizen(validID);
		
		if (!cit->voted())
		{
			district* dis = cit->getDistrict();
			dis->addVote(par);
			cit->setVote(true);
		}
		else {
			throw "This citizen has already voted !";
		}
		return true;

	}

	void electionsDay::setParties()
	{
		list<party*>::iterator itr = _parList.begin();

		for (int i = 0; i < res._size; i++)
		{
			res.arr[i]._par = (*itr);
			itr++;
		}
	}

	party* electionsDay::findParty(int numParty) const
	{
		for (party* i : _parList)
			if (i->getSerial() == numParty)
				return i;
		return nullptr;
	}

	void electionsDay::calcResults()
	{
		res.adjustResult(this->getPartyCount());
		this->setParties();
		list<district*>::iterator itrDis = _disList.begin();

		for(district* i: _disList)
			i->calcResults(res);

		res.sortArray();

		for (int i = res._size-1; i >=0; i--)
		{
			std::cout << *(res.arr[i]._par) << std::endl;
			std::cout 
				<< "Number of votes: " << res.arr[i].numVotes
				<< ", Number of electors: " << res.arr[i].electors << std::endl;
		}
		cout << endl;
	}

	const int electionsDay::getDistrictCount() const
	{
		return _disList.size();
	}

	const int electionsDay::getPartyCount() const
	{
		return _parList.size();
	}

	const char* electionsDay::checkValidID(const char* id) {
		char* validID = new char[10];
		strcpy(validID, id);
		if (this->findCitizen(validID) == nullptr)
			throw "Wrong ID, citizen not found !";
		return validID;
	}

	const int electionsDay::checkValidParDis(int num, int flag)
	{
	#define PARTY 1
	#define DISTRICT 2
		if (flag == DISTRICT) {
			if (num<1 || num > getDistrictCount())
				throw "District not found !";
			return num;
		}
		if (flag == PARTY) {
			if (num<1 || num > getPartyCount())
				throw "Party not found !";
			return num;
		}
	}

	bool electionsDay::didPeopleVote()
	{
		for(district* i: _disList)
		{
			if (i->getHowManyPeopleVote() == 0 && i->_votersList.size()>0)
				return false;
		}
		return true;
	}
	bool electionsDay::enoughRep()
	{
		for(district* i : _disList)
		{
			i->calcElectors();
			if (!i->enoughRepresentors())
				return false;
		}
		return true;
	}

	void electionsDay::saveLists(std::ostream& out) const
	{
		simpleEDay* check;
		int checker = 1;
		if (typeid(&check).name() != typeid(this).name())
			checker = 0;
		out.write(rcastcc(&checker), sizeof(checker));
		out.write(rcastcc(&_day), sizeof(_day));
		out.write(rcastcc(&_month), sizeof(_month));
		out.write(rcastcc(&_year), sizeof(_year));
		out.write(rcastc(const_cast<int*>(&(res._size))), sizeof(res._size));
		int count = _disList.size();
		out.write(rcastcc(&count), sizeof(count));
		for(district* i: _disList)            ///saves all districts data
		{
			i->saveDetails(out);
		}
		count = _citList.size();
		out.write(rcastcc(&count), sizeof(count));
		for(citizen* i : _citList) 	         /// saves all citizens main details include district number 
		{
			i->save(out);
		}
		count = _parList.size();
		out.write(rcastcc(&count), sizeof(count));
		for(party* i : _parList)             /// saves all party details
		{
			i->saveDetails(out);
		}

	}
	void electionsDay::saveDistrictLists(std::ostream& out) const
	{
		for(district* i : _disList)
		{
			i->saveLists(out);
			i->saveVotes(out);
		}
	}

	void electionsDay::loadDistrictLists(std::istream& in) {
		for(district* i : _disList)
		{
			i->loadLists(in, this);
			i->loadVotes(in, this);
		}
	}

	void electionsDay::savePartyLists(std::ostream& out) const
	{
		for(party* i : _parList)
		{
			i->saveRepresentors(out);
		}
	}

	void electionsDay::loadPartyLists(std::istream& in)
	{
		for(party* i : _parList)
		{
			i->loadRepresentors(in, this);
		}
	}
	void electionsDay::loadRes(std::istream& in) {
		for (int i = 0; i < res._size; i++)
		{
			in.read(rcastc(&res.arr[i].numVotes), sizeof(res.arr[i].numVotes));
			in.read(rcastc(&res.arr[i].electors), sizeof(res.arr[i].electors));
		}
		int serial;
		for (int i = 0; i < res._size; i++)
		{
			in.read(rcastc(&serial), sizeof(serial));
			res.arr[i]._par = findParty(serial);
		}
	}

	void electionsDay::Load(std::istream& in) {
		loadLists(in);
		loadDistrictLists(in);
		loadPartyLists(in);
		loadRes(in);
	}

	void electionsDay::Save(std::ostream& out) const {
		saveLists(out);
		saveDistrictLists(out);
		savePartyLists(out);
		saveRes(out);
	}

	void electionsDay::loadLists(std::istream& in) {
		district* dis;
		
		int count;
		in.read(rcastc(&count), sizeof(count));
		for (int i = 0; i < count; i++)
		{
			dis = loadDistrict(in);
			_disList.push_back(dis);
		}

		citizen* cit;
		in.read(rcastc(&count), sizeof(count));
		for (int i = 0; i < count; i++)
		{
			cit = loadCitizen(in);
			_citList.push_back(cit);
		}

		party* par;
		in.read(rcastc(&count), sizeof(count));
		for (int i = 0; i < count; i++)
		{
			par = loadParty(in);
			_parList.push_back(par);
		}

	}

	district* electionsDay::loadDistrict(std::istream& in)
	{
		int check;
		in.read(rcastc(&check), sizeof(check));
		switch (check) {
		case 1:
			return new specialDistrict(in);
			break;
		case 0:
			return new district(in);
			break;
		}
	}

	party* electionsDay::loadParty(std::istream& in) {
		party* par = new party(in);
		char id[10];
		in.read(rcastc(id), 10);
		par->setPrime(findCitizen(id));
		return par;
	}

	citizen* electionsDay::loadCitizen(std::istream& in) {
		citizen* cit =  new citizen(in);
		int disNum;
		in.read(rcastc(&disNum), sizeof(disNum));
		cit->setDistrict(findDistrict(disNum));
		return cit;
	}
}

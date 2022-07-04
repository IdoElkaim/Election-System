#pragma once
#include "global.h"

namespace elections {
	class simpleEDay;
	class electionsDay {
	public:
		virtual bool addCitizen(const string name, const char* id, int yob, int disNum);
		virtual bool addDistrict(const string name, int repsNum, int isSpecial);
		bool addParty(const string name, const char* primeID);
		citizen* findCitizen(const char* id) const;
		district* findDistrict(int disNumber) const;
		party* findParty(int numParty) const;
		virtual void printCitizenList();
		virtual void printDistrictsList();
		virtual void printPartiesList();
		bool vote(const char* id, party* par);
		virtual void calcResults();
		void setParties();
		const int getDistrictCount() const;
		const int getPartyCount() const;
		const char* checkValidID(const char* id);
		const int checkValidParDis(int num, int flag);
		bool didPeopleVote();
		bool enoughRep();
		party* getPartyHead() const { return _parList.front(); }
		district* getDisHead() const { return _disList.front(); }
		citizen* getCitHead() const { return _citList.front(); }
		bool DisListIsEmpty() { return _disList.empty();}
        bool ParListIsEmpty() { return _parList.empty();}
        bool CitListIsEmpty() { return _citList.empty();}
        int getYear() { return _year; }
		
		//Serialization
		void saveLists(std::ostream& out) const;
		void saveDistrictLists(std::ostream& out) const;
		void savePartyLists(std::ostream& out) const;
		void saveRes(std::ostream& out) const { res.save(out); }
		void Save(std::ostream& out) const;
		void Load(std::istream& in);
		void loadLists(std::istream& in);
		district* loadDistrict(std::istream& in);
		party* loadParty(std::istream& in);
		citizen* loadCitizen(std::istream& in);
		void loadDistrictLists(std::istream& in);
		void loadPartyLists(std::istream& in);
		void loadRes(std::istream& in);

		electionsDay();
		electionsDay(std::istream& in);
		electionsDay(int day, int month, int year);
		virtual ~electionsDay();

	protected:
		results res;
		int _day, _month, _year;
		list<citizen*> _citList;
		list<party*> _parList;
		list<district*> _disList;
	};

	//Template function for deleting memory that was used
	template<class T>
	void deleteList(T& del) {
		auto beg = del.begin();
		auto end = del.end();

		for (; beg !=end; beg++)
			delete *beg;
	}
}


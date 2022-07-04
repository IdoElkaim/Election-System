#pragma once
#include "global.h"

namespace elections {

	class PartyMembers {
	public:
		friend class party;
		bool addMember(citizen* mem);
        int getDisNum() {return _disNum;}
		PartyMembers(int disnumber, citizen* mem);
		PartyMembers(std::istream& in);
		~PartyMembers() {};
		PartyMembers(const PartyMembers& other) { _disNum = other._disNum; _members = other._members; };
		void operator=(const PartyMembers&) = delete;

	private:

		int _disNum;
		DynamicArray<citizen*> _members;
	};

	class electionsDay;
	class party {
	public:
		bool setName(const string name);
		bool setPrime(citizen* prime);
		bool addMember(citizen* mem, int disNum);
		bool setParty(const string name, citizen* prime);
		friend std::ostream& operator<<(std::ostream& os, const party& par);
		const string getName() const { return _name; }
		int getSerial() const { return _serial; }
		void printList() const;
		bool listCheck(int disNum);
		const int getCount() { return this->counter; }
		const string getPrimeName() { return _prime->getName(); }
		const char* getPrimeId() { return _prime->getID(); }
		void saveDetails(std::ostream& out) const;
		void saveRepresentors(std::ostream& out) const;
		void loadDetails(std::istream& is);
		void loadRepresentors(std::istream& in, electionsDay* eDay);
		const int countMemListNodes() const;

		party(std::istream& in);
		party();
		party operator=(const party&) = delete;
		party(const party&) = delete;
		~party();
	private:
		static int counter;
		string _name;
		citizen* _prime;
		int _serial;
		list<PartyMembers> _membersList;
	};
}


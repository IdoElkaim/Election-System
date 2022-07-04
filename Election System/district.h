#pragma once
#include "global.h"


namespace elections {
	class results;
	class electionsDay;
	class specialDistrict;
	class district {
	public:
		friend class SimpleEDay;
		friend class electionsDay;
		void calcResults(results& res);
		bool addVoter(citizen* voter);
		bool addRep(citizen* mem, party* par);
		bool addVote(party* par);
		virtual void print(std::ostream& os) const;
		friend std::ostream& operator<<(std::ostream& os, const district& dis);
		int getSerial() const{ return _serial; }
		const string getName() const { return _name; }
		int getElectors() const { return _electors; }
		int getPartyVotes(party* par);
		party* getWinnerParty();
		bool setPercent(float per) { _percentage = per; return true; }
		virtual void printWinner();
		const int getCount();
		const int getHowManyPeopleVote() { return _numVotes; }
		void calcElectors();
		bool enoughRepresentors();
		const int getElectorsByParty(party* par);
		const int getMaxMod();
		void printData();
		void sortVotes();
		void reduceCounter() { counter--; }
		void saveDetails(std::ostream& out) const;
		void saveLists(std::ostream& out);
		void saveVotes(std::ostream& out);
		void loadDetails(std::istream& in);
		void loadLists(std::istream& in, electionsDay* eDay);
		void loadVotes(std::istream & in, electionsDay* eDay);
		void read( char* filename);
		void write( char* filename);
		const int getvListSize() { return _votersList.size(); }
		int countRepresntors() const;


		void operator=(const district&) = delete;
		district(const district&) = delete;
		district();
		district(std::istream& in);
		district(const string name, int electors);
		~district();

	protected:
		DynamicArray<votes> _votes;

	private:
		static int counter;
		string _name;
		const int _serial, _electors;
		int _numVotes;
		DynamicArray<citizen*> _votersList;
		float _percentage;
		representors* _repListHead, *_repListTail;
	};
}
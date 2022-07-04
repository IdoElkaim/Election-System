#pragma once
#include "global.h"

namespace elections {

	class results {
	public:
		friend class district;
		friend class electionsDay;
		void updateElec(int numOfElectors, party* par);
		void updateVotes(int numOfVotes, party* par);
		void sortArray();
		void swap(int ind);
		void adjustResult(int numOfParties);
		void save(std::ostream& out) const;

		void operator=(const results& res) = delete;
		results(std::istream& in);
		results();
		results(int numberOfParties);
		~results() {};

	private:

		struct PartyResByDis {
			int electors, numVotes;
			party* _par;
		};
		int _size;
		PartyResByDis* arr;	//array is not dynamic, getting the size according to the number of district created in				  //each elections day
	};
}

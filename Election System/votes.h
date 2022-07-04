#pragma once
#include "global.h"

namespace elections {
	class votes {

	public:
		friend class district;
		friend class specialDistrict;
		bool addVote(){ _numVotes++; return true;}
		int getElec() { return totalElec; }
		int getVotes() { return _numVotes; }
		int getVotes() const{ return _numVotes; }

		
		party* getParty() { return _par; }
		
		votes(party* par) { _par = par; _numVotes = 0; totalElec = 0; _modVotes = 0; }
		votes() {
			_par = nullptr;
			_numVotes = 0;
			totalElec = 0;
			_modVotes = 0;
		};

	private:
		int _numVotes, totalElec;
		party* _par;
		float _modVotes;
	};
}
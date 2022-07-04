#pragma once
#include "global.h"

namespace elections {

	class representors {
	public:		
		friend class district;
		friend class SimpleEDay;
		party* getParty() { return _par; }
		citizen* getRep() { return _rep; }

		representors() : _rep(nullptr), _par(nullptr), nextParty(nullptr), nextRep(nullptr) {};
		representors(citizen* member, party* par);

	private:
		citizen* _rep;
		party* _par;
		representors* nextParty;
		representors* nextRep;
	};
}
#include "global.h"

namespace elections {

	representors::representors(citizen* member, party* par)
	{
		_rep = member;
		_par = par;
		nextParty = nullptr;
		nextRep = nullptr;
	}
}
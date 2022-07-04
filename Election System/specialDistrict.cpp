#include "global.h"

namespace elections {

	specialDistrict::specialDistrict() : district() {};

	specialDistrict::specialDistrict(const string name, int repsNum) : 
	district(name, repsNum){
	}

	void specialDistrict::printWinner()
	{
		int i = 0;
		while (_votes[i].totalElec > 0)
			cout << "The candidate: " << _votes[i]._par->getPrimeName()
			<< " won " << _votes[i++].totalElec << " electors." << endl;
	}


	void specialDistrict::print(std::ostream& os) const {
		os << "District Number: " << this->getSerial() << ", Name: " << this->getName() <<
			" Type: Splitted" << ", Number Of Representatives: " << this->getElectors() << endl;
	}
}
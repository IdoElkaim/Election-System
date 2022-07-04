#pragma once
#include "global.h"

namespace elections {
	class results;
	class specialDistrict : public district {
	public:
		virtual void print(std::ostream& os) const;
		virtual void printWinner();

		
		specialDistrict();
		specialDistrict(std::istream& in) : district(in) {};

		specialDistrict(const string name, int repsNum);
	};
}
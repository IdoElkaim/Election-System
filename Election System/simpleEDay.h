#pragma once
#include "global.h"

namespace elections {
	class SimpleEDay : public electionsDay {
	public:
		friend class district;
		friend class representors;
		virtual bool addCitizen(const char* name, const char* id, int yob, int disNum);
		virtual void calcResults();
		virtual void printPartiesList();


		SimpleEDay();
		SimpleEDay(std::istream& in) : electionsDay(in) {};
		SimpleEDay(int day, int month, int year, int electors);

	};
}
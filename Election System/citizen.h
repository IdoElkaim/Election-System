#pragma once
#include "global.h"


namespace elections {

	class district;
	class citizen {
	public:
		bool setName(const string);
		bool setID(const char*);
		bool setYear(int year);
		bool setDistrict(district * dis);
		bool setVote(bool b);
		const char* getID() { return this->id; }
		const string getName() { return this->_name; }
		district * getDistrict() { return dis; }
		friend std::ostream& operator<<(std::ostream& os, const citizen& cit);
		bool voted() { return _voted; }
		void save(std::ostream& out) const;

		citizen();
		citizen(std::istream& in);
		citizen(const string name, const char* _id, int yob, district* dis);
		citizen(const citizen& ) = delete;
		citizen operator=(const citizen&) = delete;

	private:
		char id[10];
		string _name;
		bool _voted;
		int birthYear;
		district* dis;
	};
}


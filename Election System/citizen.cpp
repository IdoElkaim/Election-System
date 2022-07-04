#include "global.h"

namespace elections {

	bool citizen::setName(const string inName)
	{
		_name = inName;
		return true;
	}
	citizen::citizen(const string name, const char* _id, int yob, district* _dis)
	{
		_name = name;
		strcpy(id, _id);
		birthYear = yob;
		dis = _dis;
		_voted = false;
	}

	bool citizen::setID(const char* inID) 
	{
		strcpy(this->id, inID);
		return true;

	}

	bool citizen::setYear(int inYear)
	{
		this->birthYear = inYear;
		return true;

	}

	bool citizen::setDistrict(district * dis)
	{
		this->dis = dis;
		return true;

	}

	bool citizen::setVote(bool b)
	{
		this->_voted = b;
		return true;
	}

	citizen::citizen(std::istream& in) {

		in.read(rcastc(&_name), sizeof(_name));
		in.read(rcastc(id), sizeof(id));
		in.read(rcastc(&_voted), sizeof(_voted));
		in.read(rcastc(&birthYear), sizeof(birthYear));
		dis = nullptr;
	}


	citizen::citizen() : _voted(false) {
		birthYear = 0;
		dis = nullptr;
		id[0] = 0;
		_name = "";
	}

	std::ostream& operator<<(std::ostream& os, const citizen& cit)
	{
		os << "ID: " << cit.id << ", Name: " << cit._name <<
			", Birth Year: " << cit.birthYear << ", District: " << cit.dis->getSerial() << endl;
		return os;
	}

	void citizen::save(std::ostream& out) const {
		int disNum = this->dis->getSerial();
		out.write(rcastcc(&_name), sizeof(_name));			// saves name
		out.write(rcastcc(id), sizeof(id));		// saves id
		out.write(rcastcc(&_voted), sizeof(_voted));		//saves boolian voted
		out.write(rcastcc(&birthYear), sizeof(birthYear));	// saves year of birth
		out.write(rcastcc(&disNum), sizeof(disNum));		// saves district num
	}
}


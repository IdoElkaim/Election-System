#pragma once
#include "global.h"
using namespace std;

namespace elections {
	int party::counter = 0;

	PartyMembers::PartyMembers(std::istream& in) :_members() {
		in.read(rcastc(&_disNum), sizeof(_disNum));
	}


	PartyMembers::PartyMembers(int disnumber, citizen* mem) : _members()	{
		_disNum = disnumber;
		_members.push_back(mem);
	}

	bool PartyMembers::addMember(citizen* mem)
	{
	    _members.push_back(mem);
		return true;
	}

	party::party(std::istream& in) : _prime(nullptr), _membersList() {
		in.read(rcastc(&counter), sizeof(counter));
		in.read(rcastc(&_name), sizeof(_name));
		in.read(rcastc(const_cast<int*>(&_serial)), sizeof(_serial));

	}

	party::party() : _serial(++counter){
		_name = "";
		_prime = nullptr;
		_membersList.clear();
	}

	party::~party() {
	}

	bool party::setParty(const string name, citizen* prime) {
		this->setName(name);
		this->setPrime(prime);
		return true;

	}

	bool party::setName(const string name)
	{
		_name = name;
		return true;

	}

	bool party::setPrime(citizen* prime)
	{
		_prime = prime;
		return true;

	}

	bool party::listCheck(int disNum)
	{
	    list<PartyMembers>::iterator iter = _membersList.begin();
        list<PartyMembers>::iterator end = _membersList.end();
	    for(;iter != end; ++iter){
	        if(iter->getDisNum() == disNum)
	            return true;
        }
	    return false;
	}


	bool party::addMember(citizen* mem, int disNum)
	{
		if (this->listCheck(disNum))
		{
            list<PartyMembers>::iterator iter = _membersList.begin();
            list<PartyMembers>::iterator end = _membersList.end();
            for(;iter != end; ++iter){
                if(iter->getDisNum() == disNum)
                    iter->_members.push_back(mem);
            }
		}
		else
		{
			_membersList.push_back(PartyMembers(disNum, mem));
		}
		return true;

	}


	ostream& operator<<(ostream& os, const party& par)
	{
		os << "Party: " << par.getName() << ", Number: " << par.getSerial()
			<< ", Candidate: " << par._prime->getName() <<endl;
		par.printList();
		return os;
	}

	void party::printList() const
	{
        list<PartyMembers>::const_iterator iter = _membersList.begin();
        list<PartyMembers>::const_iterator end = _membersList.end();
        for(;iter != end; ++iter){
            for(int i = 0; i<iter->_members.size();i++)
                cout << *(iter->_members[i] )<< endl;
		}
        cout <<endl;
	}

	void party::saveDetails(std::ostream& out) const
	{
		const char* id = _prime->getID();
		out.write(rcastcc(&counter), sizeof(counter));
		out.write(rcastcc(&_name), sizeof(_name));
		out.write(rcastcc(&_serial), sizeof(_serial));
		out.write(rcastcc(id), 10);
	}


	void party::loadDetails(std::istream& in)
	{
		in.read(rcastc(&counter), sizeof(counter));
		in.read(rcastc(&_name), sizeof(_name));
		in.read(rcastc(&_serial), sizeof(_serial));
		in.read(rcastc(&counter), sizeof(counter));
	}

	void party::saveRepresentors(std::ostream& out) const
	{
		int count = countMemListNodes();
		out.write(rcastcc(&count),sizeof(count));
		char* str;
		int size, disNum;
        list<PartyMembers>::const_iterator iter = _membersList.begin();
        list<PartyMembers>::const_iterator end = _membersList.end();
        for(;iter != end; ++iter) {
            size=iter->_members.size();
            disNum = iter->_disNum;
            out.write(rcastcc(&disNum), sizeof(int));
            out.write(rcastcc(&size), sizeof(int));
            for(int i=0; i<size; i++)
            {
                str = const_cast<char*>(iter->_members[i]->getID());
                out.write(rcastcc(str), 10);
            }
        }
	}

	void party::loadRepresentors(std::istream& in, electionsDay* eDay) {
		int count, size;
		in.read(rcastc(&count), sizeof(count));
		PartyMembers* node;
		char id[10];
		for (int i = 0; i < count; i++)
		{
			node = new PartyMembers(in);
            in.read(rcastc(&size), sizeof(size));
            for (int j = 0; j < size; j++)
			{
				in.read(rcastc(id), 10);
				node->_members.push_back(eDay->findCitizen(id));
			}
			_membersList.push_back(*node);
		}
	}
	const int party::countMemListNodes() const
	{
		return _membersList.size();
	}

}
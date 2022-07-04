#pragma once
#include "global.h"

namespace elections {

	results::results(std::istream& in){
		in.read(rcastc(&_size), sizeof(_size));
		arr = new PartyResByDis[_size];
		for (int i = 0; i < _size; i++)
		{
			in.read(rcastc(&arr[i].numVotes), sizeof(arr[i].numVotes));
			in.read(rcastc(&arr[i].electors), sizeof(arr[i].electors));
		}
	}

	results::results() {
		_size = 0;
		arr = nullptr;
	}

	results::results(int numberOfParties)
	{
		_size = numberOfParties;
		arr = new PartyResByDis[_size];
		for (int i = 0; i < _size; i++)
		{
			arr[i].electors = 0;
			arr[i].numVotes = 0;
		}
	}

	void results::updateElec(int numOfElectors, party* par)
	{
		for (int i = 0; i < _size; i++)
		{
			if (arr[i]._par == par)
				arr[i].electors += numOfElectors;
		}
	}

	void results::updateVotes(int numOfVotes, party* par)
	{
		for (int i = 0; i < _size; i++)
		{
			if (arr[i]._par == par)
				arr[i].numVotes += numOfVotes;
		}
	}

	void results::sortArray()
	{
			int i, j;
			bool swapped;
			for (i = 0; i < _size-1; i++)
			{
				swapped = false;
				for (j = 0; j < _size - i-1; j++)
				{
					if (arr[j].electors > arr[j + 1].electors)
					{
						swap(j);
						swapped = true;
					}
				}

				// IF no two elements were swapped by inner loop, then break 
				if (swapped == false)
					break;
			}
	}

	void results::swap(int ind)
	{
		PartyResByDis temp;
		temp = arr[ind + 1];
		arr[ind + 1] = arr[ind];
		arr[ind] = temp;
	}

	void results::adjustResult(int numberOfParties)
	{
		_size = numberOfParties;
		arr = new PartyResByDis[_size];
		for (int i = 0; i < this->_size; i++)
		{
			arr[i].electors = 0;
			arr[i].numVotes = 0;
		}
	}

	void results::save(std::ostream& out) const
	{
		int serial;
		out.write(rcastcc(&this->_size), sizeof(_size)); // saves how many array slots
		for (int i = 0;i < _size;i++)
		{
			out.write(rcastcc(&arr[i].numVotes), sizeof(arr[i].numVotes)); //saves the party number of votes
			out.write(rcastcc(&arr[i].electors), sizeof(arr[i].electors)); // saves the party number of electors
		}
		for (int i = 0; i < _size; i++)
		{
			serial = arr[i]._par->getSerial();
			out.write(rcastcc(&serial), sizeof(serial));  //saves the party serian number.
		}
	}
}

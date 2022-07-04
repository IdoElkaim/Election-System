#pragma once
#include "global.h"

namespace elections {
	
	template<class T>
	class DynamicArray {
	public:
		DynamicArray(int size = 2) : _logicalSize(0), _physicalSize(size), _arr(new T[size]) {}
		DynamicArray(const DynamicArray& other) : _arr(nullptr) {*this = other;	}
		~DynamicArray() {delete[] _arr;	}

		const DynamicArray& operator=(const DynamicArray& other) {
			if (this != &other) {
				_logicalSize = other._logicalSize;
				_physicalSize = other._physicalSize;
				delete[] _arr;
				_arr = new T[_physicalSize];
				for (int i = 0; i < _logicalSize; i++)
					_arr[i] = other._arr[i];
			}
			return *this;
		}

		const T& operator[](int i) const { return _arr[i]; }
		T& operator[](int i) { return _arr[i]; }

		void push_back(const T& value) {
			if (_logicalSize == _physicalSize)
				resize();
			_arr[_logicalSize++] = value;
		}

		const T& front()    const { return _arr[0]; }
		int      size()     const { return _logicalSize; }
		int      capacity() const { return _physicalSize; }
		bool     empty()    const { return _logicalSize == 0; }
		void     clear() { _logicalSize = 0; }

		class iterator
		{
		private:
			DynamicArray* _da;
			int				_i;
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using different_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			iterator(DynamicArray& arr, int i) : _da(&arr), _i(i) {}
			iterator(const iterator& other) : _da(other._da), _i(other._i) {}

			// in const_iterator:	const_iterator(const iterator& other)
			//     					operator=(const iterator& other)


			const iterator& operator=(const iterator& other) {
				_da = other._da;
				_i = other._i;
				return *this;
			}

			// comparison with another iterator
			bool operator==(const iterator& other) const {
				return (_da == other._da) && (_i == other._i);
			}
			bool operator!=(const iterator& other) const {
				return !(*this == other);
			}

			// smart-pointer iterator methods
			T& operator*() {
				return _da->_arr[_i];
			}
			T* operator->() {
				return &_da->_arr[_i];
			}

			// increment-decrement iterator methods
			iterator& operator++() {//++x (prefix)
				++_i;
				return *this;
			}
			iterator operator++(int) {//x++ (sufix)
				iterator temp(*this);
				++_i;
				return temp;
			}
			iterator& operator--() {
				--_i;
				return *this;
			}
			iterator operator--(int) {
				iterator temp(*this);
				--_i;
				return temp;
			}

			int getIndex() { return _i; }
		};

		class const_iterator
		{
		private:
			const DynamicArray* _da;
			int				_i;
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using different_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			const_iterator(DynamicArray& arr, int i) : _da(&arr), _i(i) {}
			const_iterator(const iterator& other) : _da(other._da), _i(other._i) {}

			const const_iterator& operator=(const iterator& other) {
				_da = other._da;
				_i = other._i;
				return *this;
			}

			// comparison with another iterator
			bool operator==(const iterator& other) const {
				return (_da == other._da) && (_i == other._i);
			}
			bool operator!=(const iterator& other) const {
				return !(*this == other);
			}
			bool operator==(const const_iterator& other) const {
				return (_da == other._da) && (_i == other._i);
			}
			bool operator!=(const const_iterator& other) const {
				return !(*this == other);
			}

			// smart-pointer iterator methods
			const T& operator*() const{
				return _da->_arr[_i];
			}
			const T* operator->() const{
				return &_da->_arr[_i];
			}

			// increment-decrement iterator methods
			const_iterator& operator++() {
				++_i;
				return *this;
			}
			const_iterator operator++(int) {
				const_iterator temp(*this);
				++_i;
				return temp;
			}
			const_iterator& operator--() {
				--_i;
				return *this;
			}
			const_iterator operator--(int) {
				const_iterator temp(*this);
				--_i;
				return temp;
			}
			int getIndex() { return _i; }

		};

		class reverse_iterator
		{
		private:
			DynamicArray* _da;
			int				_i;
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using different_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			reverse_iterator(DynamicArray& arr, int i) : _da(&arr), _i(i) {}
			reverse_iterator(const reverse_iterator& other) : _da(other._da), _i(other._i) {}

			// in const_iterator:	const_iterator(const iterator& other)
			//     					operator=(const iterator& other)

			friend class const_iterator;

			const reverse_iterator& operator=(const reverse_iterator& other) {
				_da = other._da;
				_i = other._i;
				return *this;
			}

			// comparison with another iterator
			bool operator==(const iterator& other) const {
				return (_da == other._da) && (_i == other._i);
			}
			bool operator!=(const iterator& other) const {
				return !(*this == other);
			}

			// smart-pointer iterator methods
			T& operator*() {
				return _da->_arr[_i];
			}
			T* operator->() {
				return &_da->_arr[_i];
			}

			// increment-decrement iterator methods
			iterator& operator++() {//--x
				--_i;
				return *this;
			}
			iterator operator++(int) {//x--
				iterator temp(*this);
				--_i;
				return temp;
			}
			iterator& operator--() {
				++_i;
				return *this;
			}
			iterator operator--(int) {
				iterator temp(*this);
				++_i;
				return temp;
			}
		};
		friend class const_iterator;
		friend class reverse_iterator;

		// should return iterator to new element
		void insert(const iterator& pos, const T& val) {
			if (_logicalSize == _physicalSize)
				resize();

			iterator itrEnd = end();
			iterator itrCurrent = itrEnd, itrPrev = --itrEnd;
			while (itrCurrent != pos)
			{
				*itrCurrent = *itrPrev;
				itrCurrent = itrPrev--;
			}

			iterator p = pos;
			*p = val;
			++_logicalSize;
		}

		// returns iterator to first element after deleted element/s
		iterator erase(const iterator& iter) {
			if (iter.i == _logicalSize - 1) {
				_logicalSize--;
				return _arr[_logicalSize];
			}
			iterator res(_arr[iter.i]);
			for (int i = iter.i; i < _logicalSize - 1; i++)
				_arr[i] = _arr[i + 1];
			_logicalSize--;
			return res;
		}

		iterator erase(const iterator& first, const iterator& last) {
			int diff = last.i - first.i, counter = 0;
			iterator res(_arr[first.i]);
			for (int i = last.i; i < _logicalSize; i++)
			{
				_arr[first.i + counter++] = _arr[i];
			}
			_logicalSize -= diff;
			return res;
		}

		iterator begin() {
			return iterator(*this, 0);
		}
		iterator end() {
			return iterator(*this, _logicalSize);
		}
		/*const_iterator begin() const{
			return const_iterator(*this, 0);
		}
		const_iterator end() const {
			return const_iterator(*this, _logicalSize);
		}*/
		
		const_iterator cbegin() {
			return const_iterator(*this, 0);
		}
		const_iterator cend()  {
			return const_iterator(*this, _logicalSize);
		}

		

		reverse_iterator rbegin() { return reverse_iterator(_arr, _logicalSize); }
		reverse_iterator rend() { return reverse_iterator(_arr, 0); }

		void print() const {
			for (int i = 0; i < _logicalSize; i++)
				std::cout << _arr[i] << " ";
			std::cout << std::endl;
		}

		void resize() {
			_physicalSize *= 2;
			T* temp = new T[_physicalSize];
			for (int i = 0; i < _logicalSize; i++)
				temp[i] = _arr[i];

			delete[] _arr;
			_arr = temp;
		}

	private:

		T* _arr;
		int _logicalSize;
		int _physicalSize;
	};
}

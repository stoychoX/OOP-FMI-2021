//
//Created by 82275 on 27.04.2021
//partly using code used in recent OOP seminars (FMI Sofia).
//

#pragma once
#include<iostream>
#include<exception>

const size_t INITIAL_CAPACITY = 1; //Default capacity of non-initialized class
const size_t GROWTH_CAPACITY = 2; //Default value for growth capacity

namespace psn {
	/*Why create a iterator?
	- To be able to use range based for loops*/
	template<class T>
	class iterator {
	public:
		//using:
		using _valType = typename T::_valType; //Fixing issues with handling types!
		using _pntType = _valType*; //Last two aren't a must, making the code easier
		using _refType = _valType&; //to read and understand.

	public:
		iterator(_pntType passedVal) : memPointer(passedVal) {};

		iterator& operator++() {
			memPointer++;
			return *this;
		}

		iterator& operator++(int) {
			iterator it = *this;
			++(*this);
			return it;
		}

		iterator& operator--() {
			memPointer--;
			return *this;
		}

		iterator& operator--(int) {
			iterator it = *this;
			--(*this);
			return it;
		}

		_refType operator[](const size_t& index) {
			return *(memPointer + index);
		}

		_pntType operator->() {
			return *(memPointer);
		}

		_refType operator*() {
			return *(memPointer);
		}

		bool operator==(const iterator& it) const {
			return (this->memPointer == it.memPointer);
		}

		bool operator!=(const iterator& it) const {
			return !(memPointer == it.memPointer);
		}

		bool operator>(const iterator& other) const {
			return (memcmp(this, &other, sizeof(*this)) == 1);
		}

		bool operator<(const iterator& other) const {
			return (memcmp(this, &other, sizeof(*this)) == -1);
		}

	private:
		_pntType memPointer;
	};

	template <class T>
	class vector {
	public:
		using _valType = T;
		using Iterator = iterator<vector<T>>; //making it just like STL: vector<T>::Iterator
	private:
		T* value;
		size_t size;
		size_t capacity;
	public:
		//constructors:
		vector();
		vector(const size_t& _Size);
		vector(const size_t& _Size, const T& _Val);
		vector(const vector& other);
		vector(vector&& other);

		//operators = and ==
		vector& operator=(vector& other) noexcept;
		vector& operator=(vector&& other) noexcept;
		bool operator==(const vector& other) const;
		bool operator!=(const vector& other) const;

		//size related operations
		void reserve(const size_t& newCapacity);
		void resize(const size_t& newSize);
		void clear() noexcept;
		size_t Size() const;
		size_t Capacity() const;
		bool isEmpty() const;
		void shrink_to_fit();

		//insertion or removal operations:
		void push_back(const T& value);
		void pop_back();
		void insertList(const size_t& _Where, T* list, const size_t& sizeOfItems);
		vector<T> operator+(const vector<T> other) const;
		vector<T> intersec(const vector<T> other) const;
		vector<T>& operator+=(const vector<T>& other);

		//getting items out of vector:
		const T& operator[] (const size_t& index) const;
		T& operator[] (const size_t& index);
		vector<T> subset(const size_t& from, const size_t& to) const;
		vector<T> inclusiveSubset(const size_t& from, const size_t& to) const;
		vector<T> leftInclusiveSubset(const size_t& from, const size_t& to) const;
		vector<T> rightInclusiveSubset(const size_t& from, const size_t& to) const;

		//iterators:
		Iterator begin() const {
			return Iterator(value);
		}
		Iterator end() const {
			return Iterator(value + size);
		}

		//destroy:
		~vector();

		//print methods:
		/*Definition of operator << outside a class is a funcion template,
		* but if you define it inside a class it is not.
		* That's why we need to template it ourselves.
		*/
		template <class U>
		friend std::ostream& operator<< (std::ostream& os, const vector<U>& vect);

	private:
		void printAll(std::ostream& os) const;

		//swap:
		void swapVectors(vector& first);
		//memory allocators:

		/*memalloc:
		* Allocating memory of type T with _Size's capacity.
		* Returning a pointer to the allocated memory.*/
		static T* memalloc(const size_t& _Size) {
			return static_cast<T*> (operator new (sizeof(T) * _Size));
		}

		/*constructRange(T*,T*,const T&):
		* Used to construct with param constructor objects directly in the memory, which is in between @param: begin and @param: end
		* _Value: the default value, which we use for our elements.
		*/
		static void constructRange(T* begin, T* end, const T& _Value) {
			while (begin != end) {
				new ((void*)begin) T(_Value);
				begin++;
			}
		}

		/*constructRange(T*,T*):
		* Used to construct with default constructor objects directly in the memory, which is in between @param: begin and @param: end
		*/
		static void constructRange(T* begin, T* end) {
			while (begin != end) {
				new ((void*)begin) T();
				begin++;
			}
		}

		/*copyRange:
		* Copying range of memory, which is between @param: begin and @param: end, to certan destination, pointed to eith @param: _Dest
		*/
		static void copyRange(T* begin, T* end, T* _Dest) {
			while (begin != end) {
				new ((void*)_Dest) T(*begin);
				begin++; _Dest++;
			}
		}

		/*destructRange:
		* Calling default destructor on a range of memory, starting from @param: begin and ending at @param: end
		*/
		static void destructRange(T* begin, T* end) {
			while (begin != end) {
				begin->~T();
				++begin;
			}
		}

		/*void freeRange:
		* used to destruct and delete everything in range, starting from: @param begin
		* and ending at: @param end
		*/
		static void freeRange(T* begin, T* end) {
			destructRange(begin, end);
			operator delete(begin);
		}

		/*copyDynamic:
		* Returns a pointer to a new memory,
		*allocated with a given capacity represented by @param: newCapacity, and filled with
		* elements from the current vector.
		*/
		T* copyDynamic(T* dataToCopyFrom, const size_t& sizeOfCopiedItems, const size_t& newCapacity) {
			T* newData = memalloc(newCapacity);
			copyRange(dataToCopyFrom, dataToCopyFrom + sizeOfCopiedItems, newData);
			return newData;
		}
	};

	/*Default constructor: */
	template<class T>
	vector<T>::vector() : value(nullptr), size(0), capacity(0) {};

	/*
	* Allocating memory in the initialization list and constructing range.
	* @param: size is used to allocate vector with _Size elements.
	*/
	template<class T>
	vector<T>::vector(const size_t& _Size) : value(memalloc(_Size)), size(_Size), capacity(_Size) {
		constructRange(value, value + capacity);
	};

	template<class T>
	vector<T>::vector(const size_t& _Size, const T& _Val) : value(memalloc(_Size)), size(_Size), capacity(_Size) {
		constructRange(value, value + capacity, _Val);
	};

	template<class T>
	vector<T>::vector(const vector& other) : value(memalloc(other.capacity)), size(other.size), capacity(other.capacity) {
		copyRange(other.value, other.value + size, value); //Getting the information we need in this->value.
	};

	/*Move constructor*/
	template<class T>
	vector<T>::vector(vector&& other) : value(other.value), size(other.size), capacity(other.capacity) {
		other.value = nullptr;
		other.size = 0;
	}

	/*Destructor*/
	template<class T>
	vector<T>::~vector() {
		freeRange(value, value + size);
	}

	template<class T>
	const T& vector<T>::operator[](const size_t& index) const {
		if (index < 0 || index >= size)
			throw std::exception("Invalid index!");

		return this->value[index];
	}

	template<class T>
	T& vector<T>::operator[](const size_t& index) {
		if (index < 0 || index >= size)
			throw std::exception("Invalid index!");

		return this->value[index];
	}

	template<class T>
	size_t vector<T>::Size() const {
		return this->size;
	}

	template<class T>
	bool vector<T>::isEmpty() const {
		return (this->size == 0);
	}

	/*void push_back:
	* If we still have capacity
	* we allocate (value + size)'th pointer with _Val.
	* If we dont have size, we resize the vector
	* and add T(_Val) after the resize.
	*/
	template<class T>
	void vector<T>::push_back(const T& _Val) {
		/*If we have enough capacity we simply push: */
		if (size != capacity) {
			new ((void*)(value + size)) T(_Val);
			size++;
			return;
		}

		size_t newCapacity;

		if (capacity == 0)
			newCapacity = INITIAL_CAPACITY;
		else
			newCapacity = capacity * GROWTH_CAPACITY;

		T* newData = copyDynamic(this->value, this->size, newCapacity);
		new ((void*)(newData + size)) T(_Val); //pushing
		freeRange(value, value + size);
		value = newData;
		capacity = newCapacity;
		size++;
	}

	/*pop_back:
	* It's importatn first to call ~T() and then
	* reduce memory, in order not to leak memory.
	*/
	template<class T>
	void vector<T>::pop_back() {
		if (this->isEmpty())
			return;

		(value + size - 1)->~T();
		size--;
	}

	/*vector<T> operator=:
	* Used copy-and-swap idiom.
	* @param: other is used to get the information in our vector;
	*/
	template<class T>
	vector<T>& vector<T>::operator=(vector<T>& other) noexcept {
		vector<T> v(other);
		swapVectors(other);

		return *this;
	}

	/*move semantics for operator ==*/
	template<class T>
	vector<T>& vector<T>::operator=(vector<T>&& other) noexcept {
		if (this != &other) {
			delete[] value; //simple delete[] operator works here
			this->size = other.size;
			value = other.value;

			other.value = nullptr;
			other.size = 0;
		}

		return *this;
	}

	/*void printAll:
	* Getting all the elements of the vector and writing them in @param: os
	*/
	template<class T>
	void vector<T>::printAll(std::ostream& os) const {
		for (int i = 0; i < size; i++)
			os << value[i] << " ";
	}

	/*vector operator==:
	* Comparing two vectors, returns true if the're equal
	* and false otherwise.
	*/
	template<class T>
	bool vector<T>::operator==(const vector& other) const {
		bool _equals = this->size == other.size &&
			this->size != 0;

		if (_equals) {
			int size = sizeof(this->value[0]);

			for (int i = 0; i < this->size; i++)
				_equals = _equals &&
				memcmp(&this->value[i], &other.value[i], size) == 0;

			return _equals;
		}

		return false;
	}

	template<class T>
	bool vector<T>::operator!=(const vector<T>& other) const {
		return !(*this == other);
	}

	/*used to swap two vectors, via std::swap*/
	template<class T>
	void vector<T>::swapVectors(vector& first) {
		//using std::swap for copy-and-swap idiom.
		std::swap(*this, first);
	}

	/*void reserve:
	* @param newSize: getting a size_t size, creating a new mem block
	* and giving it to value.
	* Example: old value (1, 2, 3, 4, 5, \, \) capacity = 7
	* reserve(9) -> new value (1, 2, 3, 4, 5, \, \, \, \) capacity = 9, size still 5
	*/
	template<class T>
	void vector<T>::reserve(const size_t& newSize) {
		if (newSize <= capacity)
			return;

		T* newData = copyDynamic(this->value, size, newSize);
		freeRange(value, value + size);
		this->value = newData;
		this->capacity = newSize;
	}

	template <class T>
	void vector<T>::resize(const size_t& newSize) {
		if (newSize <= size) {
			destructRange(value + newSize, value + size);
			size = newSize;
			return;
		}

		if (newSize <= capacity) {
			constructRange(value + size, value + newSize);
			capacity = newSize;
			return;
		}

		size_t newCapacity = newSize;
		size_t growth = size * GROWTH_CAPACITY;

		if (newCapacity < growth)
			newCapacity = growth;

		reserve(newSize); //added

		T* newData = copyDynamic(value, size, newCapacity);
		constructRange(this->value + this->size, this->value + newSize);
		freeRange(value, value + size);

		value = newData;
		capacity = newCapacity;
		size = newSize;
	}

	template <class T>
	inline std::ostream& operator<<(std::ostream& os, const psn::vector<T>& vect) {
		vect.printAll(os);
		return os;
	}
	/*clears everything*/
	template <class T>
	void vector<T>::clear() noexcept {
		this->size = 0;
		this->capacity = 0;

		freeRange(this->value, this->value + this->size);

		this->value = nullptr;
	}

	/*Inserting an array of objects in
	@param where -> The exact location of our list in the vector.
	@param list -> the elements we would like to add.
	@param countOfItems -> how many items would we like to add*/
	template <class T>
	void vector<T>::insertList(const size_t& _Where, T* list, const size_t& countOfItems) {
		if (_Where < 0 || _Where > size)
			throw std::exception("Invalid index");

		if (size + countOfItems > capacity)
			reserve(size + countOfItems);

		/*Creating memory block capable of holding all the elements and copying the elements
		* before _Where in*/
		T* newData = copyDynamic(this->value, _Where, size + countOfItems);

		/*Getting the elements from list in:*/
		copyRange(list, list + countOfItems, newData + _Where);

		/*getting the rest of the items in*/
		copyRange(this->value + _Where, this->value + size, newData + _Where + countOfItems);

		freeRange(this->value, this->value + size);
		this->value = newData;
		this->size = this->size + countOfItems;
	}

	template <class T>
	vector<T> vector<T>::subset(const size_t& from, const size_t& to) const {
		if (from < 0 || to < 1 ||
			from > size - 1 || to > size)
			throw std::exception("Invalid indexes");

		int newSize = to - from - 1;
		vector<T> toReturn;

		T* data = memalloc(newSize);
		copyRange(value + from, value + to, data);

		toReturn.insertList(0, data, newSize);

		toReturn.capacity = newSize;
		return toReturn;
	}

	template<class T>
	vector<T> vector<T>::inclusiveSubset(const size_t& from, const size_t& to) const {
		return subset(from - 1, to + 1);
	}

	template<class T>
	vector<T> vector<T>::leftInclusiveSubset(const size_t& from, const size_t& to) const {
		return subset(from - 1, to);
	}

	template<class T>
	vector<T> vector<T>::rightInclusiveSubset(const size_t& from, const size_t& to) const {
		return subset(from, to + 1);
	}

	template <class T>
	vector<T> vector<T>::operator+(const vector<T> other) const {
		size_t newSize = this->size + other.size;
		size_t newCapacity = newSize;

		vector<T> toReturn;
		toReturn.insertList(0, this->value, this->size);
		toReturn.insertList(this->size, other.value, other.size);
		toReturn.size = newSize; toReturn.capacity = newSize;

		return toReturn;
	}

	template <class T>
	vector<T> vector<T>::intersec(const vector<T> other) const {
		vector<T> container;

		for (int i = 0; i < this->size; i++) {
			for (int j = 0; j < other.size; j++) {
				if (this->value[i] == other.value[j]) {
					container.push_back(this->value[i]);
					break;
				}
			}
		}

		return container;
	}

	template<class T>
	vector<T>& vector<T>::operator+=(const vector<T>& other) {
		for (int i = 0; i < other.size; i++) {
			this->push_back(other.value[i]);
		}
		return *this;
	}

	template<class T>
	size_t vector<T>::Capacity() const {
		return this->capacity;
	}

	template<class T>
	void vector<T>::shrink_to_fit() {
		destructRange(this->value + this->size, this->value + this->capacity); //TODO -> check efficiency
		this->capacity = this->size;
	}
}
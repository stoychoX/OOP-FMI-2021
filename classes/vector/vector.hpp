/********************************************************************
* Created by 82275 on 27.04.2021
* using code written in recent OOP seminars (FMI Sofia).
********************************************************************/

#ifndef VECTOR_HEADER
#define VECTOR_HEADER

const size_t INITIAL_CAPACITY = 16;
const size_t GROWTH_CAPACITY = 2;

#include<exception>
#include<cassert>  
#include<cstddef> // size_t

template <class T>
class vector {
private:
	class vector_iterator {
	public:
		using valType = T;
		using pntType = valType*;
		using refType = valType&;

	public:
		vector_iterator(pntType passedVal) : memPointer{passedVal} {};
		vector_iterator(pntType passedVal, size_t _push) : memPointer{ passedVal + _push } {};

		vector_iterator& operator++() {
			memPointer++;
			return *this;
		}

		vector_iterator& operator++(int) {
			vector_iterator it = *this;
			++(*this);
			return it;
		}

		vector_iterator& operator--() {
			memPointer--;
			return *this;
		}

		vector_iterator& operator--(int) {
			vector_iterator it = *this;
			--(*this);
			return it;
		}

		vector_iterator& operator+(int off) const {
			vector_iterator res{ memPointer + off };
			return res;
		}

		pntType operator->() {
			return memPointer;
		}

		const pntType operator->() const {
			return memPointer;
		}

		refType operator*() {
			return *(memPointer);
		}

		const refType operator*() const {
			return *(memPointer);
		}

		bool operator==(const vector_iterator& it) const {
			return (this->memPointer == it.memPointer);
		}

		bool operator!=(const vector_iterator& it) const {
			return !(memPointer == it.memPointer);
		}

		bool operator>(const vector_iterator& other) const {
			return memPointer > other.memPointer;
		}

		bool operator<(const vector_iterator& other) const {
			return memPointer < other.memPointer;
		}
	private:
		pntType memPointer;
		friend class vector;
	};

public:
	using Iterator = vector_iterator;

private:
	T* value;
	size_t v_size;
	size_t v_capacity;

public:
	vector();
	explicit vector(const size_t& _Size);
	vector(const size_t& _Size, const T& _Val);
	vector(const vector& other);
	vector(vector&& other) noexcept;
	vector(Iterator Beg, Iterator End);

	vector& operator=(const vector& other) noexcept;
	vector& operator=(vector&& other) noexcept;

	void reserve(const size_t& newCapacity);
	void resize(const size_t& newSize);
	void clear() noexcept;
	size_t size() const;
	size_t capacity() const;
	bool isEmpty() const;
	void shrink_to_fit();

	void push_back(const T& value);
	void move_back(T&& value);
	void pop_back();
	void insertList(size_t _Where, const T* list, size_t sizeOfItems);

	const vector<T>& operator+(const vector<T>& other) const;
	const vector<T>& intersec(vector<T> other) const;
	vector<T>& operator+=(const vector<T>& other);
	void erase(Iterator at);

	const T& operator[] (size_t index) const;
	T& operator[] (size_t index);

	vector<T> subset(Iterator from, Iterator to) const;
	void sort(int from, int to);
	void sort();

	Iterator begin() {
		return Iterator(value);
	}

	Iterator end() {
		return Iterator(value + v_size);
	}

	const Iterator& begin() const {
		return Iterator(value);
	}

	const Iterator& end() const {
		return Iterator(value + v_size);
	}

	const Iterator& c_begin() const {
		return Iterator(value);
	}

	const Iterator& c_end() const {
		return Iterator(value + v_size);
	}

	const Iterator& find(Iterator beginRange, Iterator endRange, const T& item) const;
	const Iterator& find(const T&) const;
	bool contains(const T& elem) const;

	int distance(const Iterator& Beg, const Iterator& End) const;

	~vector();
private:
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

	/* constructRange(T*,T*):
	*  Used to construct with default constructor objects directly in the memory, which is in between @param: begin and @param: end
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
	static void copyRange(const T* begin, const T* end, T* _Dest) {
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
	T* copyDynamic(T* dataToCopyFrom, const size_t& sizeOfCopiedItems, size_t newCapacity) {
		T* newData = memalloc(newCapacity);
		copyRange(dataToCopyFrom, dataToCopyFrom + sizeOfCopiedItems, newData);
		return newData;
	}

	static void moveRange(T* begin, T* end, T* _Dest) {
		while (begin != end) {
			new ((void*)_Dest) T(std::move(*begin));
			begin++; _Dest++;
		}
	}

	T* moveDynamic(T* dataToCopyFrom, const size_t& sizeOfCopiedItems, size_t newCapacity) {
		T* newData = memalloc(newCapacity);
		moveRange(dataToCopyFrom, dataToCopyFrom + sizeOfCopiedItems, newData);
		return newData;
	}

	size_t partition(T* data, size_t size);

	void qsort(int from, int to);

	int closestPowerOfTwo(int v) {
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v++;
		return v;
	}
};

template<class T>
vector<T>::vector() : value{ nullptr }, v_size{ 0 }, v_capacity{ 0 } {};

/*
* Allocating memory in the initialization list and constructing range.
* @param: size is used to allocate vector with _Size elements.
*/
template<class T>
vector<T>::vector(const size_t& _Size) : value{ memalloc(_Size) }, v_size{ _Size }, v_capacity{ _Size } {
	constructRange(value, value + v_capacity);
};

template<class T>
vector<T>::vector(const size_t& _Size, const T& _Val) : value{ memalloc(_Size) }, v_size{ _Size }, v_capacity{ _Size } {
	constructRange(value, value + v_capacity, _Val);
};

template<class T>
vector<T>::vector(const vector& other) : value{ memalloc(other.v_capacity) }, v_size{ other.v_size }, v_capacity{ other.v_capacity } {
	copyRange(other.value, other.value + v_size, value);
};

template<class T>
vector<T>::vector(vector&& other) noexcept : value{ other.value }, v_size{ other.v_size }, v_capacity{ other.v_capacity } {
	other.value = nullptr;
	other.v_size = 0;
}

template<class T>
vector<T>::vector(Iterator Beg, Iterator End) {
	int diff = distance(Beg, End);
	assert(diff >= 0);
	int cap = closestPowerOfTwo(diff);
	value = copyDynamic(Beg.memPointer, diff, cap);
	v_size = diff;
	v_capacity = cap;
}

template<class T>
const typename vector<T>::Iterator& vector<T>::find(Iterator Beg, Iterator End, const T& item) const {
	for (; Beg != End; ++Beg)
		if (*Beg == item)
			return Beg;

	return end();
}

template<class T>
const typename vector<T>::Iterator& vector<T>::find(const T& item) const {
	return find(begin(), end(), item);
}

template<class T>
bool vector<T>::contains(const T& elem) const {
	return find(begin(), end(), elem) != end();
}

template<class T>
vector<T>::~vector() {
	freeRange(value, value + v_size);
}

template<class T>
size_t vector<T>::partition(T* data, size_t collectionSize) {
	size_t pp = 0;
	T pivot = data[collectionSize - 1];

	for (size_t i = 0; i < collectionSize - 1; i++)
		if (data[i] < pivot)
			std::swap(data[i], data[pp++]);

	std::swap(data[pp], data[collectionSize - 1]);

	return pp;
}

template<class T>
void vector<T>::qsort(int from, int to) {
	if (to - from + 1 < 2)
		return;

	size_t idx = partition(value + from, to - from + 1);
	qsort(from, from + idx - 1);
	qsort(from + idx + 1, to);
}

template<class T>
int vector<T>::distance(const Iterator& Beg, const Iterator& End) const {
	return End.memPointer - Beg.memPointer;
}

template<class T>
const T& vector<T>::operator[](size_t index) const {
	if (index >= v_size)
		throw std::exception("Invalid index!");

	return value[index];
}

template<class T>
T& vector<T>::operator[](size_t index) {
	if (index >= v_size)
		throw std::exception("Invalid index!");

	return value[index];
}

template<class T>
size_t vector<T>::size() const {
	return v_size;
}

template<class T>
bool vector<T>::isEmpty() const {
	return !v_size;
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
	if (v_size != v_capacity) {
		new ((void*)(value + v_size)) T(_Val);
		v_size++;
		return;
	}

	size_t newCapacity = (v_capacity == 0) ? INITIAL_CAPACITY : v_capacity * GROWTH_CAPACITY;

	T* newData = copyDynamic(value, v_size, newCapacity);
	new ((void*)(newData + v_size)) T(_Val);

	freeRange(value, value + v_size);

	value = newData;
	v_capacity = newCapacity;
	v_size++;
}

template<class T>
inline void vector<T>::move_back(T&& _value) {
	if (v_size != v_capacity) {
		new ((void*)(value + v_size)) T(std::move(_value));
		v_size++;
		return;
	}

	size_t newCapacity = (v_capacity == 0) ? INITIAL_CAPACITY : v_capacity * GROWTH_CAPACITY;

	T* newData = moveDynamic(value, v_size, newCapacity);
	new ((void*)(newData + v_size)) T(std::move(_value));

	freeRange(value, value + v_size);

	value = newData;
	v_capacity = newCapacity;
	v_size++;
}

/*pop_back:
* It's importatn first to call ~T() and then
* reduce memory, in order not to leak memory.
*/
template<class T>
void vector<T>::pop_back() {
	if (this->isEmpty())
		return;

	(value + v_size - 1)->~T();
	v_size--;
}

template<class T>
vector<T>& vector<T>::operator=(const vector<T>& other) noexcept {
	if (this != &other) {
		freeRange(value, value + v_size);
		value = copyDynamic(other.value, other.v_size, other.v_capacity);

		v_size = other.v_size;
		v_capacity = other.v_capacity;
	}

	return *this;
}

template<class T>
vector<T>& vector<T>::operator=(vector<T>&& other) noexcept {
	if (this != &other) {
		delete[] value;
		v_size = other.v_size;
		value = other.value;

		other.value = nullptr;
		other.v_size = 0;
	}

	return *this;
}

/*void reserve:
* @param newSize: getting a size_t size, creating a new mem block
* and giving it to value.
* Example: old value (1, 2, 3, 4, 5, \, \) capacity = 7
* reserve(9) -> new value (1, 2, 3, 4, 5, \, \, \, \) capacity = 9, size still 5
*/
template<class T>
void vector<T>::reserve(const size_t& newSize) {
	if (newSize <= v_capacity)
		return;

	T* newData = copyDynamic(value, v_size, newSize);
	freeRange(value, value + v_size);
	value = newData;
	v_capacity = newSize;
}

/*
Resizes the container to contain count elements
*/
template <class T>
void vector<T>::resize(const size_t& newSize) {
	if (newSize <= v_size) {
		destructRange(value + newSize, value + v_size);
		v_size = newSize;
		return;
	}

	if (newSize <= v_capacity) {
		destructRange(value + newSize, value + v_capacity);
		v_capacity = newSize;
		return;
	}

	T* newData = copyDynamic(value, v_size, newSize);
	constructRange(newData + v_size, newData + newSize);
	freeRange(value, value + v_size);

	value = newData;
	v_capacity = newSize;
	v_size = newSize;
}

template <class T>
void vector<T>::clear() noexcept {
	this->v_size = 0;
	this->v_capacity = 0;

	freeRange(value, value + v_size);

	value = nullptr;
}

/*Inserting an array of objects in
@param where -> The exact location of our list in the vector.
@param list -> the elements we would like to add.
@param countOfItems -> how many items would we like to add*/
template <class T>
void vector<T>::insertList(size_t _Where, const T* list, size_t countOfItems) {
	if (_Where > v_size)
		throw std::exception("Invalid index");

	if (v_size + countOfItems > v_capacity)
		reserve(v_size + countOfItems);

	/*Creating memory block capable of holding all the elements and copying the elements
	* before _Where in*/
	T* newData = copyDynamic(value, _Where, v_size + countOfItems);

	/*Getting the elements from list in:*/
	copyRange(list, list + countOfItems, newData + _Where);

	/*getting the rest of the items in*/
	copyRange(value + _Where, value + v_size, newData + _Where + countOfItems);

	freeRange(value, value + v_size);
	value = newData;
	v_size = v_size + countOfItems;
}

template <class T>
vector<T> vector<T>::subset(Iterator Beg, Iterator End) const {
	vector<int> v(Beg, End);
	return v;
}

template<class T>
void vector<T>::sort(int from, int to) {
	assert(from >= 0 && to >= 0 &&
		to - from < v_size && from <= to);

	qsort(from, to);
}

template<class T>
void vector<T>::sort() {
	sort(0, v_size - 1);
}

template <class T>
const vector<T>& vector<T>::operator+(const vector<T>& other) const {
	size_t newSize = v_size + other.v_size;
	size_t newCapacity = newSize;

	vector<T> toReturn;
	toReturn.insertList(0, value, v_size);
	toReturn.insertList(v_size, other.value, other.v_size);
	toReturn.v_size = newSize; toReturn.v_capacity = newSize;

	return toReturn;
}

template <class T>
const vector<T>& vector<T>::intersec(vector<T> other) const {
	vector<T> currentCopy(*this);
	currentCopy.sort();
	other.sort();

	size_t length = other.size() < currentCopy.size() ? other.size() : currentCopy.size();

	size_t fstIdx = 0;
	size_t sndIdx = 0;
	size_t resultIdx = 0;

	while (fstIdx < length && sndIdx < length) {
		if (currentCopy[fstIdx] == other[sndIdx]) {
			currentCopy[resultIdx++] = currentCopy[fstIdx];
			++fstIdx;
			++sndIdx;
		}
		else if (currentCopy[fstIdx] > other[sndIdx])
			++sndIdx;
		else
			++fstIdx;
	}

	currentCopy.resize(resultIdx);
	return currentCopy;
}

template<class T>
vector<T>& vector<T>::operator+=(const vector<T>& other) {
	reserve(v_size + other.v_size);
	copyRange(other.value, other.value + other.v_size, value + v_size);

	v_size = v_size + other.v_size;

	return *this;
}

template<class T>
void vector<T>::erase(Iterator at) {
	if (at == end())
		return;

	const auto& lastElem = --end();

	for (; at != lastElem; ++at)
		*at = *(++at);

	pop_back();
}

template<class T>
size_t vector<T>::capacity() const {
	return v_capacity;
}

template<class T>
void vector<T>::shrink_to_fit() {
	destructRange(value + v_size, value + v_capacity);
	v_capacity = v_size;
}

#endif // !VECTOR_HEADER

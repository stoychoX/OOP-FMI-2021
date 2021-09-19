//
//Written on 5/5/2021 by 82275 with code used in
//OOP practicum in FMI Sofia
//
#include<iostream>
const int DEFAULT_CAPACITY_SIZE = 8;
const int DEFAULT_GROWTH_CAPACITY = 2;
const int DEFAULT_PRIORITIES_COUNT = 2;


template<class T>
class Queue {
private:
	T* _val; //dynamic array containing the values of the queue
	size_t _capacity; //capacity of queue
	size_t _size; //current size of queue

	size_t head; //Where to get
	size_t tail; //Last added element

	void resize(const size_t& _Size); //resizing the queue when needed

	void copy(const Queue& toCpy);
	void free(); //destruct
public:
	//Default constructor:
	Queue();

	//Copy constructor:
	Queue(const Queue& toCpy);

	//Access the elements
	T& front();
	T& back();

	//Const access:
	T c_front() const;
	T c_back() const;

	void swap(const size_t first, const size_t second);

	//Adding and removing elements
	void enqueue(const T& toAdd);
	T dequeue();

	//Operator = and ==
	Queue& operator=(const Queue& toCopy);
	bool operator==(const Queue& toCmp) const;

	//Size related helpers:
	void shrink_to_fit();
	size_t size() const;
	size_t capacity() const;
	bool isEmpty() const;

	//Destroy:
	~Queue();
};

template<class T>
void Queue<T>::resize(const size_t& _Size) {
	T* newData = new T[_Size];

	for (size_t i = 0, ind = head; i < _size; i++, (++ind) %= _capacity)
		newData[i] = _val[ind];

	delete[] _val;
	_capacity = _Size;

	_val = newData;
	head = 0;
	tail = _size - 1;
}

template<class T>
inline T& Queue<T>::front() {
	return this->_val[head]; //returns a reference to the first element!
}

template<class T>
inline T& Queue<T>::back() {
	return this->_val[tail];
}

template<class T>
inline T Queue<T>::c_front() const {
	return this->_val[head];
}

template<class T>
inline T Queue<T>::c_back() const {
	return this->_val[tail];
}

template<class T>
inline void Queue<T>::swap(const size_t first, const size_t second) {
	if (first < 0 || second < 0 ||
		first > _size || second > _size)
		throw std::exception("Invalid index passed");
	T temp = this->_val[first];
	this->_val[first] = this->_val[second];
	this->_val[second] = temp;
}

template<class T>
inline void Queue<T>::copy(const Queue& toCpy) {
	_size = toCpy.size();
	_capacity = toCpy.capacity();
	this->tail = toCpy.tail;
	this->head = toCpy.head;

	_val = new T[_size];
	for (int i = 0; i < toCpy._size; i++)
		_val[i] = toCpy._val[i];
}

template<class T>
inline void Queue<T>::free() {
	delete[] this->_val;
}

template<class T>
Queue<T>::Queue() {
	_val = new T[DEFAULT_CAPACITY_SIZE];
	head = tail = _size = 0;
	_capacity = DEFAULT_CAPACITY_SIZE;
}

template<class T>
inline Queue<T>::Queue(const Queue& toCpy) {
	copy(toCpy);
}

template<class T>
void Queue<T>::enqueue(const T& toAdd) {
	if (_size == _capacity)
		resize(DEFAULT_GROWTH_CAPACITY * _capacity);

	if (_size)
		tail++;

	tail %= _capacity;

	_val[tail] = toAdd;
	_size++;
}

template<class T>
inline T Queue<T>::dequeue() {
	if (!_size)
		throw std::exception("Empty queue!");

	T toReturn = _val[head];
	(++head) %= _capacity;
	_size--;

	return toReturn;
}

template<class T>
inline Queue<T>& Queue<T>::operator=(const Queue<T>& toCopy) {
	if (this != &toCopy) {
		free();
		copy(toCopy);
	}
	return *this;
}

template<class T>
inline bool Queue<T>::operator==(const Queue& toCmp) const {
	bool eq = (this->_size == toCmp.size());

	if (this->isEmpty() && eq)
		return true;

	if (eq) {
		for (size_t i = 0; i < _size; i++)
			if (this->_val[i] != toCmp._val[i])
				return false;

		return true;
	}
	return false;
}

template<class T>
inline void Queue<T>::shrink_to_fit() {
	int newSize = (tail - head) + 1;

	if (newSize <= 0)
		throw std::exception("Fatal error in shrinking!");

	T* newData = new T[newSize];

	for (int i = 0, ind = head; i < newSize; i++, (++ind) %= _capacity)
		newData[i] = _val[ind];

	delete[] _val;
	_val = newData;
	_capacity = newSize;
	tail = newSize - 1;
	head = 0;
	_size = newSize;
}

template<class T>
inline size_t Queue<T>::size() const {
	return this->_size;
}

template<class T>
inline size_t Queue<T>::capacity() const {
	return this->_capacity;
}

template<class T>
inline bool Queue<T>::isEmpty() const {
	return (this->_size == 0);
}

template<class T>
inline Queue<T>::~Queue() {
	free();
}

template<class T>
class kPriorityQueue {
private:
	Queue<T>* vals; //A dynamic array of class queue, each one having its own priority
	size_t priorities; //Keeping track of priorities

	void copy(const kPriorityQueue<T>& other);
	void free();
	void enqueuePriv(const T& arg, const size_t& ind); //Used to hande exceptions
public:
	//constructors:
	kPriorityQueue();
	kPriorityQueue(const size_t& priorities);
	kPriorityQueue(const kPriorityQueue<T>& other);

	//operator=:
	kPriorityQueue<T>& operator=(const kPriorityQueue<T>& other);

	//accessing data:
	T& top(); //Returns reference to the top element in the priority queue.
	T c_top() const;

	//Push/Pop elements:
	T dequeue();
	void enqueue(const T& arg, const size_t& ind);

	//Destroy
	~kPriorityQueue();
};

template<class T>
inline void kPriorityQueue<T>::copy(const kPriorityQueue<T>& other) {
	this->vals = new Queue<T>[other.priorities];
	for (int i = 0; i < other.priorities; i++)
		this->vals[i] = other.vals[i];
	this->priorities = other.priorities;
}

template<class T>
inline void kPriorityQueue<T>::free() {
	delete[] vals;
}

template<class T>
inline T kPriorityQueue<T>::dequeue() {
	int cnt = this->priorities - 1;

	while (cnt >= 0) {
		try {
			return vals[cnt].dequeue();
		}
		catch (const std::exception&) {}
		cnt--;
	}

	throw std::exception("Empty pQueue");
}

template<class T>
inline void kPriorityQueue<T>::enqueue(const T& arg, const size_t& ind) {
	try {
		enqueuePriv(arg, ind);
	}
	catch (const std::exception& exp) { std::cout << "error! No such priority!"; }
}

template<class T>
inline kPriorityQueue<T>::kPriorityQueue() {
	this->vals = new Queue<T>[DEFAULT_PRIORITIES_COUNT]();
	this->priorities = DEFAULT_PRIORITIES_COUNT;
}

template<class T>
inline kPriorityQueue<T>::kPriorityQueue(const size_t& priorities) {
	vals = new Queue<T>[priorities]();
	this->priorities = priorities;
}

template<class T>
inline kPriorityQueue<T>::kPriorityQueue(const kPriorityQueue<T>& other) {
	copy(other);
}

template<class T>
inline kPriorityQueue<T>& kPriorityQueue<T>::operator=(const kPriorityQueue<T>& other)
{
	if (this != &other) {
		free();
		copy(other);
	}

	return *this;
}

template<class T>
inline T& kPriorityQueue<T>::top() {
	return this->vals[priorities - 1].front();
}

template<class T>
inline T kPriorityQueue<T>::c_top() const {
	return this->vals[priorities - 1].c_front();
}

template<class T>
inline kPriorityQueue<T>::~kPriorityQueue() {
	free();
}

template<class T>
inline void kPriorityQueue<T>::enqueuePriv(const T& arg, const size_t& ind) {
	if (ind < 0 || ind > priorities)
		throw std::exception("error! No such priority.");

	vals[ind].enqueue(arg);
}

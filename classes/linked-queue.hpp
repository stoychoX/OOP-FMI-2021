/****************************************
Linked queue implementation.

19.09.2021
******************************************/

#include<exception>
#ifndef LINKED_QUEUE
#define LINKDE_QUEUE

#define MAXSIZE 100

template<class T>
struct Node {
	T data;
	Node* next;

	Node(const T& arg, Node* next = nullptr) :
		data(arg),
		next(next) {}
};

template<class T>
class queue {
private:
	Node<T>* head;
	Node<T>* tail;

	int size;
public:
	//construct:
	queue();
	queue(const queue<T>& other);
	queue(queue<T>&& other);

	//operator=
	queue<T>& operator=(const queue<T>& other);
	queue<T>& operator=(queue<T>&& other);

	//funcs
	void enqueue(const T& arg);
	T dequeue();
	const T& top() const;

	//indicators full/empty
	bool isEmpty() const;
	bool isFull() const;

	void clear();

	//destruct:
	~queue();
private:
	//helpers
	void free();
	void copy(const queue<T>& other);
};

#endif //!LINKED_QUEUE

template<class T>
inline queue<T>::queue() : head(nullptr), tail(nullptr), size(0) {}

template<class T>
inline queue<T>::queue(const queue<T>& other) {
	copy(other);
}

template<class T>
inline queue<T>::queue(queue<T>&& other) {
	this->head = other.head;
	this->tail = other.tail;
	this->size = other.size;

	other.head = other.tail = nullptr;
	other.size = 0;
}

template<class T>
inline queue<T>& queue<T>::operator=(const queue<T>& other) {
	if (this != &other) {
		free();
		copy(other);
	}

	return *this;
}

template<class T>
inline queue<T>& queue<T>::operator=(queue<T>&& other) {
	clear();

	this->head = other.head;
	this->tail = other.tail;
	this->size = other.size;

	other.head = other.tail = nullptr;
	other.size = 0;

	return *this;
}

template<class T>
inline void queue<T>::enqueue(const T& arg) {
	//we add at tail
	if (isFull())
		throw std::domain_error("Queue is full");

	Node<T>* toAdd = new Node<T>(arg);

	if (isEmpty()) {
		head = toAdd;
		tail = toAdd;
	}
	else {
		tail->next = toAdd;
		tail = toAdd;
	}
	++size;
}

template<class T>
inline T queue<T>::dequeue() {
	//we remove at head
	if (isEmpty())
		throw std::domain_error("Queue is empty");

	Node<T>* toRemove = head;
	head = head->next;
	T toReturn = toRemove->data;
	delete toRemove;
	--size;

	if (isEmpty())
		tail = nullptr;

	return toReturn;
}

template<class T>
inline const T& queue<T>::top() const {
	if (isEmpty())
		throw std::domain_error("Queue is empty");

	return head->data;
}

template<class T>
inline bool queue<T>::isEmpty() const {
	return (size == 0);
}

template<class T>
inline bool queue<T>::isFull() const {
	return (size == MAXSIZE);
}

template<class T>
inline void queue<T>::clear() {
	free();

	tail = head = nullptr;
	size = 0;
}

template<class T>
inline queue<T>::~queue() {
	free();
}

template<class T>
inline void queue<T>::free() {
	while (head) {
		Node<T>* toRemove = head;
		head = head->next;
		delete toRemove;
	}
}

template<class T>
inline void queue<T>::copy(const queue<T>& other) {
	try {
		Node<T>* iterator = other.head;

		while (iterator) {
			T arg = iterator->data;
			this->enqueue(arg);
			iterator = iterator->next;
		}
	}
	catch (...) {
		clear();
		throw;
	}
}

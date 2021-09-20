/********************************************
LINKED LIST IMPLEMENTATION &&
LINKED LIST ITERATOR IMPLEMENTATION

20.9.2021
*********************************************/

#ifndef LINKED_LIST
#define LINKED_LIST
#include<exception>

template<class T>
struct Node {
	T data;
	Node* next;

	Node(const T& arg, Node* next = nullptr) :
		data(arg), next(next) {}
};

template<class T>
class linked_list_iterator {
private:
	Node<T>* curr;

public:
	linked_list_iterator(Node<T>* arg);

	linked_list_iterator& operator++();
	linked_list_iterator& operator++(int);

	bool operator !=(const linked_list_iterator<T>& other);

	bool operator >(const linked_list_iterator<T>& other);
	bool operator <(const linked_list_iterator<T>& other);

	bool operator >=(const linked_list_iterator<T>& other);
	bool operator <=(const linked_list_iterator<T>& other);

	T& operator*() const;
};

template<class T>
class linked_list {
private:
	Node<T>* first;
	Node<T>* last;
	int size;

public:
	//construct:
	linked_list();
	linked_list(const linked_list<T>& other);
	linked_list(linked_list<T>&& other);

	//operator=
	linked_list<T>& operator=(const linked_list<T>& other);
	linked_list<T>& operator=(linked_list<T>&& other);

	//push:
	void push_at_front(const T& arg);
	void push_at_end(const T& arg);
	void push_at(const T& arg, unsigned index);

	//begin and end:
	linked_list_iterator<T> begin() const;
	linked_list_iterator<T> end() const;
	
	//extract:
	T at(int index) const;
	T front() const;
	T back() const;

	//remove:
	T remove_at(unsigned index);
	T remove_at_front();
	T remove_at_end();

	void clear();

	bool isEmpty() const;

	~linked_list();
private:
	void free();
	void copy(const linked_list<T>& other);
};

#endif //!LINKED_LIST

template<class T>
inline linked_list<T>::linked_list() : first(nullptr), last(nullptr), size(0) {}


template<class T>
inline linked_list<T>::linked_list(const linked_list<T>& other) {
	copy(other);
}

template<class T>
inline linked_list<T>::linked_list(linked_list<T>&& other) {
	this->first = other.first;
	this->last = other.last;
	this->size = other.size;

	other.first = other.last = nullptr;
	other.size = 0;
}

template<class T>
inline linked_list<T>& linked_list<T>::operator=(const linked_list<T>& other) {
	if (this != &other) {
		free();
		copy(other);
	}
	return *this;
}

template<class T>
inline linked_list<T>& linked_list<T>::operator=(linked_list<T>&& other) {
	if (this != &other) {
		this->first = other.first;
		this->last = other.last;
		this->size = other.size;

		other.first = other.last = nullptr;
		other.size = 0;
	}
	return *this;
}

template<class T>
linked_list<T>::~linked_list() {
	free();
}

template<class T>
inline void linked_list<T>::push_at_front(const T& arg) {
	push_at(arg, 0);
}

template<class T>
inline void linked_list<T>::push_at_end(const T& arg) {
	push_at(arg, size);
}

//whole logic of inserion is collected at one place:
template<class T>
inline void linked_list<T>::push_at(const T& arg, unsigned index) {
	if (index > size)
		throw std::out_of_range("Index out of range!");

	if (!index) {
		first = new Node<T>(arg, first);
		if (!size)
			last = first;
	}
	else if (index == size) {
		last->next = new Node<T>(arg);
		last = last->next;
	}
	else {
		Node<T>* it = first;
		for (int i = 0; i < index - 1; i++)
			it = it->next;
		it->next = new Node<T>(arg, it->next);
	}
	++size;
}

template<class T>
inline linked_list_iterator<T> linked_list<T>::begin() const {
	return linked_list_iterator<T>(first);
}

template<class T>
inline linked_list_iterator<T> linked_list<T>::end() const {
	return linked_list_iterator<T>(nullptr);
}

template<class T>
inline T linked_list<T>::at(int index) const {
	if (isEmpty() || index > size)
		throw std::out_of_range("Invalid index");

	if (index == size)
		return last->data;

	Node<T>* it = first;
	for (int i = 0; i < index; i++)
		it = it->next;
	return it->data;
}

template<class T>
inline T linked_list<T>::front() const {
	return at(0);
}

template<class T>
inline T linked_list<T>::back() const {
	return at(size);
}

template<class T>
inline T linked_list<T>::remove_at(unsigned index) {
	if (index > size || isEmpty())
		throw std::out_of_range("Invalid index");

	if (!index) {
		Node<T>* toRemove = first;
		T toReturn = toRemove->data;

		first = first->next;

		delete toRemove;
		--size;
		return toReturn;
	}
	else {
		Node<T>* it = first;
		for (int i = 0; i < index - 1; i++)
			it = it->next;

		Node<T>* toRemove = it->next;
		T toreturn = toRemove->data;
		it->next = it->next->next;

		delete toRemove;
		--size;
		return toreturn;
	}
}

template<class T>
inline T linked_list<T>::remove_at_front() {
	return remove_at(0);
}

template<class T>
inline T linked_list<T>::remove_at_end() {
	return remove_at(size - 1);
}

template<class T>
inline void linked_list<T>::clear() {
	free();

	first = last = nullptr;
	size = 0;
}

template<class T>
inline bool linked_list<T>::isEmpty() const {
	return (first == nullptr);
}

template<class T>
inline void linked_list<T>::free() {
	while (first) {
		Node<T>* toDel = first;
		first = first->next;
		delete toDel;
	}
}

template<class T>
inline void linked_list<T>::copy(const linked_list<T>& other) {
	Node<T>* it = other.first;
	try {
		while (it) {
			T arg = it->data; //might throw
			this->push_at_end(arg);
			it = it->next;
		}
		this->size = other.size;
	}
	catch (...) {
		clear();
		throw;
	}
}

template<class T>
inline linked_list_iterator<T>::linked_list_iterator(Node<T>* arg) {
	this->curr = arg;
}

template<class T>
inline linked_list_iterator<T>& linked_list_iterator<T>::operator++() {
	if (curr) //nullptr corner case
		curr = curr->next;

	return *this;
}

template<class T>
inline linked_list_iterator<T>& linked_list_iterator<T>::operator++(int) {
	linked_list_iterator<T> toreturn(*this);
	++(*this);
	return toreturn;
}

template<class T>
inline bool linked_list_iterator<T>::operator!=(const linked_list_iterator<T>& other) {
	return (this->curr != other.curr);
}

template<class T>
inline bool linked_list_iterator<T>::operator>(const linked_list_iterator<T>& other) {
	return (this->curr > other.curr);
}

template<class T>
inline bool linked_list_iterator<T>::operator<(const linked_list_iterator<T>& other) {
	return this->curr < other.curr;
}

template<class T>
inline bool linked_list_iterator<T>::operator>=(const linked_list_iterator<T>& other) {
	return !(this < &other);
}

template<class T>
inline bool linked_list_iterator<T>::operator<=(const linked_list_iterator<T>& other) {
	return !(this > &other);
}

template<class T>
inline T& linked_list_iterator<T>::operator*() const {
	return curr->data;
}

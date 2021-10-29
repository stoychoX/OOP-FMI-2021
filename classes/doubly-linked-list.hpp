/******************************
Doubly Linked List implementation
Doubly Linked List iterator implementation
29.10.2021
*******************************/

#ifndef DOUBLY_LINKED_LIST_HEADER
#define DOUBLY_LINKED_LIST_HEADER

#include<exception>

template <class T>
struct doublyLinkedNode {
	T data;
	doublyLinkedNode<T>* prev;
	doublyLinkedNode<T>* next;

	doublyLinkedNode(const T& argument, doublyLinkedNode<T>* ptrNext = nullptr, doublyLinkedNode<T>* ptrPerv = nullptr) : data(argument), prev(ptrPerv), next(ptrNext) {}
};


template<typename T>
class doublyLinkedListIterator {
	using Iterator = doublyLinkedListIterator;

private:
	doublyLinkedNode<T>* current;
	
public:
	doublyLinkedListIterator(doublyLinkedNode<T>* arg) : current(arg) {}
	
	Iterator& operator++() {
		if (current->next == nullptr) {
			return *this;
		}

		current = current->next;
		return *this;
	}

	Iterator& operator++(int) {
		if (current->next == nullptr) {
			return *this;
		}
		Iterator it = *this;
		++(*this);
		return it;
	}

	Iterator& operator--() {
		if (current->prev == nullptr) {
			return *this;
		}

		current = current->prev;
		return *this;
	}

	Iterator& operator--(int) {
		if (current->prev == nullptr) {
			return *this;
		}
		Iterator it = *this;
		--(*this);
		return it;
	}

	T& operator*() {
		return current->data;
	}

	T* operator->() {
		return &current->data;
	}

	bool operator==(const Iterator& other) const  {
		return (current == other.current);
	}

	bool operator!=(const Iterator& other)const {
		return !(other == *this);
	}
};


template <class T>
class doublyLinkedList {
private:
	doublyLinkedNode<T>* head;
	doublyLinkedNode<T>* tail;
	int size;

	void copyFrom(const doublyLinkedList<T>& other);
	void free();

public:
	//construct:
	doublyLinkedList();
	doublyLinkedList(const doublyLinkedList<T>& other);
	doublyLinkedList(doublyLinkedList<T>&& other);

	//operator=
	doublyLinkedList<T>& operator=(const doublyLinkedList<T>& other);
	doublyLinkedList<T>& operator=(doublyLinkedList<T>&& other);

	//push operations:
	void pushAt(const T& arg, int index);
	void pushBack(const T& arg);
	void pushFront(const T& arg);

	//pop operations:
	void popAt(int index);
	void popBack();
	void popFront();

	//extract operations:
	const T& back() const;
	const T& front() const;
	T back();
	T front();
	
	//no at(index);

	//helpers
	int length() const;
	bool isEmpty() const;

	//iterate:
	doublyLinkedListIterator<T> begin() {
		return doublyLinkedListIterator<T>(tail);
	}

	doublyLinkedListIterator<T> end() {
		return doublyLinkedListIterator<T>(head);
	}

	~doublyLinkedList();
};

template<class T>
doublyLinkedList<T>::doublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

template<class T>
doublyLinkedList<T>::doublyLinkedList(const doublyLinkedList<T>& other) {
	copyFrom(other);
}

template<class T>
doublyLinkedList<T>::doublyLinkedList(doublyLinkedList<T>&& other) {
	this->head = other.head;
	this->tail = other.tail;
	this->size = other.size;

	other.tail = other.head = nullptr;
	other.size = 0;
}

template<class T>
doublyLinkedList<T>& doublyLinkedList<T>::operator=(const doublyLinkedList<T>& other) {
	if (this != &other) {
		free();
		copy(other);
	}
	return *this;
}

template<class T>
doublyLinkedList<T>& doublyLinkedList<T>::operator=(doublyLinkedList<T>&& other) {
	if (this != &other) {
		free();

		this->head = other.head;
		this->tail = other.tail;
		this->size = other.size;

		other.tail = other.head = nullptr;
		other.size = 0;
	}
	return *this;
}

template<class T>
void doublyLinkedList<T>::free() {
	doublyLinkedNode<T>* it = tail;
	while (it) {
		doublyLinkedNode<T>* toDelete = it;
		it = it->next;
		delete[] toDelete;
	}
	size = 0;
}

template<class T>
void doublyLinkedList<T>::copyFrom(const doublyLinkedList<T>& other) {
	//TODO EXCEPTION SAFETY??
	doublyLinkedNode<T>* it = other.tail;
	while (it) {
		this->pushBack(it->data);
		it = it->next;
	}
}

template<class T>
void doublyLinkedList<T>::pushAt(const T& arg, int index) {
	if (index < 0 || index > size) {
		throw std::out_of_range("Invalid index");
	}

	doublyLinkedNode<T>* toAdd = new doublyLinkedNode<T>(arg);

	if (index == 0) { //push back = tail
		if (tail == nullptr) {
			tail = head = toAdd;
		}
		else {
			toAdd->next = tail;
			tail->prev = toAdd;
			tail = toAdd;
		}
	}
	else if (index == size) { //push front = head
		head->next = toAdd;
		toAdd->prev = head;
		head = toAdd;
	}
	else {
		doublyLinkedNode<T>* it = tail;
		for (int i = 0; i < index - 1; i++)
			it = it->next;

		it->next->prev = toAdd;
		toAdd->next = it->next;
		toAdd->prev = it;
		it->next = toAdd;

	}
	++size;
}

template<class T>
void doublyLinkedList<T>::popAt(int index) {
	if (index < 0 || index > size) {
		throw std::out_of_range("Invalid index");
	}

	if (isEmpty()) {
		throw std::exception("Empty list");
	}

	doublyLinkedNode<T>* toRemove;

	if (index == 0) {
		if (head == tail) {
			toRemove = head;
			head = tail = nullptr;
		}
		else {
			toRemove = tail;
			tail = tail->next;
			tail->prev = nullptr;
		}
	}
	else if (index == size) {
		toRemove = head;
		head = head->prev;
		head->next = nullptr;
	}
	else {
		doublyLinkedNode<T>* it = tail;
		for (int i = 0; i < index; i++)
			it = it->next;

		toRemove = it;
		doublyLinkedNode<T>* prev = it->prev;
		it->prev->next = it->next;
		it->next->prev = prev;
	}
	delete[] toRemove;
	--size;
}

template<class T>
void doublyLinkedList<T>::pushBack(const T& arg) {
	pushAt(arg, 0);
}

template<class T>
void doublyLinkedList<T>::pushFront(const T& arg) {
	pushAt(arg, size);
}

template<class T>
void doublyLinkedList<T>::popFront() {
	popAt(size);
}

template<class T>
void doublyLinkedList<T>::popBack() {
	popAt(0);
}

template<class T>
const T& doublyLinkedList<T>::back() const {
	return tail->data;
}

template<class T>
const T& doublyLinkedList<T>::front() const {
	return head->data;
}

template<class T>
int doublyLinkedList<T>::length() const {
	return this->size;
}

template<class T>
bool doublyLinkedList<T>::isEmpty() const {
	return !size; //nullptr
}

template<class T>
T doublyLinkedList<T>::front() {
	return head->data;
}

template<class T>
T doublyLinkedList<T>::back() {
	return tail->data;
}

template<class T>
doublyLinkedList<T>::~doublyLinkedList() {
	free();
}

#endif // !DOUBLY-LINKED-LIST-HEADER

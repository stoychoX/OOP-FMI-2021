/*****************************
Linked stack implementation.
18 september 2021.
******************************/
#include<exception>

#ifndef LINKED_STACK
#define LINKED_STACK

#define MAX_SIZE 100

template<class T>
struct Node {
	T data;
	Node* next;

	Node(T arg, Node* next = nullptr) :
		data(arg),
		next(next) {}
};

template<class T>
class stack {
private:
	Node<T>* tos;
	int size;

public:
	//constructors:
	stack();
	stack(const stack<T>& other);
	stack(stack<T>&& other);

	//operator=
	stack<T>& operator=(const stack<T>& other);
	stack<T>& operator=(stack<T>&& other);

	//functions:
	void push(const T& arg);
	const T& pop();
	const T& top() const;

	//indicators full/empty
	bool isFull() const;
	bool isEmpty() const;

	//destructor:
	~stack();

private:
	//helper functions:
	void copy(const stack<T>& other);
	void free();
};

#endif // !LINKED_STACK

template<class T>
inline stack<T>::stack() : tos(nullptr), size(0) {}

template<class T>
inline stack<T>::stack(const stack<T>& other) {
	copy(other);
}

template<class T>
inline stack<T>::stack(stack<T>&& other) {
	this->tos = other.tos;
	this->size = other.size;

	other.tos = nullptr;
	other.size = 0;
}

template<class T>
inline stack<T>& stack<T>::operator=(const stack<T>& other) {
	if (this != &other) {
		free();
		copy(other);
	}
	return *this;
}

template<class T>
inline stack<T>& stack<T>::operator=(stack<T>&& other) {
	if(this != &other){
	this->tos = other.tos;
	this->size = other.size;
	
	other.tos = nullptr;
	other.size = 0;
	}
	return *this;
}

template<class T>
inline void stack<T>::push(const T& arg) {
	if (this->isFull())
		throw std::domain_error("Stack is full!");

	Node<T>* n = new Node<T>(arg);
	n->next = tos;
	tos = n;
	++size;
}

template<class T>
inline const T& stack<T>::pop() {
	if (this->isEmpty())
		throw std::domain_error("Stack is empty!");

	Node<T>* temp = tos;
	tos = tos->next;
	const T toReturn = temp->data;
	delete temp;
	--size;
	return toReturn;
}

template<class T>
inline const T& stack<T>::top() const {
	if (isEmpty())
		throw std::exception("Empty stack");
	return tos->data;
}

template<class T>
inline bool stack<T>::isFull() const {
	return (size == MAX_SIZE);
}

template<class T>
inline bool stack<T>::isEmpty() const {
	return (size == 0);
}

template<class T>
inline stack<T>::~stack() {
	free();
}

template<class T>
inline void stack<T>::copy(const stack<T>& other) {
	this->tos = nullptr;

	T* arr = new T[other.size];
	Node<T>* it = other.tos;
	for (int i = 0; i < other.size; i++) {
		arr[i] = it->data;
		it = it->next;
	}

	for (int i = other.size - 1; i >= 0; --i)
		this->push(arr[i]);
	
	delete[] arr;
}

template<class T>
inline void stack<T>::free() {
	while (tos) {
		Node<T>* temp = tos;
		tos = tos->next;
		delete temp;
	}

	size = 0;
}

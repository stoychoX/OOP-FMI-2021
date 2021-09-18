/*****************************
Stack implementation.
18.09.2021
******************************/

#ifndef STACK_HPP
#define STACK_HPP
#include<exception>

#define MAXSIZE 100

template<class T>
class stack {
private:
	int tos;
	T* data;
	int size_of_stack;

public:
	//construct
	stack();
	stack(const stack<T>& other);
	stack(stack<T>&& other);

	//operator= 
	stack& operator=(const stack& other);
	stack& operator=(stack&& other);

	void push(const T& arg);
	T& pop();

	int size() const; 
	void clear();

	const T& top() const;

	//indicators:
	bool isFull() const;
	bool isEmpty() const;

	//destruct:
	~stack();
private:
	//helpers:
	void copy(const stack<T>& other);
	void free();
};

#endif // !STACK_HPP

template<class T>
inline stack<T>::stack() : size_of_stack(MAXSIZE), tos(0) {
	this->data = new T[size_of_stack];
}

//copy constructor:
template<class T>
inline stack<T>::stack(const stack<T>& other) {
	copy(other);
}

template<class T>
inline stack<T>::stack(stack<T>&& other) {
	this->data = other.data;
	this->size_of_stack = other.size_of_stack;
	this->tos = other.tos;

	other.data = nullptr;
	other.tos = other.size_of_stack = 0;
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
	this->data = other.data;
	this->tos = other.tos;
	this->size_of_stack = other.size_of_stack;

	other.data = nullptr;
	other.size_of_stack = other.tos = 0;

	return *this;
}

template<class T>
inline void stack<T>::push(const T& arg) {
	if (isFull())
		throw std::domain_error("Stack is full");
	else
		this->data[tos++] = arg;
}

template<class T>
inline bool stack<T>::isFull() const {
	return (this->tos == this->size_of_stack - 1);
}

template<class T>
inline bool stack<T>::isEmpty() const {
	return (this->tos == 0);
}

template<class T>
inline stack<T>::~stack() {
	free();
}

template<class T>
inline void stack<T>::copy(const stack<T>& other) {
	this->data = new T[other.size_of_stack];
	this->size_of_stack = other.size_of_stack;
	this->tos = other.tos;

	try {
		for (int i = 0; i < other.tos; i++)
			this->data[i] = other.data[i];
	}
	catch (...) {
		free();
		throw;
	}
}

template<class T>
inline void stack<T>::free() {
	delete[] this->data;
}

template<class T>
T& stack<T>::pop() {
	if (this->isEmpty())
		throw std::domain_error("stack is empty");

	return this->data[--tos];
}

template<class T>
int stack<T>::size() const {
	return this->tos;
}

template<class T>
inline void stack<T>::clear() {
	free();

	this->data = new T[this->size_of_stack];
	tos = 0;
}

template<class T>
inline const T& stack<T>::top() const {
	if (isEmpty())
		throw std::domain_error("Empty stack");

	return this->data[tos - 1];
}

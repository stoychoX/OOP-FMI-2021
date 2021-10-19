/*****************************
* Linked stack custom implementation.
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

	int Size() const;//Size instead of size to avoid ambiguous "size" 

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
	this->tos = other.tos;
	this->size = other.size;

	other.tos = nullptr;
	other.size = 0;
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
	return !size;
}

template<class T>
inline int stack<T>::Size() const {
	return this->size;
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
	/*
	this->tos = nullptr;
	stack<T> helper;
	Node<T>* it = other.tos;
	while (it) {
		helper.push(it->data);
		it = it->next;
	}

	while (!helper.isEmpty())
		this->push(helper.pop());
	*/
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

//В по - горната имплементация закоментираната част на функцията copy не работи адекватно (поне при мен, в другата имплементация не дава грешка).
// Възможно е и да има други грешки, нямах много време да го тествам.

/*************************************
*Implementation with added move semantics
***************************************/

#pragma once
#include<iostream>
#include<cassert>

template <typename T>
struct StackElement {
    T data;
    StackElement<T>* next;
};

template <typename T>
class LinkedStack {
    // представяне
    StackElement<T>* topPtr; // указател към връх на стека

    void copy(LinkedStack const& ls);

    void erase();

    void move(LinkedStack&& other);

public:
    // създаване на празен стек
    LinkedStack();

    // конструктор за копиране
    LinkedStack(LinkedStack const& ls);
    LinkedStack(LinkedStack&& other);

    // операция за присвояване
    LinkedStack& operator=(LinkedStack const& ls);
    LinkedStack& operator=(LinkedStack&& other);

    // деструктор
    ~LinkedStack();

    // move -> added

    // проверка за празнота на стек
    bool empty() const;

    // включване на елемент 
    void push(T const& x);

    // изключване на елемент
    T pop();

    // извличане последно включения елемент
    T const& top() const;

    // извличане последно включения елемент + възможност за неговата промяната
    T& top();
};

// Създаване на празен стек
template <typename T>
LinkedStack<T>::LinkedStack() {
    topPtr = nullptr;
}

// Проверка за празен стек
template <typename T>
bool LinkedStack<T>::empty() const {
    return topPtr == nullptr;
}

// Копиране на стек 
// Вариант 3 - чрез използване на допълнителен стек
template <typename T>
void LinkedStack<T>::copy(LinkedStack<T> const& ls) {
    topPtr = nullptr;

    StackElement<T>* p = ls.topPtr;
    LinkedStack<T> tmp;
    while (p != nullptr) {
        tmp.push(p->data);
        p = p->next;
    }

    while (!tmp.empty())
        push(tmp.pop());
}

// Изтриване на паметта за стека
template <typename T>
void LinkedStack<T>::erase() {
    // !!! delete topPtr;
    while (!empty()) {
        pop();
    }
}

template<typename T>
inline void LinkedStack<T>::move(LinkedStack<T>&& other) {
    this->topPtr = other.topPtr;
    other.topPtr = nullptr; //empty() is defined with nullptr, assertion safe
}

// Конструктор за копиране
template <typename T>
LinkedStack<T>::LinkedStack(LinkedStack<T> const& ls) {
    copy(ls);
}

template<typename T>
inline LinkedStack<T>::LinkedStack(LinkedStack&& other) {
    move(other);
}

// Деструктор
template <typename T>
LinkedStack<T>::~LinkedStack() {
    erase();
}

// Оператор за присвояване между два стека
template <typename T>
LinkedStack<T>& LinkedStack<T>::operator=(LinkedStack<T> const& ls) {
    if (this != &ls) {
        erase();
        copy(ls);
    }
    return *this;
}

template<typename T>
inline LinkedStack<T>& LinkedStack<T>::operator=(LinkedStack<T>&& other)
{
    if (this != &other) {
        move(other);
    }
    return *this;
}

// Добавяне на нов елемент
template <typename T>
void LinkedStack<T>::push(T const& x) {
    // Заделяне на памет за нов елемент
    StackElement<T>* newElemPtr = new StackElement<T>;

    // Записване на стойност за този нов елемент
    newElemPtr->data = x;

    // Създаване на връзка между новия елемент и стария връх на стека (вкл. nullptr)
    newElemPtr->next = topPtr;

    // Пренасочване на върха на стека към новия елемент
    topPtr = newElemPtr;
}

// Изтриване на елемента на върха на стека
template <typename T>
T LinkedStack<T>::pop() {
    assert(!empty());

    T result = top();

    // Насочване на временен указател към текущия връх на стека, който следва да бъде изтрит
    StackElement<T>* tempElemPtr = topPtr;

    // Насочване на върха на стека към предходния елемент
    topPtr = topPtr->next;

    // Изтриване на върха на стека
    delete tempElemPtr;

    return result;
}

template <typename T>
T const& LinkedStack<T>::top() const {
    assert(!empty());

    return topPtr->data;
}

template <typename T>
T& LinkedStack<T>::top() {
    assert(!empty());

    return topPtr->data;
}
/*****************************
Static stack implementation.
18.09.2021

quick update: 20.09.2022
******************************/

#ifndef STATIC_STACK_HPP
#define STATIC_STACK_HPP

#include<exception>
#include<cassert>

template<class T, unsigned stackSize = 100>
class stack {
private:
	T data[stackSize];
	size_t tos;
public:
	stack();

	void push(const T& arg);
	const T& pop();

	size_t size() const;
	size_t capacity() const;

	const T& top() const;

	bool isFull() const;
	bool isEmpty() const;
};

#endif // !STACK_HPP

template<class T, unsigned stackSize>
inline stack<T, stackSize>::stack() : tos{ 0 }, data{} {}

template<class T, unsigned stackSize>
inline void stack<T, stackSize>::push(const T& arg) {
	if (isFull())
		throw std::exception("Stack is full");

	this->data[tos++] = arg;
}

template<class T, unsigned stackSize>
inline bool stack<T, stackSize>::isFull() const {
	return tos == stackSize;
}

template<class T, unsigned stackSize>
inline bool stack<T, stackSize>::isEmpty() const {
	return tos == 0;
}

template<class T, unsigned stackSize>
const T& stack<T, stackSize>::pop() {
	if (isEmpty())
		throw std::exception("Stack is empty");

	return data[--tos];
}

template<class T, unsigned stackSize>
size_t stack<T, stackSize>::size() const {
	return tos;
}

template<class T, unsigned stackSize>
size_t stack<T, stackSize>::capacity() const {
	return stackSize;
}

template<class T, unsigned stackSize>
inline const T& stack<T, stackSize>::top() const {
	if (isEmpty())
		throw std::exception("Empty stack");

	return data[tos - 1];
}

void runTests() {
	stack<int> s;

	for (size_t i = 0; i < s.capacity(); i++)
		s.push(i);

	assert(s.isFull());

	try { s.push(0); assert(false); }
	catch (...) {}

	for (size_t i = 0; i < s.capacity(); i++) {
		int curr = s.top();
		s.pop();
		assert(curr == s.capacity() - i - 1);
	}

	assert(s.isEmpty());

	if (s.capacity() < 10)
		return;

	for (size_t i = 0; i < 10; i++)
		s.push(i);

	assert(s.size() == 10);
	
	while (!s.isEmpty()) { s.pop(); }

	try { s.pop(); assert(false); }
	catch (...) {}

	try { s.top(); assert(false); }
	catch (...) {}
}

int main() {
	runTests();
}

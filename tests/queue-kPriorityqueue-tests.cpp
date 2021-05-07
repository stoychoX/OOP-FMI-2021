//
//A .cpp file containing tests
//of Queue and kPriorityQueue
//
#include<iostream>
#include"Queue.hpp"

#define end "\n\n"

int main() {
	std::cout << "==========QUEUE==========\n";
	std::cout << "Creating empty queue with int values q1...\n" << end;
	Queue<int> q1;

	std::cout << "Adding numbers from 1 to 10 in..." << end;
	for (int i = 0; i < 10; i++)
		q1.enqueue(i + 1);
	std::cout << "Front: " << q1.c_front() << " back: " << q1.c_back() << end;
	std::cout << "Popping two elements from q1..." << end;
	q1.dequeue();
	q1.dequeue();
	std::cout << "Front: " << q1.c_front() << " back: " << q1.c_back() << end;
	std::cout << "Creating qCpy, copy of q1: " << end;
	Queue<int> qCpy(q1);
	std::cout << "Trying == on q1 and qCpy: " << (q1 == qCpy) << end;
	std::cout << "Popping from qCpy and trying == again: ";
	qCpy.dequeue();
	std::cout << (q1 == qCpy) << end;

	std::cout << "==========KPRIORITYQUEUE==========\n";
	std::cout << "Creating kPriorityQueue containing elements of type int k1, having 3 priorities..." << end;
	kPriorityQueue<int> k1(3);
	std::cout << "Adding the numbers from 1 to 10 with priority ind % 3\n" << end;
	for (int i = 0; i < 10; i++)
		k1.enqueue(i + 1, (i + 1) % 3);

	std::cout << "Top: " << k1.c_top() << end;
	std::cout << "Popping each element:\n";
	for (int i = 0; i < 10; i++)
		std::cout << k1.dequeue()<<" ";
	std::cout << "\nFirst popping elements with mod 2, then with mod 1 and last popped are the elements divisible by 3" << end;
}

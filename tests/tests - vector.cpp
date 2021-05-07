/*A .cpp file where i will test
every method of my vector.
All of the tests are made with int,
but would work with every suitable
data type.*/
#include<iostream>
#include"vector.h"

#define _end "\n\n"

int main() {
	std::cout << "==========TESTS OF VECTOR CLASS==========\n";

	//Default construction of vector:
	std::cout << "Creating empty vect1...\n";
	vector<int> vect1;

	//Constructing vector by passing it size:
	std::cout << "\nCreating vect2 with capacity 100...\n";
	vector<int> vect2(100);

	//Constructing vector by passing it size and T*:
	std::cout << "\nCreating vect3 with 10 elements with values 10:\n";
	vector<int> vect3(10, 10);
	std::cout << "vect3: " << vect3 << _end; //10 10 10 ... 10;

	//Creating vector by copying other vector data:
	std::cout << "Creating vectCopy, copy of vect3:\n";
	vector<int> vectCopy(vect3);
	std::cout << "vectCopy: " << vectCopy << _end; //10 10 10 ... 10;

	//Creating vector by moving another vector data:
	std::cout << "Creating vectMove, moving vect3: \n";
	vector<int> vectMove(std::move(vect3));
	std::cout << vectMove << '\n'; //10 10 10 ... 10;
	try { std::cout << vect3[0] << '\n'; }
	catch (const std::exception&) { std::cout << "Vect 3 moved!\n\n"; }

	//Testing operator=
	std::cout << "Making vect3 equal to vectCopy... \n";
	vect3 = vectCopy;
	std::cout << "Vect 3: " << vect3 << _end;

	//Testing move semantics of operator=
	std::cout << "Moving vect3 in vect2 via operator=(vector&&)\n";
	vect2 = std::move(vect3);
	std::cout << "Vect 2: " << vect2 << '\n';
	try { std::cout << vect3[0] << '\n'; }
	catch (const std::exception&) { std::cout << "Vect 3 moved\n\n"; }

	//Testing operator ==
	std::cout << "Checking if vect2 and vectMove are equal... \n";
	bool eq = (vect2 == vectMove); //They are indeed equal
	eq ? std::cout << "Vectors are equal\n\n" : std::cout << "Vectors arent equal\n\n";

	//Testing operator !=
	std::cout << "Checking if vect2 and vectMove aren't equal... \n";
	bool eq2 = (vect2 != vectMove); //They are indeed equal
	eq2 ? std::cout << "Vectors are equal\n\n" : std::cout << "Vectors aren't equal\n\n";

	//Testing operator[] const:
	std::cout << "Getting vect2[5]: " << vect2[5] << _end;

	//Testing _refType operator[]
	std::cout << "Changing vect2[5] to 5... \n";
	vect2[5] = 5;
	std::cout << "Getting vect2[5]: " << vect2[5] << _end;

	//Pushing back (adding) the numbers from one to ten in vect1:
	std::cout << "Pushing back numbers in vect1... \n";
	for (int i = 0; i < 10; i++)
		vect1.push_back(i + 1);

	std::cout << "Vect 1: " << vect1 << _end; //1, 2, 3 ... 10

	//Subset and inclusivesubset functionality:
	std::cout << "Getting subset from vect 1 (from 1 to 6)...\n";
	vector<int> subset = vect1.inclusiveSubset(1, 6);
	std::cout << "New subset vector after subsetting: " << subset << _end;

	//operator+:
	std::cout << "Testing operator+ on vect1 and subset: \n";
	std::cout << vect1 + subset << _end;

	//operator+=:
	std::cout << "Testing operator+= on vect1 and subset: \n";
	vect1 += subset;
	std::cout << vect1 << _end;

	//shrinking vect2 (removing unused memory):
	std::cout << "Vect2 capacity: " << vect2.Capacity() << '\n' << "Schriking to fit...\nVect2 capacity: ";
	vect2.shrink_to_fit();
	std::cout << vect2.Capacity() << _end;

	//clear:
	std::cout << "Clearing vect2... ";
	vect2.clear();
	std::cout << vect2 << "\nsize:" << vect2.Size() << "\ncapacity: " << vect2.Capacity() << _end;

	std::cout << "isEmpty on vect2: " << vect2.isEmpty() << _end;

	//insertList:
	std::cout << "Inserting list of 9 integers in vect2...\n";
	int* list = new int[9];
	for (int i = 0; i < 9; i++)
		list[i] = 2 * i;
	vect2.insertList(0, list, 9);
	std::cout << "vect2: " << vect2 << _end;
	delete[] list;

	//pop_back removing numbers from back
	std::cout << "Popping back 2 numbers:\n";
	vect2.pop_back();
	vect2.pop_back();
	std::cout << "vect2: " << vect2 << _end;

	//resize:
	std::cout << "Resizing vect2 to size 100...\n";
	vect2.resize(100);
	std::cout << "vect2.size: " << vect2.Size() << _end;
	std::cout << "Setting element 100 to random number...\n";
	vect2[99] = rand() % 100;
	std::cout << "Getting vect2[99] -> " << vect2[99] << _end;

	//-------------------ITERATOR--------------------
	//Using iterators mainly to use 
	//range based for loop.
	std::cout << "Creating iteratorTester vector... "<<_end;
	vector<int> iteratorTester(10, 10);
	vector<int>::Iterator it = iteratorTester.begin();

	std::cout << "Using range based for loop to print the elements: \n";
	for (auto _val : iteratorTester)
		std::cout << _val << " ";

	//Find: returning copy of the searched element
	std::cout << "\npushing 20...\n";
	iteratorTester.push_back(20);
	std::cout << "Found by .find: "<<iteratorTester.find(iteratorTester.begin(), iteratorTester.end(), 20) << _end;;

	//Contains: searching for the element:
	std::cout << "Does it contain 20:\n";
	std::cout<<iteratorTester.contains(20)<<_end;

	//erase: Erasing every element
	std::cout << "Erasing every 10 in iteratorTester..." << _end;
	iteratorTester.erase(10);
	std::cout << iteratorTester << _end;

	std::cout << "Testing iterators >, <, ==, !=\nbegin == begin:\n"<<
		(iteratorTester.begin() == iteratorTester.begin())<<"\nbegin != _end: \n"<<
		(iteratorTester.begin() != iteratorTester.end())<<"\nbegin > _end: \n"<<
		(iteratorTester.begin() > iteratorTester.end())<<"\nbegin < _end: \n"<<
		(iteratorTester.begin() < iteratorTester.end())<<_end;
}

/*A .cpp file where i will test
every method of my psn::vector.
All of the tests are made with int,
but would work with every suitable
data type.*/
#include<iostream>
#include"vector.h"

int main() {
	std::cout << "==========TESTS OF VECTOR CLASS==========\n";

	//Default construction of vector:
	std::cout << "Creating empty vect1...\n";
	psn::vector<int> vect1;

	//Constructing vector by passing it size:
	std::cout << "\nCreating vect2 with capacity 100...\n";
	psn::vector<int> vect2(100);

	//Constructing vector by passing it size and T*:
	std::cout << "\nCreating vect3 with 10 elements with values 10:\n";
	psn::vector<int> vect3(10, 10);
	std::cout << "vect3: " << vect3 << "\n\n"; //10 10 10 ... 10;

	//Creating vector by copying other vector data:
	std::cout << "Creating vectCopy, copy of vect3:\n";
	psn::vector<int> vectCopy(vect3);
	std::cout << "vectCopy: " << vectCopy << "\n\n"; //10 10 10 ... 10;

	//Creating vector by moving another vector data:
	std::cout << "Creating vectMove, moving vect3: \n";
	psn::vector<int> vectMove(std::move(vect3));
	std::cout << vectMove << '\n'; //10 10 10 ... 10;
	try { std::cout << vect3[0] << '\n'; }
	catch (const std::exception&) { std::cout << "Vect 3 moved!\n\n"; }

	//Testing operator=
	std::cout << "Making vect3 equal to vectCopy... \n";
	vect3 = vectCopy;
	std::cout << "Vect 3: " << vect3 << "\n\n";

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
	std::cout << "Getting vect2[5]: " << vect2[5] << "\n\n";

	//Testing _refType operator[]
	std::cout << "Changing vect2[5] to 5... \n";
	vect2[5] = 5;
	std::cout << "Getting vect2[5]: " << vect2[5] << "\n\n";

	//Pushing back (adding) the numbers from one to ten in vect1:
	std::cout << "Pushing back numbers in vect1... \n";
	for (int i = 0; i < 10; i++)
		vect1.push_back(i + 1);

	std::cout << "Vect 1: " << vect1 << "\n\n"; //1, 2, 3 ... 10

	//Subset and inclusivesubset functionality:
	std::cout << "Getting subset from vect 1 (from 1 to 6)...\n";
	psn::vector<int> subset = vect1.inclusiveSubset(1, 6);
	std::cout << "New subset vector after subsetting: " << subset << "\n\n";

	//operator+:
	std::cout << "Testing operator+ on vect1 and subset: \n";
	std::cout << vect1 + subset << "\n\n";

	//operator+=:
	std::cout << "Testing operator+= on vect1 and subset: \n";
	vect1 += subset;
	std::cout << vect1 << "\n\n";

	//shrinking vect2 (removing unused memory):
	std::cout << "Vect2 capacity: " << vect2.Capacity() << '\n' << "Schriking to fit...\nVect2 capacity: ";
	vect2.shrink_to_fit();
	std::cout << vect2.Capacity() << "\n\n";

	//clear:
	std::cout << "Clearing vect2... ";
	vect2.clear();
	std::cout << vect2 << "\nsize:" << vect2.Size() << "\ncapacity: " << vect2.Capacity() << "\n\n";

	std::cout << "isEmpty on vect2: " << vect2.isEmpty() << "\n\n";

	//insertList:
	std::cout << "Inserting list of 9 integers in vect2...\n";
	int* list = new int[9];
	for (int i = 0; i < 9; i++)
		list[i] = 2 * i;
	vect2.insertList(0, list, 9);
	std::cout << "vect2: " << vect2 << "\n\n";
	delete[] list;

	//pop_back removing numbers from back
	std::cout << "Popping back 2 numbers:\n";
	vect2.pop_back();
	vect2.pop_back();
	std::cout << "vect2: " << vect2 << "\n\n";

	//resize:
	std::cout << "Resizing vect2 to size 100...\n";
	vect2.resize(100);
	std::cout << "vect2.size: " << vect2.Size() << "\n\n";
	std::cout << "Setting element 100 to random number...\n";
	vect2[99] = rand() % 100;
	std::cout << "Getting vect2[99] -> " << vect2[99] << "\n\n";

	//-------------------ITERATOR--------------------
	//Using iterators mainly to use 
	//range based for loop.
	psn::vector<int> iteratorTester(10, 10);
	psn::vector<int>::Iterator it = iteratorTester.begin();

	std::cout << "Using range based for loop to print the elements: \n";
	for (auto _val : iteratorTester)
		std::cout << _val << " ";

	std::cout << "\n\nTesting iterators >, <, ==, !=\nbegin == begin:\n"<<
		(iteratorTester.begin() == iteratorTester.begin())<<"\nbegin != end: \n"<<
		(iteratorTester.begin() != iteratorTester.end())<<"\nbegin > end: \n"<<
		(iteratorTester.begin() > iteratorTester.end())<<"\nbegin < end: \n"<<
		(iteratorTester.begin() < iteratorTester.end())<<"\n\n";
}

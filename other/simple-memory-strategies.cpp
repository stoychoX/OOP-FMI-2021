#include<iostream>
#include<string>
#include<vector>
#include<string_view>

void* operator new(size_t n) {
	std::cout << "Allocation made" << std::endl;
	return malloc(n);
}

void notOptimalPrint(const std::string& toPrint) {
	std::cout << toPrint << "\n";
}

void optimalPrintMe(std::string_view toPrint) {
	std::cout << toPrint << "\n";
}

int main() {
	std::cout << "Simple code ----string----";
	std::cout << "\n\nNot an optimal way:";
	std::string myName = "Stoycho Kyosev";
	std::string myFirstName = myName.substr(0, 7);
	std::string myLastName = myName.substr(8, 13);

	notOptimalPrint(myFirstName);
	notOptimalPrint(myLastName);

	std::cout << "\n\nOptimal way:\n";
	const char* name = "Stoycho Kyosev";
	std::string_view viewName(name);
	std::string_view first = viewName.substr(0, 7);
	std::string_view second = viewName.substr(8, 13);
	
	optimalPrintMe(first);
	optimalPrintMe(second);


	std::cout << "\n\nSimple code ----vector----";
	std::cout << "\n\nNot optimal way:\n";
	std::vector<int> v;
	for (int i = 0; i < 20; i++)
		v.push_back(i);

	std::cout << "info in vector: ";
	for (auto arg : v)
		std::cout << arg << " ";

	std::cout << "\n\nOptimal way:\n";

	std::vector<int> p;
	p.reserve(20);
	for (int i = 0; i < 20; i++)
		p.emplace_back(i);

	std::cout << "info in vector: ";
	for (auto arg : p)
		std::cout << arg << " ";
}
//runs only in c++17!

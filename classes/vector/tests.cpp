#include"vector.hpp"
#include<iostream>
#include<cassert>
#include<algorithm>
#include<chrono>
#include<functional>
#include<thread>

void constructorTests() {
	vector<int> start;
	size_t length = 200;

	for (size_t i = 0; i < length; i++)
		start.push_back(rand());

	vector<int> copy(start);
	for (size_t i = 0; i < length; i++)
		assert(copy[i] == start[i]);

	vector<int> moved(std::move(copy));

	for (size_t i = 0; i < length; i++)
		assert(moved[i] == start[i]);

	vector<int> range(moved.begin(), moved.end());

	for (size_t i = 0; i < length; i++)
		assert(range[i] == start[i]);
}

void pushBackTests() {
	vector<int> v;

	for (size_t i = 0; i < 1000; i++)
		v.push_back(i);

	assert(v.size() == 1000);

	for (int i = 0; i < 1000; i++)
		assert(v[i] == i);
}

void insertListTest() {
	vector<int> v;
	for (size_t i = 0; i < 1000; i++)
		v.push_back(i);

	int* arr = new int[500];
	for (size_t i = 0; i < 500; i++)
		arr[i] = 1000 + i;

	v.insertList(v.size(), arr, 500);

	delete[] arr;

	for (int i = 0; i < 1500; i++)
		assert(v[i] == i);
}

void eraseTest() {
	vector<int> v;

	for (size_t i = 0; i < 1001; i++)
		v.push_back(i);

	for (size_t i = 0; i < 1001; i += 2)
		v.erase(v.begin() + i);

	for (auto x : v)
		assert(x % 2);

	assert(v.size() == 500);
}

void findTests() {
	vector<int> v;

	for (size_t i = 0; i < 1000; i++)
		v.push_back(i);

	for (size_t i = 0; i < 1000; i++) {
		int fst = rand() % 1000;
		int snd = rand() % 1000;
		if (fst > snd) std::swap(fst, snd);

		if (fst == snd) continue;

		auto beg = v.begin() + fst;
		auto end = v.begin() + snd;
		assert
		(
			v.find(beg, end, (fst + snd) / 2) != v.end()
		);
	}

	assert(v.find(1001) == v.end());
}

void sortTests() {
	size_t testsCount = 400;
	size_t elemCount = 10000;

	vector<int> v;
	v.resize(elemCount);

	for (size_t i = 0; i < testsCount; i++) {
		for (size_t j = 0; j < elemCount; j++) {
			v[j] = rand();
		}
		v.sort();
		assert(std::is_sorted(v.begin(), v.end()));
	}
}

void intersecTests() {
	size_t elementsCount = 400;
	size_t testsCount = 150;

	vector<int> fst;
	vector<int> snd;
	fst.resize(elementsCount);
	snd.resize(elementsCount);
	for (size_t i = 0; i < testsCount; i++) {
		for (size_t j = 0; j < elementsCount; j++) {
			fst[j] = rand() % 1000;
			snd[j] = rand() % 1000;
		}

		auto res1 = fst.intersec(snd);
		auto res2 = snd.intersec(fst);

		for (auto x : res1) assert(fst.contains(x) && snd.contains(x));
		for (auto x : res2) assert(fst.contains(x) && snd.contains(x));
	}
}

void subsetTest() {
	for (int i = 0; i < 100; i++) {
		vector<int> v;

		size_t numElems = rand();

		for (size_t i = 0; i < numElems; i++)
			v.push_back(rand() % 10000);

		size_t fst = rand() % numElems;
		size_t snd = rand() % numElems;

		if (fst > snd)
			std::swap(fst, snd);

		auto subset = v.subset(v.begin() + fst, v.begin() + snd);

		auto vBeg = v.begin() + fst;
		auto sBeg = subset.begin();

		assert(subset.size() == snd - fst);

		for (; sBeg != subset.end(); ++sBeg, ++vBeg)
			assert(*sBeg == *vBeg);
	}
}

void runAllAsync() {
	vector<std::thread> v;
	vector<std::function<void()>> tests;

	tests.push_back(constructorTests);
	tests.push_back(pushBackTests);
	tests.push_back(findTests);
	tests.push_back(insertListTest);
	tests.push_back(sortTests);
	tests.push_back(intersecTests);
	tests.push_back(subsetTest);

	for (const auto& func : tests)
		v.move_back(std::thread(func));

	for (auto& cThread : v)
		cThread.join();
}

int main() {
	std::cout << "Start tests...\n";
	runAllAsync();
	std::cout << "No assertions failed!";

	// 4.69 s
	// on Intel CORE i5 10th GEN
	// compiled under MSVC & g++ 12
}

#include<iostream>
#include<queue>

void radixSort(int* arr, int len) {
	std::queue<int> holders[10];

	int digitLocation = 1;
	bool running = true;

	do {
		int leftShift = 1;
		int rightShift = 1;

		for (int i = 0; i < digitLocation; i++)
			leftShift *= 10;

		for (int i = 1; i < digitLocation; i++)
			rightShift *= 10;

		for (int i = 0; i < len; i++) {
			int currDigit = (arr[i] % leftShift) / rightShift;
			holders[currDigit].push(arr[i]);
		}

		int indexForReturningTheItems = 0;

		for (int i = 0; i < 10; i++) {
			while (holders[i].empty() == false) {
				int lastElem = holders[i].front();
				holders[i].pop();
				arr[indexForReturningTheItems++] = lastElem;
			}
		}

		bool allZeroes = true;
		for (int i = 0; i < len && allZeroes; i++)
			allZeroes = (((arr[i] / leftShift) == 0) && allZeroes);

		running = !allZeroes;

		digitLocation++;

	} while (running);
}

int main() {
	int* arr = new int[100];

	for (int i = 0; i < 100; i++)
		arr[i] = rand() % 55473;

	radixSort(arr, 100);

	for (int i = 0; i < 100; i++)
		std::cout << arr[i] << " ";
	
	delete[] arr;
}

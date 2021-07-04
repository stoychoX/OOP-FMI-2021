#include "engine.h"

engine::engine() {
	init();
}

void engine::init() {
	snake mySnake(10, 10);
	world myWorld(25, 40, mySnake);
	this->myWorld = myWorld;
}

void engine::run() {
	bool running = true;
	char c = ' ';
	this->myWorld.print();

	while (running) {
		if (_kbhit()) {
			c = _getch();
		}

		if (c == ' ')
			continue;

		system("ClS");
		myWorld.move(c, running);

		if (!running)
			continue;

		if (this->myWorld.isFruitLocation())
			this->myWorld.obtain();

		this->myWorld.print();
		Sleep(80);
	}

	this->myWorld.printScore();
}
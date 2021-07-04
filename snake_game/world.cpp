#include "world.h"

world::world()
{
}

world::world(int w, int h, snake s) {
	this->width = w;
	this->heigth = h;
	this->mySnake = s;
	this->generateNewLocation();
}

bool world::isDead() const {
	bool dead = false;
	int x = this->mySnake.getHeadX();
	int y = this->mySnake.getHeadY();

	dead = (x == 0 || y == this->width - 1 || y == 0 || x == this->heigth - 1);

	return dead || this->mySnake.headCrossesBody();
}

void world::printLoc() const {
	int x = this->mySnake.getHeadX();
	int y = this->mySnake.getHeadY();
	std::cout << std::endl << x << " " << y;
}

void world::move(const char _move, bool& exit) {
	char c = ' ';

	switch (_move) {
	case 'a':
		c = 'l';
		break;
	case 's':
		c = 'u';
		break;
	case 'd':
		c = 'r';
		break;
	case 'w':
		c = 'd';
		break;
	default:
		return;
		break;
	}

	this->mySnake.move(c);
	exit = !this->isDead();
}

int world::getFruitX() const {
	return this->fruitX;
}

int world::getFruitY() const {
	return this->fruitY;
}

void world::generateNewLocation() {
	srand(time(0));
	do {
		this->fruitX = rand() % this->heigth + 2;
		this->fruitY = rand() % this->width + 2;
	} while (this->mySnake.isBodyLocation(fruitX, fruitY) || this->fruitX >= this->heigth - 1 || this->fruitY >= this->width - 1);
}

bool world::isFruitLocation() const {
	return (this->mySnake.getHeadX() == getFruitX() &&
		this->mySnake.getHeadY() == getFruitY());
}

void world::obtain() {
	this->mySnake.eat();
	generateNewLocation();
}

void world::printScore() const {
	std::cout << "Died...\nScore: " << this->mySnake.length() * 10;
}

void world::print() const {
	for (int i = 0; i < this->width; i++) {
		for (int j = 0; j < this->heigth; j++) {
			if (!i || !j || (i == this->width - 1) || (j == this->heigth - 1)) {
				std::cout << "#";
				continue;
			}

			if (j == this->fruitX && i == this->fruitY) {
				std::cout << "F";
				continue;
			}

			int loc = this->mySnake.isCellLocation(j, i);
			if (loc != -1)
				std::cout << (char)(loc);
			else
				std::cout << " ";
		}
		std::cout << std::endl;
	}
	this->printLoc();
}

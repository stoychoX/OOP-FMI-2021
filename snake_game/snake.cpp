#include "snake.h"

snake::snake() : snake(5, 5) {}

snake::snake(int x, int y) {
	cell c = { x, y, 'O' };
	this->body.reserve(100);
	this->body.push_back(c);
	this->insertAtX = x + 1;
	this->insertAtY = y;
}

void snake::move(const char _move) {
	//movind first obj:
	this->insertAtX = this->body[this->body.size() - 1].x;
	this->insertAtY = this->body[this->body.size() - 1].y;

	int prevX = this->body[0].x;
	int prevY = this->body[0].y;
	int tempX = 0;
	int tempY = 0;

	switch (_move) {
	case 'l':
		this->body[0].x--;
		break;
	case 'r':
		this->body[0].x++;
		break;
	case 'u':
		this->body[0].y++;
		break;
	case 'd':
		this->body[0].y--;
		break;
	default:
		return;
		break;
	}

	//moving other objs:
	for (int i = 1; i < this->body.size(); i++) {
		tempX = body[i].x;
		tempY = body[i].y;

		body[i].x = prevX;
		body[i].y = prevY;

		prevX = tempX;
		prevY = tempY;
	}
}

int snake::getHeadX() const {
	return this->body[0].x;
}

int snake::getHeadY() const {
	return this->body[0].y;
}

int snake::isCellLocation(int x, int y) const {
	for (const cell& arg : this->body)
		if (arg.x == x && arg.y == y)
			return (int)(arg.val);
	return -1;
}

bool snake::isBodyLocation(int x, int y) const {
	for (const cell& arg : this->body)
		if (x == arg.x && y == arg.y)
			return true;
	return false;
}

int snake::length() const {
	return this->body.size();
}

bool snake::headCrossesBody() const {
	int x = this->body[0].x;
	int y = this->body[0].y;

	for (int i = 1; i < this->body.size(); i++)
		if (this->body[i].x == x && this->body[i].y == y)
			return true;

	return false;
}

void snake::eat() {
	cell c = { this->insertAtX, this->insertAtY, 'o' };
	this->insertAtX++;
	this->body.push_back(c);
}
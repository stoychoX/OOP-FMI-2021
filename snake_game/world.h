#pragma once
#include"snake.h"
#include<time.h>
#include<iostream>

class world {
private:
	snake mySnake;
	int width;
	int heigth;
	
	int fruitX;
	int fruitY;

public:
	world();
	world(int w, int h, snake s);

	bool isDead() const;
	void printLoc() const;
	void move(const char _move, bool&);

	int getFruitX() const;
	int getFruitY() const;

	void generateNewLocation();

	bool isFruitLocation() const;
	void obtain();

	void printScore() const;

	void print() const;
};
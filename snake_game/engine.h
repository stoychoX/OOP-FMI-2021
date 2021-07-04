#pragma once
#include"world.h"
#include<conio.h>
#include<windows.h>

class engine {
private:
	world myWorld;
	void init();
public:
	engine();
	void run();
};
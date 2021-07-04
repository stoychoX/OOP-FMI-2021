#pragma once
#include<vector>

struct cell {
	int x;
	int y;
	char val;
};

class snake {
private:
	std::vector<cell> body;
	int insertAtX;
	int insertAtY;
public:
	snake();
	snake(int x, int y);

	void move(const char _move);

	int getHeadX() const;
	int getHeadY() const;

	int isCellLocation(int x, int y) const;
	bool isBodyLocation(int x, int y) const;

	int length() const;

	bool headCrossesBody() const;

	void eat();
};


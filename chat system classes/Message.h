#pragma once
#include"user.h"
#include"DateTime.h"
#include<fstream>
#pragma warning(disable:4996)

class Message {
private:
	char content[1024];
	User* sentBy;
	User* reciever;
	DateTime createdAt;
public:
	//construct:
	Message();
	Message(const char* msg, User* sentBy, User* reciever);

	//helpers:
	void printMessage(std::ostream& os) const;
	void saveMessageToFile(const char* location);

	//decided not to re-writeq those thats why they're static
	static void readMessageFromFile(const char* location, const int& toRead);
	static int countDigits(int num);
	static char* intToChar(int toConvert);
};
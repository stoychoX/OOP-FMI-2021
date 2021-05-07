#include "Message.h"

int Message::countDigits(int num) {
	int cnt = 0;
	while (num != 0) {
		cnt++;
		num /= 10;
	}

	return cnt;
}

char* Message::intToChar(int toConvert) {
	int size = Message::countDigits(toConvert);

	char* toReturn = new char[size + 1];
	int it = size - 1;

	while (toConvert != 0) {
		int lastDigit = toConvert % 10;
		toReturn[it--] = lastDigit + '0';
		toConvert /= 10;
	}
	toReturn[size] = '\0';

	return toReturn;
}

Message::~Message(){
	free();
}

void Message::free(){
	delete[] this->reciever;
	delete[] this->sentBy;
}

Message::Message() {
	strcpy(this->content, "");
	this->sentBy = nullptr;
	this->reciever = nullptr;
}

Message::Message(const char* msg, User* sentBy, User* reciever) {
	this->sentBy = sentBy;
	this->reciever = reciever;
	strcpy(this->content, msg);
	DateTime date;
	this->createdAt = date.now();
}

void Message::printMessage(std::ostream& os) const {
	os << "Sent by: " << this->sentBy->getUsername() << "\nTo: " << this->reciever->getUsername() << "\nAt: ";
	this->createdAt.print(os);
	os << "Content: " << this->content;
	os << std::endl;
}

void Message::saveMessageToFile(const char* location) {
	std::ofstream outFile(location, std::ios::binary | std::ios::app);
	if (outFile.bad())
		return;

	int sizeMessage = strlen(this->content);
	int sizeNameOfSender = strlen(this->sentBy->getUsername());
	int sizeNameOfReciever = strlen(this->reciever->getUsername());

	int sizeOfDate = strlen(this->createdAt.getMonth());

	//message:
	outFile.write((const char*)&sizeMessage, sizeof(int));
	outFile.write(this->content, sizeMessage);

	//sender
	outFile.write((const char*)&sizeNameOfSender, sizeof(int));
	outFile.write(this->sentBy->getUsername(), sizeNameOfSender);

	//reciever
	outFile.write((const char*)&sizeNameOfReciever, sizeof(int));
	outFile.write(this->reciever->getUsername(), sizeNameOfReciever);

	//hour:
	int hrs = this->createdAt.getHrs();
	outFile.write((const char*)&hrs, sizeof(size_t));

	//minutes:
	int mins = this->createdAt.getMins();
	outFile.write((const char*)&mins, sizeof(size_t));

	//date:
	outFile.write((const char*)&sizeOfDate, sizeof(int));
	outFile.write(this->createdAt.getMonth(), sizeOfDate);

	outFile.close();
}

void Message::readMessageFromFile(const char* location, const int& toRead) {
	std::ifstream inFile(location, std::ios::binary | std::ios::app | std::ios::_Nocreate);

	if (!inFile) {
		std::cout << "No such chat"; 
		return;
	}

	const int MSG_BUFF = 100;

	char messages[MSG_BUFF][100];
	int cnt = 0;

	while (!inFile.eof()) {
		int size = 0;
		int senderSize = 0;
		int recieverSize = 0;

		int hrs = 0;
		int mins = 0;
		int dateLen = 0;
		//reciever
		inFile.read((char*)&size, sizeof(int));
		char* content = new char[size + 1];
		inFile.read(content, size);
		content[size] = '\0';

		if (size == 0)
			continue;

		//sender:
		inFile.read((char*)&senderSize, sizeof(int));
		char* senderName = new char[senderSize + 1];
		inFile.read(senderName, senderSize);
		senderName[senderSize] = '\0';

		//reciever
		inFile.read((char*)&recieverSize, sizeof(int));
		char* recieverName = new char[recieverSize + 1];
		inFile.read(recieverName, recieverSize);
		recieverName[recieverSize] = '\0';

		//hour:
		inFile.read((char*)&hrs, sizeof(size_t));

		//minute:
		inFile.read((char*)&mins, sizeof(size_t));

		//date
		inFile.read((char*)&dateLen, sizeof(int));
		char* date = new char[dateLen + 1];
		inFile.read(date, dateLen);
		date[dateLen] = '\0';

		messages[cnt][0] = '\0';
		strcat(messages[cnt], senderName);
		strcat(messages[cnt], " to ");
		strcat(messages[cnt], recieverName);
		strcat(messages[cnt], "(");
		strcat(messages[cnt], date);
		strcat(messages[cnt], " ");
		strcat(messages[cnt], Message::intToChar(hrs));
		strcat(messages[cnt], ":");
		strcat(messages[cnt], Message::intToChar(mins));
		strcat(messages[cnt], "): ");
		strcat(messages[cnt], content);
		strcat(messages[cnt], "\n");

		cnt++;

		delete[] recieverName;
		delete[] senderName;
		delete[] content;
		delete[] date;
	}

	inFile.close();

	int border = cnt - toRead;
	if (border < 0)
		border = 0;

	for (int i = border; i < cnt; i++){
		std::cout << messages[i];
	}
}
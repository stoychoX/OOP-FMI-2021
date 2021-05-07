#include<iostream>
#include"user.h"
#include"database.h"
#include "Message.h"
#include"libbcrypt/include/bcrypt/BCrypt.hpp"

//app commands:
#define ext "exit" //exit
#define reg "register" //creating a new user
#define enter "login" //logging in
#define chng "change" //changing password of a user

//User commands:
#define msg "message" //sending message
#define lgout "logout" //logging out of current user
#define view "view"//viewing message

//constants:
const int CMD_MX_SIZE = 10; //command max size
const int FIELD_MX_SIZE = 100; //field max size
const char* DATA_FILE = "users.vgdb"; //where to save data

//only allowed one word names, emails and passwords
void cleanStream() {
	std::cin.clear();
	std::cin.ignore(255, '\n');
}

//update database with changed passwords
void update(Database& data) {
	int userSize = data.getSize();
	if (userSize != 0)
		data.saveAndUpdate(data[0], DATA_FILE);
	else
		return;

	for (int i = 1; i < userSize; i++) {
		data.saveUserToFile(data[i], DATA_FILE);
	}
}

//adding user to database:
void registerUser(Database& myData, User& toAdd) {
	myData.addUser(toAdd);
	myData.saveUserToFile(toAdd, DATA_FILE);
}

//reading users from DATA_FILE
void readUsersAndWriteToData(Database& data) {
	std::ifstream inFile(DATA_FILE);
	if (inFile.bad()) {
		return;
	}

	while (!inFile.eof()) {
		char buff[FIELD_MX_SIZE];
		inFile.getline(buff, FIELD_MX_SIZE);
		if (strlen(buff) == 0) { //why
			continue;
		}
		User u = data.loadUserFromFile(buff);
		if (strcmp(u.getUsername(), "no-name") != 0) //why
			data.addUser(u);
	}

	inFile.close();
}

//view
void viewMessages(User& sender, User& reciever, const int& msgsToRead) {
	if (strcmp(sender.getUsername(), reciever.getUsername()) == 0)
		return;

	char viewLoc[1024];

	viewLoc[0] = '\0';
	size_t snd = sender.getID();
	size_t rcv = reciever.getID();

	size_t maxInd = std::max(snd, rcv);
	size_t minInd = std::min(snd, rcv);

	strcat(viewLoc, Message::intToChar(minInd));
	strcat(viewLoc, "-");
	strcat(viewLoc, Message::intToChar(maxInd));
	strcat(viewLoc, ".vgchat");

	Message::readMessageFromFile(viewLoc, msgsToRead);
}

//send
void compose(User* sender, User* reciever) {
	char messageLoc[1024];

	messageLoc[0] = '\0';
	size_t snd = sender->getID();
	size_t rcv = reciever->getID();

	size_t maxInd = std::max(snd, rcv);
	size_t minInd = std::min(snd, rcv);

	strcat(messageLoc, Message::intToChar(minInd));
	strcat(messageLoc, "-");
	strcat(messageLoc, Message::intToChar(maxInd));
	strcat(messageLoc, ".vgchat");

	std::cout << "Send message: ";
	char message[1024];
	std::cin.getline(message, 1024);

	Message m(message, sender, reciever);
	m.saveMessageToFile(messageLoc);
}

//showing uyser related options
void showUserDialog(Database& data, User& user) {
	std::cout << "Successfully logged in!\n";
	user.printSafe(std::cout);

	bool userIn = true;

	while (userIn) {
		std::cout << std::endl;
		char buff[FIELD_MX_SIZE];
		std::cout << "Enter command: ";
		std::cin >> buff;

		if (strcmp(buff, msg) == 0) {
			std::cout << "Reciever: ";
			char recieverName[FIELD_MX_SIZE];
			std::cin >> recieverName;

			int recieverIndex = data.findUserByUsername(recieverName);
			if (recieverIndex == -1) {
				std::cout << "No such user!\n";
				cleanStream();
				continue;
			}

			cleanStream();

			compose(&user, &data[recieverIndex]);

			std::cout << "Sent!\n";
		}
		else if (strcmp(buff, view) == 0) {
			int msgCount = 0;
			std::cout << "User: ";
			char uName[FIELD_MX_SIZE];
			std::cin >> uName;

			int userIndex = data.findUserByUsername(uName);
			if (userIndex == -1) {
				std::cout << "No such user!\n";
				continue;
			}

			std::cout << "How many messages would you like to see: ";
			std::cin >> msgCount;
			if (msgCount <= 0)
				msgCount = 5;

			viewMessages(user, data[userIndex], msgCount);
		}
		else if (strcmp(buff, lgout) == 0) {
			std::cout << "Logged out of: ";
			user.printSafe(std::cout);
			userIn = false;
		}
		std::cout << std::endl;
	}
}

//getting user info
void initUser(const Database& data, User& u) {
	char username[FIELD_MX_SIZE];
	char email[FIELD_MX_SIZE];
	char password[FIELD_MX_SIZE];

	std::cout << "Enter username: ";

	std::cin >> username;

	if (data.findUserByUsername(username) != -1) {
		std::cout << "We need unique username. Error!\n\n";
		return;
	}

	cleanStream();

	std::cout << "Enter password: ";
	std::cin >> password;

	cleanStream();

	std::cout << "Enter email: ";
	std::cin >> email;

	if (data.findUserByEmail(email) != -1) {
		std::cout << "Email already in use! Error!\n\n";
		return;
	}

	cleanStream();

	char* hashPass = new char[200];
	hashPass[0] = '\0';
	strcpy(hashPass, BCrypt::generateHash(password, 12).c_str()); //no string but BCrypt returns std::string?????

	u.setEmail(email);
	u.setUsername(username);
	u.setPassword(hashPass);
	u.setID(u.generateID());
	delete[] hashPass;
}

//logging in
void logIn(Database& data) {
	char username[FIELD_MX_SIZE];
	char* password = new char[FIELD_MX_SIZE];

	std::cout << "Enter username: ";
	std::cin >> username;

	int userFoundIndex = data.findUserByUsername(username);

	if (userFoundIndex == -1) {
		std::cout << "No such user\n";
		return;
	}

	cleanStream();

	std::cout << "Enter password: ";
	std::cin >> password;

	if (BCrypt::validatePassword(password, data[userFoundIndex].getPassword()) == false) {
		std::cout << "wrong password\n";
		return;
	}

	cleanStream();

	showUserDialog(data, data[userFoundIndex]);
	delete[] password;
}

//change pass dialog
void changePass(Database& data) {
	std::cout << "Enter username: ";
	char username[FIELD_MX_SIZE]; // TODO
	std::cin >> username;

	int userIndex = data.findUserByUsername(username);

	if (userIndex == -1) {
		std::cout << "No such user! \n";
		return;
	}

	char nPass[FIELD_MX_SIZE];
	std::cout << "Enter new pass: ";
	cleanStream();

	std::cin >> nPass;
	if (BCrypt::validatePassword(nPass, data[userIndex].getPassword()) == true) {
		std::cout << "Cant enter the same password!\n";
		return;
	}

	char* hashPass = new char[200];
	hashPass[0] = '\0';
	strcpy(hashPass, BCrypt::generateHash(nPass, 12).c_str());

	data[userIndex].setPassword(hashPass);
	data[userIndex].update();
	std::cout << "Successfully changed the password of user:\n";
	data[userIndex].printSafe(std::cout);
	update(data);
	delete[] hashPass;
}

//main loop:
void run() {
	Database mainData(0, 100, nullptr);
	readUsersAndWriteToData(mainData);

	bool running = true;
	while (running) {
		std::cout << "Enter command: ";
		char cmd[1024];
		std::cin >> cmd;
		if (strcmp(cmd, reg) == 0) {
			User toAdd;

			initUser(mainData, toAdd);

			while (mainData.findUserByID(toAdd.getID()) != -1) {
				toAdd.setID(toAdd.generateID());
			}

			bool isValidUser = strcmp(toAdd.getUsername(), "no-name") != 0;

			if (isValidUser)
				registerUser(mainData, toAdd);
		}
		else if (strcmp(cmd, ext) == 0) {
			running = false;
		}
		else if (strcmp(cmd, enter) == 0) {
			logIn(mainData);
			update(mainData);
		}
		else if (strcmp(cmd, chng) == 0) {
			changePass(mainData);
		}
		else {
			std::cout << "No such command!\n";
			cleanStream();
		}
		std::cout << std::endl;
	}
}

int main() {
	run();
}
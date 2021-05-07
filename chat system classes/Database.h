#pragma once
#include"user.h"
#include<fstream>
class Database {
private:
	User* usersInfo;
	size_t size;
	size_t capacity;
public:
	//construct:
	Database();
	Database(size_t sz, size_t capacity, User* uInfo);
	Database(const Database& other);

	//get:
	size_t getSize() const;
	size_t getCapacity() const;

	//set:
	void setSize(const size_t& sz);
	void setCapacity(const size_t capacity);
	void addUser(User& toAdd); //why not const - explain

	//helpers:
		//try templates:
	int findUserByID(const size_t& ID) const;
	int findUserByEmail(const char* email) const;
	int findUserByUsername(const char* username)const;

	User loadUserFromFile(const char* userInfo);
	void saveUserToFile(const User& toSave, const char* dir);
	void saveAndUpdate(const User& toSave, const char* dir);

	User& operator[](const int& index);

	void printAll(std::ostream& os) const;
	bool searchUserByID(const size_t& ID) const;
protected:
	User* copyDynamic(const User* other); //for users only
	void free();
	
public:
	~Database();
};
#pragma once
#include<cstring>//strcat, strcpy
#include<iostream> //ostream
#include<stdlib.h> //rand()
#include "DateTime.h"

class User {
	friend class Database; 
private:
	char* username;
	char* email;
	char* password;
	size_t ID;
	DateTime updatedAt;
	DateTime createdAt;

public:
	//construct:
	User();
	User(const char* username, const char* email, const char* password);
	User(const User& oher);

	//get:
	const char* getUsername() const;
	const char* getEmail() const;
	const char* getPassword() const;
	size_t getID() const;

	//set:
	void setUsername(const char* uName);
	void setEmail(const char* email); 
	void setPassword(const char* pass);
	void setID(const size_t ID);

	//helpers:
	void print(std::ostream& os) const;
	void printSafe(std::ostream& os) const; //no pass or email
	size_t generateID() const;
	User& operator=(const User& other);
	void update();
	void create();

private:
	char* copyDynamic(const char* str);
	void copy(const User& other);
	void free();

	//wrote those for email and password validation. After hashing password some of them aren't being used
	//but decided to leave them--------------------------------------------
	bool doesItContain(const char* arr, const char arg) const;
	bool doesItContainDigit(const char* arr) const;
	bool doesItContainUppercaseChar(const char* arr) const;
	bool isDigit(const char arg) const;
	bool isUpper(const char arg) const;
	//-------------------------------------------------

public:
	//destruct:
	~User();
};

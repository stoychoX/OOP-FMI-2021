#include "user.h"

#pragma warning(disable : 4996)

void User::print(std::ostream& os) const {
	os << "Username: " << this->username << "\nPassword: " << this->password << "\nEmail: " << this->email << "\nID: " << this->ID << std::endl;
}

void User::printSafe(std::ostream& os) const {
	os << "Username: " << this->username << " \nID: " << this->ID << std::endl;
	os << "Updated at: ";
	this->updatedAt.print(os);
	os << std::endl;
}

size_t User::generateID() const {
	return rand() % RAND_MAX + 1;
}

User& User::operator=(const User& other) {
	if (this != &other) {
		free();
		copy(other);
	}

	return *this;
}

char* User::copyDynamic(const char* str) {
	char* result = new char[strlen(str) + 1];
	strcpy(result, str);
	return result;
}

void User::copy(const User& other) {
	this->email = copyDynamic(other.email);
	this->password = other.password;
	this->username = copyDynamic(other.username);
	this->ID = other.ID;
}

void User::free() {
	delete[] this->email;
	delete[] this->username;
}

bool User::doesItContain(const char* arr, const char arg) const {
	int size = strlen(arr);
	for (int i = 0; i < size; i++) {
		if (arr[i] == arg)
			return true;
	}
	return false;
}

bool User::doesItContainDigit(const char* arr) const {
	int size = strlen(arr);

	for (int i = 0; i < size; i++) {
		if (isDigit(arr[i]))
			return true;
	}
	return false;
}

bool User::doesItContainUppercaseChar(const char* arr)const {
	int size = strlen(arr);
	for (int i = 0; i < size; i++) {
		if (isUpper(arr[i])) {
			return true;
		}
	}
	return false;
}

bool User::isDigit(const char arg)const {
	return arg >= '0' && arg <= '9';
}

bool User::isUpper(const char arg)const {
	return arg >= 'A' && arg <= 'Z';
}

void User::update(){
	this->updatedAt.getCurrentTime();
}

void User::create(){
	this->createdAt.getCurrentTime();
}

User::~User() {
	free();
}

User::User() {
	this->password = copyDynamic("no-pass");
	this->email = copyDynamic("no-mail");
	this->username = copyDynamic("no-name");
	update();
	create();
}

User::User(const char* username, const char* email, const char* password) :username(copyDynamic(username)),
password(copyDynamic(password)),
email(copyDynamic(email)), ID(generateID()) {
	update();
	create();
}

User::User(const User& other) {
	copy(other);
	update();
}

const char* User::getUsername() const {
	return this->username;
}

const char* User::getEmail() const {
	return this->email;
}

const char* User::getPassword() const {
	return this->password;
}

size_t User::getID() const {
	return this->ID;
}

void User::setUsername(const char* uName) {
	delete[] this->username;
	this->username = copyDynamic(uName);
}

void User::setEmail(const char* email) {
	delete[] this->email;

	bool isValidMail = doesItContain(email, '@') &&
		!(doesItContainDigit(email)) &&
		!doesItContainUppercaseChar(email) &&
		!doesItContain(email, ' ');

	if (isValidMail)
		this->email = copyDynamic(email);
	else {
		this->email = copyDynamic("---INVALID-MAIL---");
	}
}

void User::setPassword(const char* pass) {
	delete[] this->password;
	this->password = copyDynamic(pass);
}

void User::setID(const size_t ID) {
	this->ID = ID;
}
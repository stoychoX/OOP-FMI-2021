#include "Database.h"
#include"libbcrypt/include/bcrypt/BCrypt.hpp"

/*loading user data as we saved it 
@param userInfo - char, containing data about a certan user.*/
User Database::loadUserFromFile(const char* userInfo) {
	const int MAX_LEN_FLD = 40;
	const int PASS_SIZE = 1024;

	char username[MAX_LEN_FLD];
	char password[PASS_SIZE];
	char email[MAX_LEN_FLD];
	size_t ID = 0;

	int it = 0;
	int uIt = 0;
	int pIt = 0;
	int eIt = 0;


	while (userInfo[it] != ' ') {
		username[uIt++] = userInfo[it++];
	}

	it++;
	username[uIt] = '\0';

	while (userInfo[it] != ' ') {
		password[pIt++] = userInfo[it++];
	}

	it++;
	password[pIt] = '\0';

	while (userInfo[it] != ' ') {
		email[eIt++] = userInfo[it++];
	}

	it++;
	email[eIt] = '\0';

	while (userInfo[it] != '\0') {
		ID *= 10;
		ID += userInfo[it] - '0';
		it++;
	}

	User toReturn(username, email, password);
	toReturn.setID(ID);

	return toReturn;
}

/*saveUserToFile - saving user to file

@param toSave - a user to save
@param dir - file location*/
void Database::saveUserToFile(const User& toSave, const char* dir) {
	std::ofstream outFile(dir, std::ios::app);
	if (outFile.bad())
		return;

	outFile << toSave.username << " " << toSave.password << " " << toSave.email << " " << toSave.ID << std::endl;

	outFile.close();
}

/*Save and update - saving user and updating the database with new info /new password, new email could be easily added/*/
void Database::saveAndUpdate(const User& toSave, const char* dir) {

	std::ofstream outFile(dir, std::ios::trunc);
	if (outFile.bad())
		return;

	outFile << toSave.username << " " << toSave.password << " " << toSave.email << " " << toSave.ID << std::endl;

	outFile.close();
}

User& Database::operator[](const int& index) {
	if (index < 0 || index > this->size - 1) {
		User d;
		return d;
	}

	return this->usersInfo[index];
}

//just for debug ppses
void Database::printAll(std::ostream& os) const {
	int size = this->size;

	for (int i = 0; i < size; i++) {
		this->usersInfo[i].print(os);
	}
}

User* Database::copyDynamic(const User* other) {
	int size = this->size;

	User* result = new User[size];

	for (int i = 0; i < size; i++) {
		result[i] = other[i]; //overloaded =
	}

	return result;
}

void Database::free() {
	delete[] this->usersInfo;
}

bool Database::searchUserByID(const size_t& ID) const {
	int sz = this->size;

	for (int i = 0; i < sz; i++)
		if (this->usersInfo[i].ID == ID)
			return true;

	return false;
}

Database::~Database() {
	free();
}

Database::Database() {
	Database(0, 0, nullptr);
}

Database::Database(size_t sz, size_t capacity, User* uInfo) : size(sz), capacity(capacity) {
	this->usersInfo = copyDynamic(uInfo);
}

Database::Database(const Database& other) : capacity(other.capacity), size(other.size) {
	this->usersInfo = copyDynamic(other.usersInfo);
}

size_t Database::getSize() const {
	return this->size;
}

size_t Database::getCapacity() const {
	return this->capacity;
}

void Database::setSize(const size_t& sz) {
	this->size = sz;
}

void Database::setCapacity(const size_t capacity) {
	this->capacity = capacity;
}

void Database::addUser(User& toAdd) {
	if (this->capacity < this->size + 1) {
		std::cout << "Can't add user.";
		return;
	}

	//unexpectedly generating 0's sometimes
	if (toAdd.getID() == 0) {
		toAdd.setID(toAdd.generateID());
	}

	int nSize = this->size + 1;
	User* result = new User[nSize];

	for (int i = 0; i < nSize - 1; i++)
		result[i] = this->usersInfo[i];

	result[nSize - 1] = toAdd;
	this->size++;

	delete[] this->usersInfo;
	this->usersInfo = copyDynamic(result);
	delete[] result;
}

int Database::findUserByID(const size_t& ID)const {
	int sz = this->size;

	for (int i = 0; i < sz; i++)
		if (this->usersInfo[i].ID == ID)
			return i;
	return -1;
}

int Database::findUserByEmail(const char* email)const {
	int sz = this->size;
	for (int i = 0; i < sz; i++)
		if (strcmp(this->usersInfo[i].email, email) == 0)
			return i;
	return -1;
}

int Database::findUserByUsername(const char* username)const {
	int sz = this->size;
	for (int i = 0; i < sz; i++)
		if (strcmp(this->usersInfo[i].username, username) == 0)
			return i;
	return -1;
}
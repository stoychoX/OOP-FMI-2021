#include "string.h"

const int DEFAULT_CAPACITY = 4;

string::string() : vals(copyDynamic("")), size(0) {}

string::string(const char* arg) : vals(copyDynamic(arg)), size(strlen(arg)) {}

string::string(const string& other) {
	copy(other);
}

string::string(string&& other) noexcept : vals(other.vals), size(other.size) {
	other.vals = nullptr;
	other.size = 0;
}

string& string::operator=(const string& other) {
	if (this != &other) {
		free();
		copy(other);
	}
	return *this;
}

string& string::operator=(string&& other) noexcept {
	free();
	this->vals = other.vals;
	this->size = other.size;

	other.vals = nullptr;
	other.size = 0;

	return *this;
}

//TODO: faster algo
bool string::contains(const string& other) const {
	if (this->size < other.size)
		return false;

	for (int i = 0; i < this->size; i++)
		for (int j = 1; j <= this->size; j++) {
			if (i == j)
				continue;
			if (this->substr(i, j) == other)
				return true;
		}
	return false;
}

bool string::operator==(const string& other) const {
	return (strcmp(this->vals, other.vals) == 0);
}

string string::substr(const size_t& num) const {
	return substr(num, this->size);
}

string string::substr(const size_t& first, const size_t& second) const {
	if (first >= second || second > this->size)
		return string();

	size_t sz = second - first;

	if (first)
		sz++;

	char* newVals = new char[sz + 1];

	for (int i = first, ind = 0; i <= second; i++, ind++)
		newVals[ind] = this->vals[i];

	newVals[sz] = '\0';

	string toReturn(newVals);

	return toReturn;
}

string string::operator+(const string& second) const {
	size_t newSize = strlen(this->vals) + strlen(second.vals);

	char* newData = new char[newSize + 1];

	for (int i = 0; i < this->size; i++)
		newData[i] = this->vals[i];

	for (int i = this->size; i < newSize; i++)
		newData[i] = second.vals[i - this->size];

	newData[newSize] = '\0';

	string toReturn(newData);

	return toReturn;
}

string string::operator*(const size_t& num) const {
	string toReturn(this->vals);
	toReturn *= num;
	return toReturn;
}

string string::operator*=(const size_t& index) {
	string val(this->vals);

	for (int i = 1; i < index; i++)
		*this += val;

	return *this;
}

string string::operator+=(const string& other) {
	*this = *this + other;
	return *this;
}

string string::reverse() const {
	char* newData = new char[size];

	for (size_t i = 0; i < size; i++)
		newData[size - i - 1] = vals[i];

	newData[size] = '\0';
	
	return string(newData);
}

void string::clear() {
	*this = string();
}

void string::erase(const size_t& pos, const size_t& _npos) {
	if (pos > _npos || (_npos > size && _npos != string::npos))
		throw std::exception("Invalid position");

	string str = this->substr(0, pos);
	string str2 = this->substr(_npos);

	*this = str + str2;
}

void string::swap(string& other) {
	string temp = *this;
	*this = other;
	other = temp;
}

bool string::starts_with(const string& arg) const {
	size_t sz = arg.Size();

	if (sz > size)
		return false;
	return (substr(0, sz) == arg);
}

bool string::ends_with(const string& arg) const {
	size_t sz = arg.Size();

	if (sz > size)
		return false;

	return (substr(size - sz) == arg);
}

void string::resize(const size_t& n) {
	resize(n, (char)(0));
}

void string::resize(const size_t& n, const char arg) {
	if (n < 0)
		throw std::exception("Invalid size");

	if (n == 0)
		*this = string();

	if (n > this->size) {
		char* newVal = new char[n + 1];

		strcpy(newVal, vals);

		for (size_t i = size; i < n; i++)
			newVal[i] = arg;

		newVal[n] = '\0';
		*this = string(newVal);
	}
	else
		*this = this->substr(0, n);
}

void string::push_back(const char arg) {
	this->resize(size + 1, arg);
}

void string::pop_back() {
	*this = this->substr(0, size - 1);
}

void string::push_front(const char arg) {
	char* newVal = new char[size + 2];
	newVal[0] = arg;

	for (int i = 1; i <= size; i++)
		newVal[i] = vals[i - 1];

	newVal[size + 1] = '\0';
	*this = string(newVal);
}

void string::pop_front() {
	*this = this->substr(1, size);
}

size_t string::find_first_of(const string& str, size_t pos) const noexcept {
	if (pos > size)
		throw std::exception("Invalid position");

	for (size_t i = pos; i < size; i++) {
		size_t found = str.find_first_of(this->vals[i]);

		if (found != string::npos)
			return i;
	}

	return string::npos;
}

size_t string::find_first_of(const char c, size_t pos) const noexcept {
	if (pos > size)
		throw std::exception("Invalid position");

	for (int i = pos; i < size; i++) {
		if (this->vals[i] == c)
			return i;
	}
	return string::npos;
}

size_t string::find_last_of(const string& str, size_t pos) const noexcept {
	if (pos > size)
		throw std::exception("Invalid position");

	for (size_t i = size - 1; i >= pos && i != string::npos; i--) {
		size_t found = str.find_last_of(this->vals[i]);
		if (found != string::npos)
			return i;
	}
	return string::npos;
}

size_t string::find_last_of(const char c, size_t pos) const noexcept {
	if (pos > size)
		throw std::exception("Invalid position");

	for (size_t i = size - 1; i >= pos && i != string::npos; i--) {
		if (this->vals[i] == c)
			return i;
	}

	return string::npos;
}

size_t string::find_last_not_of(const string& str, size_t pos) const noexcept {
	if (pos > size)
		throw std::exception("Invalid position");

	for (size_t i = size - 1; i >= pos && i != string::npos; i--) {
		size_t found = str.find_last_not_of(this->vals[i]);
		if (found != string::npos)
			continue;
		return i;
	}
	return string::npos;
}

size_t string::find_last_not_of(const char c, size_t pos) const noexcept {
	if (pos > size)
		throw std::exception("Invalid position");

	for (size_t i = size - 1; i >= pos && i != string::npos; i--) {
		if (this->vals[i] != c)
			continue;
		return i;
	}

	return string::npos;
}

size_t string::find_first_not_of(const string& str, size_t pos) const noexcept {
	if (pos > size)
		throw std::exception("Invalid position");

	for (size_t i = pos; i < size; i++) {
		size_t found = str.find_first_of(this->vals[i]);

		if (found != string::npos)
			continue;
		return i;
	}

	return string::npos;
}

size_t string::find_first_not_of(const char c, size_t pos) const noexcept
{
	if (pos > size)
		throw std::exception("Invalid position");

	for (int i = pos; i < size; i++) {
		if (this->vals[i] != c)
			continue;
		return i;
	}
	return string::npos;
}

size_t string::Size() const {
	return this->size;
}

char& string::operator[](const size_t& index) {
	if (index > this->size)
		throw std::exception("Invalid index");
	return this->vals[index];
}

const char string::operator[](const size_t& index) const {
	if (index > this->size)
		throw std::exception("Invalid index");
	return this->vals[index];
}

char string::at(const size_t& index) const{
	if (index > this->size)
		throw std::exception("Invalid index");

	return this->vals[index];
}

const char* string::c_str() const noexcept {
	return this->vals;
}

char& string::front() { //undefined behavior @ (empty == true)
	return this->vals[0];
}

char& string::back() {
	return this->vals[this->size - 1];
}

const char string::c_front() const {
	return this->vals[0];
}

const char string::c_back() const {
	return this->vals[size - 1];
}

string string::tok(const char arg) const {
	size_t pos = this->find_first_of(arg);
	if (pos == string::npos)
		return string();
	return this->substr(0, pos);
}

bool string::empty() const {
	return !this->size;
}

bool string::operator>(const string& other) const {
	return (strcmp(this->vals, other.vals) == 1);
}

bool string::operator<(const string& other) const
{
	return (strcmp(this->vals, other.vals) == -1);
}

bool string::operator>=(const string& other) const {
	return ((this == &other) || (this > & other));
}

bool string::operator<=(const string& other) const {
	return ((this == &other) || (this < &other));
}

string::~string() {
	this->free();
}

void string::copy(const string& other) {
	this->vals = copyDynamic(other.vals);
	this->size = other.size;
}

char* string::copyDynamic(const char* arg) const{
	size_t currSize = strlen(arg);
	char* toReturn = new char[currSize + 1];

	for (size_t i = 0; i < currSize; i++)
		toReturn[i] = arg[i];

	toReturn[currSize] = '\0';
	return toReturn;
}

void string::free() {
	delete[] this->vals;
}

std::ostream& operator<<(std::ostream& os, const string& arg) {
	os << arg.c_str();
	return os;
}

// To be fixed!!!
std::istream& operator>>(std::istream& is, string& arg) {
	char toSet[1024];
	
	is >> toSet;
	
	arg = string(toSet);
	
	return is;
}

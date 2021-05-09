//
//A basic string class written on 6/5/2021 by 82275 
//
#include<iostream>
#pragma once
#pragma warning (disable : 4996)

class string {
private:
	char* vals;
	size_t size;
public:
	//constructors
	string();
	string(const char* arg);
	string(const string& other);
	string(string&& other) noexcept;

	//operator=
	string& operator=(const string& other);
	string& operator=(string&& other) noexcept;

	//boolean operations:
	bool contains(const string& other) const;
	bool operator==(const string& other) const;
	bool empty() const;
	bool operator>(const string& other) const;
	bool operator<(const string& other) const;
	bool operator>=(const string& other) const;
	bool operator<=(const string& other) const;
	bool starts_with(const string& arg) const;
	bool ends_with(const string& arg) const;

	//helpers:
	string substr(const size_t& num) const;
	string substr(const size_t& first, const size_t& second) const;
	string operator+(const string& second) const;
	string operator*(const size_t& num) const;
	string operator*=(const size_t& num);
	string operator+=(const string& other);
	string reverse() const;
	void clear();
	void erase(const size_t& pos = 0, const size_t& _npos = npos);
	void swap(string& other);
	
	//resize:
	void resize(const size_t& n);
	void resize(const size_t& n, const char arg);

	//push/pop back/front
	void push_back(const char arg);
	void pop_back();
	void push_front(const char arg);
	void pop_front();

	//find:
	size_t find_first_of(const string& str, size_t pos = 0) const noexcept;
	size_t find_first_of(const char c, size_t pos = 0) const noexcept;
	size_t find_last_of(const string& str, size_t pos = 0) const noexcept;
	size_t find_last_of(const char c, size_t pos = 0) const noexcept;
	size_t find_last_not_of(const string& str, size_t pos = 0) const noexcept;
	size_t find_last_not_of(const char c, size_t pos = 0) const noexcept;
	size_t find_first_not_of(const string& str, size_t pos = 0) const noexcept;
	size_t find_first_not_of(const char c, size_t pos = 0) const noexcept;

	//get:
	size_t Size() const;
	char& operator[](const size_t& index);
	const char operator[](const size_t& index) const;
	char at(const size_t& index) const;
	const char* c_str() const noexcept;
	char& front();
	char& back();
	const char c_front() const;
	const char c_back() const;
	string tok(const char arg) const; // str = my new string; str.tok(' ') -> my

	//ostr & istr:
	friend std::ostream& operator<<(std::ostream& os, const string& arg);
	friend std::istream& operator>>(std::istream& is, string& arg);

	//npos
	static const size_t npos = -1;

	//destructor:
	~string();
private:
	//helpers:
	void copy(const string& other); //copying into another object
	char* copyDynamic(const char* arg) const; //copying dynamically char* 
	void free(); //deleting dynamic memory
};

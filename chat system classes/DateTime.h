#pragma once
#include<ctime>
#include<iostream>

#pragma warning (disable:4996)

class DateTime {
private:
	size_t year;
	char month[12];
	char day[12];
	size_t dayNumber;
	size_t hour;
	size_t minutes;
	size_t seconds;
public:
	DateTime();
	DateTime(const size_t& year,
		const char* mounth,
		const char* day,
		const size_t& dayNum,
		const size_t& hour,
		const size_t& minutes,
		const size_t& seconds);

	DateTime operator=(const DateTime& other);
	bool operator==(const DateTime& other) const;
	DateTime now();
	void print(std::ostream& os) const;

	void setYear(const size_t year);
	void setMount(const char* mount);
	void setDay(const char* day);
	void setDayNumber(const size_t& dayNum);
	void setHours(const size_t& hrs);
	void setMinutes(const size_t& mins);
	void setSeconds(const size_t& secs);

	size_t getHrs() const;
	size_t getMins() const;
	const char* getMonth() const;

	void addSeconds(const size_t& toAdd);
	void addMinutes(const size_t& toAdd);
	void addHours(const size_t& toAdd);

	void getCurrentTime();

	friend std::ostream& operator<<(std::ostream& os, DateTime& toPrint);
private:
	void enterVal(const char* arr, char* fill, int& it, const char border, int& vIt);
	void enterNum(const char* arr, size_t& fill, int& it, const char border);
	void copy(const DateTime& other);
	bool isMemberOf(const char** arr, int arrLen, const char* searchedItem) const;
	size_t convertTimeToSeconds() const;
	void setTimeBySeconds(size_t& toSet);
	unsigned int getIndexOf(const char** arr, int arrLen, const char* searchedItem) const;
	bool isValidMonth() const;
	void setDaysNumberHelper(const size_t& toSet, unsigned int border);
	bool isLeapYear();
};
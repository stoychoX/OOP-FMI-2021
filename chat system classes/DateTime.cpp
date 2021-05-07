#include "DateTime.h"

const int MONTS_SIZE = 24;

const char* mountsCollection[MONTS_SIZE] = { "January", "February", "March", "April",
		"May", "June", "July","August", "September",
		"October", "November", "December" , "Jan",
"Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep",
"Oct", "Nov", "Dec" };

//Default constructor:
DateTime::DateTime() {
	this->year = 0; this->dayNumber = 0; this->hour = 0; this->minutes = 0; this->seconds = 0;
	strcpy(this->month, "unknown");
	strcpy(this->day, "unknown");
}

/*parameter constructor: */
DateTime::DateTime(const size_t& year, const char* mounth, const char* day, const size_t& dayNum, const size_t& hour, const size_t& minutes, const size_t& seconds) {
	this->setYear(year);
	this->setMount(mounth);
	this->setDay(day); this->setDayNumber(dayNum);
	this->setHours(hour);
	this->setMinutes(minutes);
	this->setSeconds(seconds);
}

DateTime DateTime::operator=(const DateTime& other) {
	if (this != &other) {
		copy(other);
	}
	return *this;
}

bool DateTime::operator==(const DateTime& other) const {
	return (this->year == other.year) && (strcmp(this->month, other.month) == 0) &&
		(strcmp(this->day, other.day) == 0) && (this->dayNumber == other.dayNumber)
		&& (this->hour == other.hour) && (this->minutes == other.minutes) &&
		(this->seconds == other.seconds);
}

//In this case i dont need validation. Data always valid /no user input/.
DateTime DateTime::now() {
	time_t now = time(0);

	char* dateTime = std::ctime(&now);
	size_t year;
	char month[10];
	char day[10];
	size_t dayNumber;
	size_t hour;
	size_t minutes;
	size_t seconds;

	int it = 0;
	int mIt = 0;
	int dIt = 0;

	enterVal(dateTime, day, it, ' ', dIt);
	if (dateTime[it] == ' ') //fixing the problem with single digit and multi-digit days
		it++;
	enterVal(dateTime, month, it, ' ', mIt);
	enterNum(dateTime, dayNumber, it, ' ');
	enterNum(dateTime, hour, it, ':');
	enterNum(dateTime, minutes, it, ':');
	enterNum(dateTime, seconds, it, ' ');
	enterNum(dateTime, year, it, '\n');

	DateTime toReturn(year, month, day, dayNumber, hour, minutes, seconds);

	return toReturn;
}

void DateTime::print(std::ostream& os) const {
	os << this->year << " " << this->month << " " << this->day << " " << this->dayNumber << " " <<
		this->hour << ":" << this->minutes << ":" << this->seconds << std::endl;
}

void DateTime::setYear(const size_t year) {
	if (year < 0)
		this->year = 0;
	else
		this->year = year;
}

void DateTime::setMount(const char* mount) {
	if (isMemberOf(mountsCollection, MONTS_SIZE, mount))
		strcpy(this->month, mount);
	else
		strcpy(this->month, "bad-format");
}

void DateTime::setDay(const char* day) {
	const int DAY_CNT = 14;
	const char* dayCollection[DAY_CNT] = { "Monday", "Tuesday", "Wednesday",
	"Thursday", "Friday", "Saturday", "Sunday", "Mon",
	"Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

	if (isMemberOf(dayCollection, DAY_CNT, day))
		strcpy(this->day, day);
	else
		strcpy(this->day, "bad-format");
}

void DateTime::setDaysNumberHelper(const size_t& toSet, unsigned int border) {
	if (toSet > 0 || toSet < border) {
		this->dayNumber = toSet;
	}
}

bool DateTime::isLeapYear() {
	return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);;
}

void DateTime::setDayNumber(const size_t& dayNum) {
	if (isValidMonth()) {
		int index = this->getIndexOf(mountsCollection, MONTS_SIZE, this->month) + 1;

		if (index > 12) index -= 12;

		switch (index) {
		case 1:case 3:case 5:case 7:case 8:	case 10:setDaysNumberHelper(dayNum, 32); break;
		case 2:if (!isLeapYear()) setDaysNumberHelper(dayNum, 28); else setDaysNumberHelper(dayNum, 29); break;
		case 4:case 6:case 9:case 11:case 12:setDaysNumberHelper(dayNum, 31); break;
		default:
			throw std::exception("Undefined behaviour");
			break;
		}
	}
}

void DateTime::setHours(const size_t& hrs) {
	if (hrs > 0 && hrs < 25)
		this->hour = hrs;
}

void DateTime::setMinutes(const size_t& mins) {
	if (mins >= 0 && mins < 60)
		this->minutes = mins;
}

void DateTime::setSeconds(const size_t& secs) {
	if (secs >= 0 && secs < 60)
		this->seconds = secs;
}

size_t DateTime::getHrs() const{
	return hour;
}

size_t DateTime::getMins() const{
	return this->minutes;
}

const char* DateTime::getMonth() const{
	return this->month;
}

void DateTime::addSeconds(const size_t& toAdd) {
	size_t currTime = convertTimeToSeconds();
	currTime += toAdd;
	setTimeBySeconds(currTime);
}

void DateTime::addMinutes(const size_t& toAdd) {
	this->addSeconds(60 * toAdd);
}

void DateTime::addHours(const size_t& toAdd) {
	this->addSeconds(3600 * toAdd);
}

void DateTime::getCurrentTime() {
	DateTime setter;
	*this = setter.now();
}

//helpers for now()
void DateTime::enterVal(const char* arr, char* fill, int& it, const char border, int& vIt) {
	while (arr[it] != border) {
		fill[vIt++] = arr[it++];
	}
	it++; fill[vIt] = '\0';
}

void DateTime::enterNum(const char* arr, size_t& fill, int& it, const char border) {
	fill = 0;
	while (arr[it] != border) {
		fill *= 10;
		fill += arr[it++] - '0';
	}
	it++;
}

void DateTime::copy(const DateTime& other) {
	this->year = other.year;
	strcpy(this->month, other.month);
	strcpy(this->day, other.day);
	this->dayNumber = other.dayNumber;
	this->hour = other.hour;
	this->minutes = other.minutes;
	this->seconds = other.seconds;
}

bool DateTime::isMemberOf(const char** arr, int arrLen, const char* searchedItem) const {
	for (int i = 0; i < arrLen; i++)
		if (strcmp(arr[i], searchedItem) == 0)
			return true;
	return false;
}

size_t DateTime::convertTimeToSeconds() const {
	return this->hour * 3600 + this->minutes * 60 + this->seconds;
}

void DateTime::setTimeBySeconds(size_t& toSet) {
	this->hour = toSet / 3600;
	toSet %= 3600;

	while (this->hour >= 24)
		this->hour -= 24;

	this->minutes = toSet / 60;
	toSet %= 60;

	while (minutes >= 60) {
		this->minutes -= 60;
		this->addHours(1);
	}

	this->seconds = toSet;

	while (seconds >= 60) {
		seconds -= 60;
		this->addMinutes(1);
	}
}

unsigned int DateTime::getIndexOf(const char** arr, int arrLen, const char* searchedItem) const {
	for (int i = 0; i < arrLen; i++)
		if (strcmp(arr[i], searchedItem) == 0)
			return i;
	return -1;
}

bool DateTime::isValidMonth() const {
	return this->getIndexOf(mountsCollection, MONTS_SIZE, month) != -1;
}

std::ostream& operator<<(std::ostream& os, DateTime& toPrint) {
	os << toPrint.year << " " << toPrint.month << " " << toPrint.day << " " << toPrint.dayNumber << " " <<
		toPrint.hour << ":" << toPrint.minutes << ":" << toPrint.seconds << std::endl;

	return os;
}
#include "DateTime.h"

#define SEC_IN_DAY 86400


void to_lovercase (char* str) {
	for (int i = 0; str[i] != '\0'; i++)
		if (str[i] <= 'Z' && str[i] >= 'A')
			str[i]+=32;
}

DateTime::DateTime() {
	time = std::time(nullptr);
}

DateTime::DateTime(DateTime const &time) {
	this->time = time.time;
}

DateTime::DateTime(unsigned _day, unsigned _month, unsigned _year) {
	struct tm tm_time {
		1,
		0,
		0,
		static_cast<int>(_day),
		static_cast<int>(_month - 1),
		static_cast<int>(_year-1900),
		0,
		0,
		0
	};

	time = std::mktime(&tm_time);
}

std::string DateTime::getToday() const {
	auto buf = new char[100];
	struct tm *t = std::localtime(&time);
	std::strftime(buf, 100, "%d %B 20%y, %A", t);
	to_lovercase(buf);

	return buf;
}

std::string DateTime::getYesterday() const {
	return this->getPast(1);
}

std::string DateTime::getTomorrow() const {
	return this->getFuture(1);
}

std::string DateTime::getFuture(unsigned int N) const {
	DateTime temp(*this);
	temp.time = this->time + N * SEC_IN_DAY;

	return temp.getToday();
}

std::string DateTime::getPast(unsigned int N) const {
	DateTime temp(*this);
	temp.time = this->time - N * SEC_IN_DAY;

	return temp.getToday();
}

unsigned int DateTime::getDifference(DateTime &diff) const {
	return std::abs(time - diff.time) / SEC_IN_DAY;
}

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

DateTime::DateTime(DateTime const &t) {
	this->time = t.time;
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
	auto buf = new char[100];
	time_t yesterday = time - SEC_IN_DAY;
	struct tm *t = std::localtime(&yesterday);
	std::strftime(buf, 100, "%d %B 20%y, %A", t);
	to_lovercase(buf);

	return buf;
}

std::string DateTime::getTomorrow() const {
	auto buf = new char[100];
	time_t tomorrow = time + SEC_IN_DAY;
	struct tm *t = std::localtime(&tomorrow);
	std::strftime(buf, 100, "%d %B 20%y, %A", t);
	to_lovercase(buf);

	return buf;
}

std::string DateTime::getFuture(unsigned int N) const {
	auto buf = new char[100];
	time_t future = time + N * SEC_IN_DAY;
	struct tm *t = std::localtime(&future);
	std::strftime(buf, 100, "%d %B 20%y, %A", t);
	to_lovercase(buf);

	return buf;
}

std::string DateTime::getPast(unsigned int N) const {
	auto buf = new char[100];
	time_t past = time - N * SEC_IN_DAY;
	struct tm *t = std::localtime(&past);
	std::strftime(buf, 100, "%d %B 20%y, %A", t);
	to_lovercase(buf);

	return buf;
}

unsigned int DateTime::getDifference(DateTime &diff) const {
	auto res = std::abs(time - diff.time);

	return res / SEC_IN_DAY;
}

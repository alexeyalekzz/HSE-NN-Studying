#ifndef TASK1_DATETIME_H
#define TASK1_DATETIME_H

#include <ctime>
#include <string>

class DateTime {
public:
	DateTime();
	DateTime(DateTime const &t);
	DateTime(unsigned _day, unsigned _month, unsigned _year);
	std::string getToday() const;
	std::string getYesterday() const;
	std::string getTomorrow() const;
	std::string getFuture(unsigned int N) const;
	std::string getPast(unsigned int N) const;
	unsigned int getDifference(DateTime& diff) const;
private:
	time_t time;
};

#endif //TASK1_DATETIME_H

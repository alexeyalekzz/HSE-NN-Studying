#include <iostream>
#include "DateTime.h"


int main() {

	DateTime t;

	std::cout << t.getToday() << t.getTomorrow() << t.getYesterday() << t.getFuture(5) << t.getPast(10);

	DateTime t2 (15, 11, 2019);

	std::cout << t2.getToday() << t.getDifference(t2);

	DateTime t3 (t2);

	std::cout << t3.getToday();

	return 0;
}

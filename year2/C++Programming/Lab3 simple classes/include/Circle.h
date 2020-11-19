#ifndef TASK1_CIRCLE_H
#define TASK1_CIRCLE_H

#include <cmath>

class Circle {
public:
	explicit Circle(double r);
	void setRadius (double new_r);
	void setFerence(double new_f);
	void setArea(double new_area);
	double getRadius() const;
	double getFerence() const;
	double getArea() const;
private:
	double radius;
	double ference;
	double area;
};

#endif //TASK1_CIRCLE_H

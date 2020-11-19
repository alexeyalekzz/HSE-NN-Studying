#include "Circle.h"
#include "task1.h"


double calcDelta() {
	const double earthRad{6378.1};
	Circle rope (earthRad);

	rope.setFerence((rope.getFerence() + 1)); // Прибавим метр веревки

	return rope.getRadius() - earthRad;
}

double calcCost() {
	Circle pool (3);
	Circle poolWithTrack (4);

	double S_Track = poolWithTrack.getArea() - pool.getArea();
	double track_cost = S_Track * 1000;
	double fence_cost = poolWithTrack.getFerence() * 2000;

	return track_cost + fence_cost;
}

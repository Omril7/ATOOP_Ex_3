
#ifndef EXERCISE_3_GEOMETRY_H
#define EXERCISE_3_GEOMETRY_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

using namespace std;

// angle units conversion functions
double to_radians(double theta_d);
double to_degrees(double theta_r);

// class declarations
struct Point {
	double x;
	double y;
	Point(double x, double y);
	Point();
	void print() const;
	bool operator==(const Point& rhs);
};

struct Polar_vector;

struct Cartesian_vector {
	Cartesian_vector(const struct Polar_vector & pv);
    Cartesian_vector(const struct Point & p);
	Cartesian_vector();
	void operator=(const struct Polar_vector & pv);
	double delta_x;
	double delta_y;
};

struct Polar_vector {
	Polar_vector(const Cartesian_vector & cv);
    Polar_vector(const Point & p);
	Polar_vector();
	void operator=(const Cartesian_vector & cv);
	double r;
	double theta;
};

#endif //EXERCISE_3_GEOMETRY_H
#ifndef CONSTANTS_GUARD_H
#define CONSTANTS_GUARD_H


#include <limits>
#include "Matrix.h"

const double DOUBLE_MAX_VALUE = std::numeric_limits<double>::max();

const double 	PI = 3.1415926535897932384;
const double 	TWO_PI = 6.2831853071795864769;
const double 	PI_ON_180 = 0.0174532925199432957;
const double 	invPI = 0.3183098861837906715;
const double 	invTWO_PI = 0.1591549430918953358;

const double 	kEpsilon = 0.0001;
const double	kHugeValue = 1.0E10;

static const Matrix IDENTITY_MATRIX = Matrix(1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1);

#endif
#ifndef RAY4D_H_
#define RAY4D_H_

#include <iostream>
#include <string>
#include "util.h"
#include "Ray.h"

using namespace trimesh;

class Ray4D {
public:

	vec4_d origin;
	vec4_d direction;

	// methods
	Ray4D();
	Ray4D(vec4_d origin, vec4_d direction);
	vec4_d getRayPoint(double t) const;
	//	friend std::ostream& operator<< (std::ostream& o, Ray const& ray);
	virtual ~Ray4D();
	static Ray4D convertRayTo4D(Ray ray, double origin_time, double direction_time);
};

inline vec4_d Ray4D::getRayPoint(double t) const {
	return origin + (t*direction);
}

#endif /* RAY4D_H_ */

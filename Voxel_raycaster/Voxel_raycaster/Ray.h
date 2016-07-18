/*
 * Ray.h
 *
 *  Created on: Feb 14, 2012
 *      Author: jeroenb
 */

#ifndef RAY_H_
#define RAY_H_

#include <iostream>
#include <string>
#include "util.h"

using namespace trimesh;

class Ray {
public:

	vec3_d origin;
	vec3_d direction;

	// methods
	Ray();
	Ray(vec3_d origin,vec3_d direction);
	vec3_d getRayPoint(double t) const;
//	friend std::ostream& operator<< (std::ostream& o, Ray const& ray);
	virtual ~Ray();
};

inline vec3_d Ray::getRayPoint(double t) const{
	return origin + (t*direction);
}

#endif /* RAY_H_ */

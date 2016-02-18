#ifndef RAY4D_H_
#define RAY4D_H_

#include <iostream>
#include <string>
#include "util.h"
#include "Ray.h"

using namespace trimesh;

class Ray4D {
public:

	vec4 origin;
	vec4 direction;

	// methods
	Ray4D();
	Ray4D(vec4 origin, vec4 direction);
	vec4 getRayPoint(float t) const;
	//	friend std::ostream& operator<< (std::ostream& o, Ray const& ray);
	virtual ~Ray4D();
	static Ray4D convertRayTo4D(Ray ray, float origin_time, float direction_time);
};

inline vec4 Ray4D::getRayPoint(float t) const {
	return origin + (t*direction);
}

#endif /* RAY4D_H_ */

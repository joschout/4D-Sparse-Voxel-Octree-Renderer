#include "Ray4D.h"

Ray4D::Ray4D() : origin(vec4_d(0, 0, 0, 0)), direction(vec4_d(0, 0, -1, 0)) {
}

Ray4D::Ray4D(vec4_d origin, vec4_d direction) : origin(origin), direction(direction) {
}

Ray4D::~Ray4D() {
	// TODO Auto-generated destructor stub
}

Ray4D Ray4D::convertRayTo4D(Ray ray, double origin_time, double direction_time)
{
	vec4_d origin = vec4_d(ray.origin[0], ray.origin[1], ray.origin[2], origin_time);
	vec4_d direction = vec4_d(ray.direction[0], ray.direction[1], ray.direction[2], direction_time);
	return Ray4D(origin, direction);
}


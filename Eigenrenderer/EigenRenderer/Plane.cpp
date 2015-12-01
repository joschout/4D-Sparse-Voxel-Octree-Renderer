#include "Plane.h"

const double Plane::kEpsilon = 0.001;

// ----------------------------------------------------------------------  default constructor

Plane::Plane(void)
	: GeometricObject(),
	point(0,0,0),
	normal(0, 1, 0)
{}


// ----------------------------------------------------------------------  constructor

Plane::Plane(const vec3& point, const vec3& normal)
	: GeometricObject(),
	point(point),
	normal(normal)
{
	normal = normalize(normal);
}


// ---------------------------------------------------------------- copy constructor

Plane::Plane(const Plane& plane)
	: GeometricObject(plane),
	point(plane.point),
	normal(plane.normal)
{}


// ---------------------------------------------------------------- clone

Plane*
Plane::clone(void) const {
	return (new Plane(*this));
}


// ---------------------------------------------------------------- assignment operator

Plane&
Plane::operator= (const Plane& rhs) {

	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	point = rhs.point;
	normal = rhs.normal;

	return (*this);
}


// ---------------------------------------------------------------- destructor

Plane::~Plane(void)
{}


// ----------------------------------------------------------------- hit

bool
Plane::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
	float t = (point - ray.origin) * normal / (ray.direction * normal);

	if (t > kEpsilon) {
		tmin = t;
		sr.normal = n;
		sr.local_hit_point = ray.o + t * ray.d;

		return (true);
	}

	return(false);
}


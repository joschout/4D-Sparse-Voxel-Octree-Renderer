#include "Ray.h"

// ---------------------------------------------------------------- default constructor

Ray::Ray (void)
	: 	origin(0.0), 
		direction(0.0, 0.0, 1.0) 
{}

Ray::Ray(const Point3D& orig, const Vector3D& dir)
{
	origin = orig;
	direction = dir;
}

// ---------------------------------------------------------------- copy constructor

Ray::Ray (const Ray& ray)
	: 	origin(ray.origin), 
		direction(ray.direction) 
{}

// ---------------------------------------------------------------- assignment operator

Ray& 
Ray::operator= (const Ray& rhs) {
	
	if (this == &rhs)
		return (*this);
		
	origin = rhs.origin; 
	direction = rhs.direction; 

	return (*this);	
}

// ---------------------------------------------------------------- destructor

Ray::~Ray (void) {}




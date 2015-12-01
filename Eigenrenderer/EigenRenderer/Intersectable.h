#ifndef INTERSECTABLE_H_GUARD
#define  INTERSECTABLE_H_GUARD

#include "Ray.h"
#include "ShadeRec.h"

class Intersectable
{
public:
	virtual ~Intersectable()
	{}

	virtual bool intersect(Ray ray, ShadeRec sr) = 0;
	Color color;
	Color getColor();
};

inline Color Intersectable::getColor()
{
	return color;
}
#endif

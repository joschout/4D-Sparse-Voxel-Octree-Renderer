#ifndef SPHERE_GUARD_H
#define SPHERE_GUARD_H

#include <TriMesh.h>
#include "Intersectable.h"
#include "Transformation.h"

class Sphere : public Intersectable {
public:
	Sphere(double radius, const trimesh::Color& color, const Transformation& transformation)
		: radius(radius),
		  color(color),
		  transformation(transformation)
	{
	}

	Sphere(void);

	const double radius;
	trimesh::Color color;
	Transformation transformation;
	
	~Sphere() override;

	bool intersect(Ray ray, ShadeRec sr) override;
};

#endif

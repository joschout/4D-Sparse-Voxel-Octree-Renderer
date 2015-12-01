#ifndef GUARD_SHADEREC_H
#define GUARD_SHADEREC_H

#include <TriMesh.h>
#include "Point3D.h"
#include "Normal.h"

using trimesh::Color;

class ShadeRec {
public:
	double t; 
	bool hasHitAnObject; //has the ray hit an object?
	Point3D localHitPoint; // world coordinates of hit point of the untransformed object
	Point3D hitPoint;  // world coordinates of hit point of the transformed object
	Normal normal;
	Color color;

	ShadeRec(void);
	ShadeRec(const ShadeRec& sr);
	~ShadeRec(void);
};

#endif // !GUARD_SHADEREC_H


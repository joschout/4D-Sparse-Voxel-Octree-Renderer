#ifndef GUARD_SHADEREC_H
#define GUARD_SHADEREC_H

#include <TriMesh.h>
#include "../util.h"

using trimesh::Color;

class ShadeRec {
public:
	float t;
	bool hasHitAnObject; //has the ray hit an object?
	vec3_d localHitPoint; // world coordinates of hit point of the untransformed object
	vec3_d hitPoint;  // world coordinates of hit point of the transformed object
	vec3_d normal;
	Color color;

	ShadeRec(void);
	ShadeRec(const ShadeRec& sr);
	~ShadeRec(void);
};

#endif // !GUARD_SHADEREC_H

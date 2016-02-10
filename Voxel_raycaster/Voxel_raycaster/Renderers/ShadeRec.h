#ifndef GUARD_SHADEREC_H
#define GUARD_SHADEREC_H

#include <TriMesh.h>


using trimesh::Color;

class ShadeRec {
public:
	float t;
	bool hasHitAnObject; //has the ray hit an object?
	trimesh::vec3 localHitPoint; // world coordinates of hit point of the untransformed object
	trimesh::vec3 hitPoint;  // world coordinates of hit point of the transformed object
	trimesh::vec3 normal;
	Color color;

	ShadeRec(void);
	ShadeRec(const ShadeRec& sr);
	~ShadeRec(void);
};

#endif // !GUARD_SHADEREC_H

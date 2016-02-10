#include "Constants.h"
#include "ShadeRec.h"

ShadeRec::ShadeRec()
{
	t = FLOAT_MAX_VALUE;
	hasHitAnObject = false;
	localHitPoint = trimesh::vec3();
	hitPoint = trimesh::vec3();
	normal = trimesh::vec3();
	color = Color(0, 0, 0);
}

ShadeRec::ShadeRec(const ShadeRec& sr)
	: t(sr.t),
	hasHitAnObject(sr.hasHitAnObject),
	localHitPoint(sr.localHitPoint),
	hitPoint(sr.hitPoint),
	normal(sr.normal),
	color(sr.color)
{
}

ShadeRec::~ShadeRec()
{
}
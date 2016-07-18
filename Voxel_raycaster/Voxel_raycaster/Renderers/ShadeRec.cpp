#include "Constants.h"
#include "ShadeRec.h"

ShadeRec::ShadeRec()
{
	t = FLOAT_MAX_VALUE;
	hasHitAnObject = false;
	localHitPoint = vec3_d();
	hitPoint = vec3_d();
	normal = vec3_d();
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
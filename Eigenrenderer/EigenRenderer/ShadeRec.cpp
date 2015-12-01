#include  "ShadeRec.h"
#include "Constants.h"

ShadeRec::ShadeRec()
{
	t = DOUBLE_MAX_VALUE;
	hasHitAnObject = false;
	localHitPoint = Point3D();
	hitPoint = Point3D();
	normal = Normal();
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

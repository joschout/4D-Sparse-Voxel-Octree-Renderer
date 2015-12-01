#include  "Sphere.h"
#include "Constants.h"

Sphere::Sphere(): radius(1.0), color(1.0,0.0,0.0), transformation(IDENTITY_TRANSFORM)
{
}

Sphere::~Sphere()
{
}


bool Sphere::intersect(Ray ray, ShadeRec sr)
{
	Ray transformed_ray = transformation.transformInverse(ray);

	Vector3D o = transformed_ray.origin;

	double a = transformed_ray.direction * transformed_ray.direction;
	double b = 2.0 * (transformed_ray.direction * o);
	double c = o * o - radius * radius;

	double d = b * b - 4.0 * a * c;

	if (d < 0) {
		return false;
	}
	double dr = sqrt(d);
	double q = b < 0 ? -0.5 * (b - dr) : -0.5 * (b + dr);

	double t0 = q / a;
	double t1 = c / q;

	if ((t0 >= kEpsilon && t1 >= kEpsilon && t1 >= t0) || (t0 >= kEpsilon && t1 < kEpsilon)) {
		sr.t = t0;
		Point3D localHitPoint = transformed_ray.origin + transformed_ray.direction * t0;
		Normal localNormal = localHitPoint.toVector3D().hat();
		Normal transformedNormal = transformation.transform(localNormal);

		sr.color = color;
		sr.hasHitAnObject = true;
	//	sr.ray = ray;
		sr.hitPoint = ray.origin + ray.direction* sr.t;

		sr.normal = transformedNormal;
		sr.localHitPoint = localHitPoint;
		return true;

	}
	if ((t0 >= kEpsilon && t1 >= kEpsilon && t0 > t1) || (t1 >= kEpsilon && t0 < kEpsilon)) {
		sr.t = t1;
		Point3D localHitPoint = transformed_ray.origin + transformed_ray.direction *t1;
		Normal localNormal = localHitPoint.toVector3D().hat();
		Normal transformedNormal = transformation.transform(localNormal);


		sr.color = color;
		sr.hasHitAnObject = true;
	//	sr.ray = ray;
		sr.hitPoint = ray.origin + ray.direction* sr.t;
		sr.normal = transformedNormal;
		sr.localHitPoint = localHitPoint;
		return true;
	}

	return false;
}

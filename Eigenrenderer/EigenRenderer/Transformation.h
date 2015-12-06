#ifndef TRANSFORMATION_H
#define  TRANSFORMATION_H
#include "Matrix.h"
#include "Vector3D.h"
#include "Point3D.h"
#include "Ray.h"
#include "Normal.h"
#include "Constants.h"


class Transformation
{
public: 
	Matrix matrix;
	Matrix inverse;

	Transformation(void);
	Transformation(const Matrix &matrix, const Matrix &inverse);

	Transformation invert() const;
	Transformation appendToTheLeft(const Transformation &transformation) const;
	Transformation appendToTheRight(const Transformation &transformation) const;

	static Transformation createTranslation(double x, double y, double z);
	static Transformation createScale(double x, double y, double z);
	static Transformation createRotationX(double angle);
	static Transformation createRotationY(double angle);
	static Transformation createRotationZ(double angle);
	static Transformation createIdentity();

	Vector3D transform(const Vector3D &vector) const;
	Vector3D transformInverse(const Vector3D &vector) const;

	Point3D transform(const Point3D &point) const;
	Point3D transformInverse(const Point3D& point) const;

	Ray transform(const Ray &ray) const;
	Ray transformInverse(const Ray &ray) const;

	Normal transform(const Normal &normal) const;
	
} ;

const Transformation IDENTITY_TRANSFORM = Transformation(IDENTITY_MATRIX, IDENTITY_MATRIX);
#endif


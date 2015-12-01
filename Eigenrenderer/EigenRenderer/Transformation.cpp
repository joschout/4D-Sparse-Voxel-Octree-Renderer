#include "Transformation.h"
#include "Constants.h"
#include <math.h>

Transformation::Transformation()
{
	matrix = Matrix();
	inverse = Matrix();
}

Transformation::Transformation(const Matrix& matrix_, const Matrix &inverse_)
{
	matrix = matrix_;
	inverse = inverse_;
}

Transformation Transformation::invert() const
{
	return Transformation(inverse, matrix);
}

Transformation Transformation::appendToTheLeft(const Transformation& transformation) const
{
	return Transformation(transformation.matrix * matrix,
		inverse * transformation.inverse
		);
}

Transformation Transformation::appendToTheRight(const Transformation& transformation) const
{
	return Transformation(matrix * transformation.matrix,
		transformation.inverse * inverse
		);
}

Transformation Transformation::createTranslation(double x, double y, double z)
{
	Matrix transformation = Matrix(1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1);
	Matrix inverse = Matrix(1, 0, 0, -x,
		0, 1, 0, -y,
		0, 0, 1, -z,
		0, 0, 0, 1);
	return Transformation(transformation, inverse);
}

Transformation Transformation::createScale(double x, double y, double z)
{
	Matrix transformation = Matrix(x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1);
	Matrix inverse =  Matrix(1 / x, 0, 0, 0,
		0, 1 / y, 0, 0,
		0, 0, 1 / z, 0,
		0, 0, 0, 1);
	return Transformation(transformation, inverse);
}

Transformation Transformation::createRotationX(double angle)
{
	auto radians = angle * PI_ON_180;
	auto sinRad = sin(radians);
	auto cosRad = cos(radians);

	Matrix transformation = Matrix(1, 0, 0, 0,
		0, cosRad, -sinRad, 0,
		0, sinRad, cosRad, 0,
		0, 0, 0, 1);
	Matrix inverse = transformation.transpose();
	return Transformation(transformation, inverse);
}

Transformation Transformation::createRotationY(double angle)
{
	auto radians = angle * PI_ON_180;
	auto sinRad = sin(radians);
	auto cosRad = cos(radians);

	Matrix transformation =  Matrix(cosRad, 0, sinRad, 0,
		0, 1, 0, 0,
		-sinRad, 0, cosRad, 0,
		0, 0, 0, 1);
	Matrix inverse = transformation.transpose();
	return Transformation(transformation, inverse);
}

Transformation Transformation::createRotationZ(double angle)
{
	auto radians = angle * PI_ON_180;
	auto sinRad = sin(radians);
	auto cosRad = cos(radians);

	Matrix transformation = Matrix(cosRad, -sinRad, 0, 0,
		sinRad, cosRad, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	Matrix inverse = transformation.transpose();
	return Transformation(transformation, inverse);
}

Transformation Transformation::createIdentity()
{
	return IDENTITY_TRANSFORM;
}

Vector3D Transformation::transform(const Vector3D& vector) const
{
	return matrix * vector;
}

Vector3D Transformation::transformInverse(const Vector3D& vector) const
{
	return inverse * vector;
}

Point3D Transformation::transform(const Point3D& point) const
{
	return matrix * point;
}

Point3D Transformation::transformInverse(const Point3D& point) const
{
	return inverse * point;
}

Ray Transformation::transform(const Ray& ray) const
{
	Point3D transformedOrigin = transform(ray.origin);
	Vector3D transformedDirection = transform(ray.direction);
	return Ray(transformedOrigin, transformedDirection);
}

Ray Transformation::transformInverse(const Ray& ray) const
{
	Point3D transformedOrigin = transformInverse(ray.origin);
	Vector3D transformedDirection = transformInverse(ray.direction);
	return Ray(transformedOrigin, transformedDirection);
}

Normal Transformation::transform(const Normal& normal) const
{
	Matrix transposeOfInverse = inverse.transpose();
	return (transposeOfInverse * normal);
	
}

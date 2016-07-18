#include "rotations.h"

vec3_d rotateX(const vec3_d& point, const float angle)
{
	float radAngle = angle * 3.14159265358979323846 / 180;
	float x_old = point[0];
	float y_old = point[1];
	float z_old = point[2];

	float x_new = x_old;
	float y_new = cos(radAngle) * y_old - sin(radAngle) * z_old;
	float z_new = sin(radAngle) * y_old + cos(radAngle) * z_old;

	return vec3_d(x_new, y_new, z_new);
}

vec3_d rotateY(const vec3_d& point, const float angle)
{
	float radAngle = angle * 3.14159265358979323846 / 180;
	float x_old = point[0];
	float y_old = point[1];
	float z_old = point[2];

	float x_new = cos(radAngle) * x_old + sin(radAngle) * z_old;
	float y_new = y_old;
	float z_new = -sin(radAngle) * x_old + cos(radAngle) * z_old;

	return vec3_d(x_new, y_new, z_new);
}

vec3_d rotateZ(const vec3_d& point, const float angle)
{
	float radAngle = angle * 3.14159265358979323846 / 180;
	float x_old = point[0];
	float y_old = point[1];
	float z_old = point[2];

	float x_new = cos(radAngle) * x_old - sin(radAngle) * y_old;
	float y_new = sin(radAngle) * x_old + cos(radAngle) * y_old;
	float z_new = z_old;

	return vec3_d(x_new, y_new, z_new);
}

/*
Rotates a vector along an arbitrary axis through the origin.

See also: http://inside.mines.edu/fs_home/gmurray/ArbitraryAxisRotation/
*/
vec3_d rotateAroundAxisThroughOrigin(const vec3_d& vector_to_rotate, const vec3_d& vector_of_axis, const float angle)
{

	double radAngle = angle * 3.14159265358979323846 / 180;
	double cos_angle = cos(radAngle);
	double sin_angle = sin(radAngle);

	vec3_d rotated_vector;

	const double& u = vector_of_axis[0];
	const double& v = vector_of_axis[1];
	const double& w = vector_of_axis[2];

	const double& x = vector_to_rotate[0];
	const double& y = vector_to_rotate[1];
	const double& z = vector_to_rotate[2];

	double ux_vy_wz = u * x + v * y + w * z;
	double one_minus_cos = 1 - cos_angle;

	rotated_vector[0] = u * ux_vy_wz * one_minus_cos + x * cos_angle + (-w * y + v * z) * sin_angle;
	rotated_vector[1] = v * ux_vy_wz * one_minus_cos + y * cos_angle + (w * x - u * z) * sin_angle;
	rotated_vector[2] = z * ux_vy_wz * one_minus_cos + z * cos_angle + (-v * x + u * y) * sin_angle;

	return rotated_vector;
}
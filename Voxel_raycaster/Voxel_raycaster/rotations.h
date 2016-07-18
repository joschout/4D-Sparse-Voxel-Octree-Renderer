#ifndef ROTATIONS_H
#define ROTATIONS_H

#include "util.h"

// rotates about the x axis in a counter clockwise direction.
vec3_d rotateX(const vec3_d& point, const float angle);

// rotates about the y axis in a counter clockwise direction.
vec3_d rotateY(const vec3_d& point, const float angle);

// rotates about the z axis in acounter clockwise direction.
vec3_d rotateZ(const vec3_d& point, const float angle);

/*
Rotates a vector along an arbitrary axis through the origin.

See also: http://inside.mines.edu/fs_home/gmurray/ArbitraryAxisRotation/
*/
vec3_d rotateAroundAxisThroughOrigin(const vec3_d& vector_to_rotate, const vec3_d& vector_of_axis, const float angle);
#endif


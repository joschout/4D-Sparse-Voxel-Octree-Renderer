#ifndef POINT_H
#define POINT_H

#include <TriMesh.h>
using trimesh::Vec;

class Point : public Vec<3, float> {
public:
	Point()
	{}
	Point(const Vec<3, float> &v_) : Vec<3, float>(v_)
	{}
	Point(const Vec<3, double> &v_) : Vec<3, float>((float)v_[0], (float)v_[1], (float)v_[2])
	{}
	Point(float x, float y, float z) : Vec<3, float>(x, y, z)
	{}
	Point(double x, double y, double z) : Vec<3, float>((float)x, (float)z, (float)z)
	{}
	explicit Point(const float *xyz) : Vec<3, float>(xyz[0], xyz[1], xyz[2])
	{}
	explicit Point(const double *xyz) : Vec<3, float>((float)xyz[0], (float)xyz[1], (float)xyz[2])
	{}
	Point(float x, double y, double z, double w):Vec<3, float>((float)x*1.0/w, (float)z*1.0/w, (float)z*1.0/w)
	{}

};

static double invert(double w)
{
	return 1.0 / w;
}

#endif 

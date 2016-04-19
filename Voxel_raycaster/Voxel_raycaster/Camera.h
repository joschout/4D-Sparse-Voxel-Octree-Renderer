#ifndef CAMERA_H_GUARD
#define CAMERA_H_GUARD


#include "TriMesh.h"

//#define M_PI 3.14159265358979323846;

using trimesh::vec3;
class Camera {
public:
	// http://www.cs.toronto.edu/~jepson/csc2503/readings/Camera.pdf
	// camera coordinates have their origin at the “eye”(pinhole) of the camera, e
	vec3 eye;
	//Let g be the gaze direction
	vec3 gaze;
	// Let t be the “up” direction(e.g., toward the sky so ~t = (0, 1, 0))
	vec3 top;
	//OrthonormalBasis basis;
	vec3 u;
	vec3 v;
	vec3 w;

	Camera() : eye(vec3(0, 0, 0)), gaze(vec3(0, 0, -1)), top(vec3(0, 1, 0)) {
		this->computeUVW();
	}
	Camera(vec3 eye, vec3 gaze, vec3 top) : eye(eye), gaze(gaze), top(top) {
		this->computeUVW();
	}

	void computeUVW() {
		vec3 w_ = normalize(gaze) * -1.0f;
		vec3 u_ = normalize(vec3(top CROSS w_));
		vec3 v_ = (w_ CROSS u_);
		w = w_; u = u_; v = v_;
	}
	//	void computeNewOrthonormalbasis()
	//	{
	//		basis = OrthonormalBasis(gaze, top);
	//	}
};


// rotates about the x axis in a counter clockwise direction.
inline trimesh::vec3 rotateX(const trimesh::vec3 &point, const float angle)
{
	float radAngle = angle * 3.14159265358979323846 / 180;
	float x_old = point[0];
	float y_old = point[1];
	float z_old = point[2];

	float x_new = x_old;
	float y_new = cos(radAngle) * y_old - sin(radAngle) * z_old;
	float z_new = sin(radAngle) * y_old + cos(radAngle) * z_old;

	return trimesh::vec3(x_new, y_new, z_new);
}

// rotates about the y axis in a counter clockwise direction.
inline trimesh::vec3 rotateY(const trimesh::vec3 &point, const float angle)
{
	float radAngle = angle * 3.14159265358979323846 / 180;
	float x_old = point[0];
	float y_old = point[1];
	float z_old = point[2];

	float x_new = cos(radAngle) * x_old + sin(radAngle) * z_old;
	float y_new = y_old;
	float z_new = -sin(radAngle) * x_old + cos(radAngle) * z_old;

	return trimesh::vec3(x_new, y_new, z_new);
}

// rotates about the z axis in acounter clockwise direction.
inline trimesh::vec3 rotateZ(const trimesh::vec3 &point, const float angle) 
{
	float radAngle = angle * 3.14159265358979323846 / 180;
	float x_old = point[0];
	float y_old = point[1];
	float z_old = point[2];

	float x_new = cos(radAngle) * x_old - sin(radAngle) * y_old;
	float y_new = sin(radAngle) * x_old + cos(radAngle) * y_old;
	float z_new = z_old;

	return trimesh::vec3(x_new, y_new, z_new);
}




#endif /*CAMERA_H_GUARD*/
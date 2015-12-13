#ifndef CAMERA_H_GUARD
#define CAMERA_H_GUARD


#include "TriMesh.h"

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
#endif /*CAMERA_H_GUARD*/
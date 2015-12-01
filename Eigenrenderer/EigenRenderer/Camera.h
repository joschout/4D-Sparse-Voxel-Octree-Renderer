#ifndef CAMERA_H_GUARD
#define CAMERA_H_GUARD

#include "OrthonormalBasis.h"
#include "Vector3D.h"


class Camera {
public:
	// http://www.cs.toronto.edu/~jepson/csc2503/readings/Camera.pdf
	// camera coordinates have their origin at the “eye”(pinhole) of the camera, e
	Vector3D eye;
	//Let g be the gaze direction
	Vector3D gaze;
	// Let t be the “up” direction(e.g., toward the sky so ~t = (0, 1, 0))
	Vector3D top;
	OrthonormalBasis basis;


	Camera() : eye(Vector3D(0, 0, 0)), gaze(Vector3D(0, 0, -1)), top(Vector3D(0, 1, 0)) {
		basis = OrthonormalBasis(gaze, top);
	}
	Camera(Vector3D eye, Vector3D gaze, Vector3D t) : eye(eye), gaze(gaze), top(top) {
		basis = OrthonormalBasis(gaze, top);
	}

};
#endif /*CAMERA_H_GUARD*/
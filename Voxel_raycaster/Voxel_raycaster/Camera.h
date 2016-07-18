#ifndef CAMERA_H_GUARD
#define CAMERA_H_GUARD

#include "TriMesh.h"
#include "util.h"

//#define M_PI 3.14159265358979323846;

using trimesh::vec3;
class Camera { 

public:
	// http://www.cs.toronto.edu/~jepson/csc2503/readings/Camera.pdf
	// camera coordinates have their origin at the “eye”(pinhole) of the camera, e
	vec3_d eye;

	Camera();

	Camera(vec3_d eye, vec3_d gaze, vec3_d top);

	void computeUVW();

	vec3_d getU() const;
	vec3_d getV() const;
	vec3_d getW() const;

	vec3_d getGaze() const;
	void setGaze(vec3_d gaze);

	vec3_d getTop() const;
	void setTop(vec3_d top);

private:
	

	// Let t be the “up” direction(e.g., toward the sky so ~t = (0, 1, 0))
	vec3_d top;
	//OrthonormalBasis basis;
	vec3_d u;
	vec3_d v;
	vec3_d w;

	//Let g be the gaze direction
	vec3_d gaze;
	
};

#endif /*CAMERA_H_GUARD*/

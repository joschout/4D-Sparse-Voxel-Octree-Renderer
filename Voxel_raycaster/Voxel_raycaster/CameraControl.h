#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "util.h"
#include "Renderers/RenderManager4D.h"
#include "RendererManager.h"
#include "Octree.h"


class CameraController{
public:

	Camera *camera;
	FileFormat *inputformat;
	RendererManager *rmanager;
	RendererManager4D *rmanager4D;
	Octree *octree;
	RenderContext *render_context;
	unsigned char* renderdata;


	float camera_eye_movement_speed = 3.0f; // 3 units / second
	float camera_gaze_movement_speed = 3.0f; // 3 units / second
	float time_movement_speed = 1.0f;

	float time_point = 0.0f;
	float time_step = 1.0f;

	bool changeOnlyOnKeyPress = false;

	bool camera_eye_plus_X;
	bool camera_eye_minus_X;
	bool camera_eye_plus_Y;
	bool camera_eye_minus_Y;
	bool camera_eye_plus_Z;
	bool camera_eye_minus_Z;

	bool camera_gaze_plus_X;
	bool camera_gaze_minus_X;
	bool camera_gaze_plus_Y;
	bool camera_gaze_minus_Y;
	bool camera_gaze_plus_Z;
	bool camera_gaze_minus_Z;

	bool time_plus_constant;
	bool time_minus_constant;
	bool time_plus_timestep;
	bool time_minus_timestep;

	CameraController();
	CameraController(Camera* camera, FileFormat *inputformat,
		RendererManager *render_manager, RendererManager4D *render_manager4D,
		Octree * octree, RenderContext *rc,
		unsigned char* renderdata);
	void keyboardfunc(GLFWwindow* window, int key, int scancode, int action, int mods);
	void moveCamera();
};

#endif

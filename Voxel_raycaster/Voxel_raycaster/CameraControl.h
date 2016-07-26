#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "util.h"
#include "Renderers/RenderManager4D.h"
#include "RendererManager.h"
#include "Tree4D.h"


class CameraController{
public:

	Camera *camera;
	FileFormat *inputformat;
	RendererManager *rmanager;
	RendererManager4D *rmanager4D;
	Tree4D *tree4D;
	RenderContext *render_context;
	unsigned char* renderdata;


	float camera_eye_movement_speed = 3.0f; // 3 units / second
	float camera_gaze_movement_speed = 3.0f; // 3 units / second
	float time_movement_speed = 1.0f;

	float time_point = 0.0;
	double time_step = 1.0;

	float space_step_X = 1.0f;
	float space_step_Y = 1.0f;
	float space_step_Z = 1.0f;


	float space_step_u = 1.0f;
	float space_step_v = 1.0f;
	float space_step_w = 1.0f;
	
	double rotation_angle_degrees = 10;


	bool changeOnlyOnKeyPress = true;

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

	size_t level_to_render = 1;


	CameraController();
	CameraController(Camera* camera, FileFormat *inputformat,
		RendererManager *render_manager, RendererManager4D *render_manager4D,
		Tree4D * tree4D, RenderContext *rc,
		unsigned char* renderdata);
	void keyboardfunc(GLFWwindow* window, int key, int scancode, int action, int mods);
	void moveCamera();
private:
	void handleSingleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods);
	void handleSingleKeyPress2(GLFWwindow* window, int key, int scancode, int action, int mods);
	void handleContinuousKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods);
	void handleKeyPressOnly(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif


#ifndef INITIALIZE_RENDERING_H
#define INITIALIZE_RENDERING_H
#include <Vec.h>
#include "RenderContext.h"

#include "util.h"

inline void initCamera(Camera &camera)
{
	//camera = Camera(vec3_d(0.5, 0.5, -2), vec3_d(0, 0, -1), vec3_d(0, 1, 0));//Camera(vec3_d(0, 0, -30), vec3_d(0, 0, -1), vec3_d(0, 1, 0));
	camera = Camera(vec3_d(0.125, 0.125, -0.5), vec3_d(0, 0, -1), vec3_d(0, 1, 0));
}

inline void initRenderSystem(unsigned int render_x, unsigned int render_y,
	RenderContext &render_context, Frustrum &frustrum, Camera &camera)
{	

	// Initialize camera
	initCamera(camera);

	// initialize frustrum
	float aspect_ratio = render_x / render_y;
	frustrum = Frustrum(45, aspect_ratio, 1, 100); // THIS near and far SHOULD BE NEGATIVE

	render_context = RenderContext(&camera, &frustrum, render_x, render_y);

	// initialize lights
	Light mylight = Light(vec3_d(0, 0, 0), vec3_d(1.0, 1.0, 1.0));
	mylight.SHININESS = 25.0f;
	Light mylight2 = Light(vec3_d(4, 0, -3.0f), vec3_d(0.0, 0.0, 0.8));
	Light mylight3 = Light(vec3_d(0, 0, -3.0f), vec3_d(0.0, 0.8, 0.0));
	render_context.lights.push_back(mylight);
	render_context.lights.push_back(mylight2);
	render_context.lights.push_back(mylight3);
}
#endif

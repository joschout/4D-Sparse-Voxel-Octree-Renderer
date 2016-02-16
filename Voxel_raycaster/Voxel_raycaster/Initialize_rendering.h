#ifndef INITIALIZE_RENDERING_H
#define INITIALIZE_RENDERING_H
#include <Vec.h>
#include "RenderContext.h"

using trimesh::vec3;

inline void initCamera(Camera &camera)
{
	camera = Camera(vec3(0, 0, -2), vec3(0, 0, -1), vec3(0, 1, 0));
}

inline void initRenderSystem(unsigned int render_x, unsigned int render_y,
	RenderContext &render_context, Frustrum &frustrum, Camera &camera)
{	
	initCamera(camera);
	float aspect_ratio = render_x / render_y;
	frustrum = Frustrum(45, aspect_ratio, 1, 100); // THIS near and far SHOULD BE NEGATIVE
	render_context = RenderContext(&camera, &frustrum, render_x, render_y);
	Light mylight = Light(vec3(0, 0, 0), vec3(1.0, 1.0, 1.0));
	mylight.SHININESS = 25.0f;
	Light mylight2 = Light(vec3(4, 0, -3.0f), vec3(0.0, 0.0, 0.8));
	Light mylight3 = Light(vec3(0, 0, -3.0f), vec3(0.0, 0.8, 0.0));
	render_context.lights.push_back(mylight);
	render_context.lights.push_back(mylight2);
	render_context.lights.push_back(mylight3);
}
#endif

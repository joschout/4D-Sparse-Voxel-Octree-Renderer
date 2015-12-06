#include "Camera.h"
#include "Frustrum.h"
#include "RenderContext.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>
#include "PPMWriter.h"
#include "Sphere.h"
#include "DebugRenderer.h"


using std::string;

//initial width and height
const unsigned int width_window = 640;
const unsigned int height_window = 640;
const const char* windowName = "Eigen renderer";

// viewpoint
Camera camera;
Frustrum frustrum;
RenderContext render_context;

// renderer
Renderer* renderer;

unsigned char* myData;

// OpenGL
GLuint texid;

void initializeRenderContext(unsigned int render_x, unsigned int render_y) {
	camera = Camera(Vector3D(0, 0, 0), Vector3D(0, 0, -1), Vector3D(0, 1, 0));
	float aspect_ratio = render_x / render_y;
	frustrum = Frustrum(45, aspect_ratio, 1, 100); // THIS near and far SHOULD BE NEGATIVE
	render_context = RenderContext(&camera, &frustrum, render_x, render_y);


	Sphere test_sphere = Sphere();
	Transformation sphere_transformation = Transformation::createTranslation(-5, 0, 0);
	test_sphere.transformation = sphere_transformation;
	render_context.intersectables.push_back(&test_sphere);

	Light mylight = Light(vec3(0, 0, 0), vec3(1.0, 1.0, 1.0));
	mylight.SHININESS = 25.0f;
	Light mylight2 = Light(vec3(4, 0, -3.0f), vec3(0.0, 0.0, 0.8));
	Light mylight3 = Light(vec3(0, 0, -3.0f), vec3(0.0, 0.8, 0.0));
	render_context.lights.push_back(mylight);
	render_context.lights.push_back(mylight2);
	render_context.lights.push_back(mylight3);
}

/*
int main(void)
{

		/* The framebuffer size needs to be retrieved for glViewport #1#
		int width = width_window, height = height_window;
	

		float ratio;
		ratio = width / (float)height;

		//unsigned int rendersize = 10;
		unsigned int render_x = width_window;
		unsigned int render_y = height_window;
		initializeRenderContext(render_x, render_y);

		renderer = new DebugRenderer();

		const int rgba_amount = render_x*render_y * 3;
		myData = new unsigned char[rgba_amount]; // put this on heap, it's too big, captain
		/*
		void * memset(void * ptr, int value, size_t num);
		Fill block of memory
			Sets the first num bytes of the block of memory pointed by ptr
			to the specified value(interpreted as an unsigned char).
		#1#
		//memset(myData, 0, render_context.nbOfPixels_xDir*render_context.nbOfPixels_yDir * 3);
	
		renderer->Render(render_context, myData);

	return 0;
}*/

int main_static(void)
{

	const int rgba_amount = width_window*height_window * 3;
	myData = new unsigned char[rgba_amount];
	memset(myData, '255', rgba_amount);


	PPMWriter ppmWriter = PPMWriter();
	//ppmWriter.writefile(myData, rgba_amount);
	PPMWriter::writePPMFile2();
	return 0;
}
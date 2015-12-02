#include "Camera.h"
#include "Frustrum.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "DebugRenderer.h"
#include <GLFW/glfw3.h>
#include "Sphere.h"

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

void drawFullsizeQuad()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);		glVertex3f(-1.0, -1.0, 0.0);
	glTexCoord2f(1.0, 0.0);		glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(1.0, 1.0);		glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0, 1.0, 0.0);
	glEnd();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void generateTexture() {
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, render_context.n_x, render_context.n_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, myData);
	glEnable(GL_TEXTURE_2D);
}

void setupTexture() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	generateTexture();
}

void display(void)
{
	
	memset(myData, 0, render_context.n_x*render_context.n_y * 4);
	renderer->Render(render_context, myData);

	generateTexture();
	drawFullsizeQuad();
}


static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

/*
int main(void)
{
	/* ===========================
	/* === Initializing window ===
	/* ===========================#1#
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	/* Initialize the library #1#
	if (!glfwInit())
		exit(EXIT_FAILURE);

	/* Create a windowed mode window and its OpenGL context #1#
	window = glfwCreateWindow(width_window, height_window, windowName, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/* Make the window's context current #1#
	glfwMakeContextCurrent(window);

	/* Make a key callback for the window #1#
	glfwSetKeyCallback(window, key_callback);

	/* Loop until the user closes the window #1#
	while (!glfwWindowShouldClose(window))
	{
		/* Render here #1#

		/* The framebuffer size needs to be retrieved for glViewport #1#
		int width = width_window, height = height_window;
		// This function retrieves the size, in pixels, of the framebuffer of the specified window.
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		float ratio;
		ratio = width / (float)height;

		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

		//unsigned int rendersize = 10;
		unsigned int render_x = width_window;
		unsigned int render_y = height_window;
		initializeRenderContext(render_x, render_y);

		renderer = new DebugRenderer();

		const int rgba_amount = render_x*render_y * 4;
		myData = new unsigned char[rgba_amount]; // put this on heap, it's too big, captain
		generateTexture();
		drawFullsizeQuad();




		/* Swap front and back buffers #1#
		glfwSwapBuffers(window);

		/* Poll for and process events #1#
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}*/
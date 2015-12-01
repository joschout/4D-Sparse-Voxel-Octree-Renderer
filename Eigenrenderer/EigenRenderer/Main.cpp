#include "Camera.h"
#include "Frustrum.h"
#include "RenderContext.h"
#include "RendererManager.h"
#include "DebugRenderer.h"
#include <GLFW/glfw3.h>
#include "Sphere.h"

using std::string;

// viewpoint
Camera camera;
Frustrum frustrum;
RenderContext render_context;

// renderer
RendererManager rmanager;
string rendername;
int current_r = 0;
int lightselector = 0;

unsigned char* myData;

// OpenGL
GLuint texid;

void initializeRenderSystem(unsigned int render_x, unsigned int render_y) {
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
	//Timer t = Timer();
	rendername = rmanager.getCurrentRenderer()->name;
	//camera.computeUVW();

	memset(myData, 0, render_context.n_x*render_context.n_y * 4);
	rmanager.getCurrentRenderer()->Render(render_context, myData);

	generateTexture();
	drawFullsizeQuad();
	//TwDraw();
	//glPopMatrix();
	//glutSwapBuffers(); // necessary?

//	std::stringstream out;
//	out << "Voxelicious v0.2 | Rendertime: " << t.getTimeMilliseconds() << " ms | FPS: " << 1000 / t.getTimeMilliseconds();
//	string s = out.str();
//	glutSetWindowTitle(s.c_str());
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


int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	/* Initialize the library */
	if (!glfwInit())
		exit(EXIT_FAILURE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Simple Example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Make a key callback for the window */
	glfwSetKeyCallback(window, key_callback);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */

		/* The framebuffer size needs to be retrieved for glViewport */
		int width, height;
		// This function retrieves the size, in pixels, of the framebuffer of the specified window.
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		float ratio;
		ratio = width / (float)height;

		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		
		unsigned int rendersize = 10;
		unsigned int render_x = rendersize;
		unsigned int render_y = rendersize;
		initializeRenderSystem(render_x, render_y);

		rmanager = RendererManager();
		DebugRenderer* renderer = new DebugRenderer();
		rmanager.addRenderer(renderer);

		const int rgba_amount = render_x*render_y * 4;
		myData = new unsigned char[rgba_amount]; // put this on heap, it's too big, captain
		generateTexture();
		drawFullsizeQuad();

		
	

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
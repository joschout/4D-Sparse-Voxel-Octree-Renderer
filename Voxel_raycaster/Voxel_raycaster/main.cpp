#include <TriMesh.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <vector>
#include <iostream>

#include "RenderContext.h" 
#include "util.h"
#include "Renderers/DiffuseOctreeRenderer.h"
#include "Renderers/DebugRenderer.h"
#include "Renderers/LevelRenderer.h"
#include "RendererManager.h"
#include "OctreeReader.h"
#include "printInfo.h"
#include "Renderers/DepthRenderer.h"
#include "Renderers/WorkOctreeRenderer.h"
#include "Renderers/NormalRenderer.h"
#include "Renderers/BaseOctreeRenderer.h"
#include "Renderers/TopLevelRenderer.h"
#include "ImGui/imgui_impl_glfw.h"
#include "GLHandler.h"
#include "Renderers/BasicGridRenderer.h"
#include "Grid.h"
#include "Tree4DReader.h"
#include "Parameter_parsing.h"
#include "Initialize_rendering.h"
#include "Renderers/OctreePrinter.h"
#include "Tree4DPrinter.h"
#include "Renderers/BasicTree4DRenderer.h"

FileFormat inputformat = GRID;

/*enum RenderType { octreeT, gridT , tree4DT};//, triangleT };
RenderType typeOfRenderer = octreeT;*/

using namespace std;

// viewpoint
Camera camera;
Frustrum frustrum;
RenderContext render_context;

// renderer
RendererManager rmanager;
string rendername;
int current_r = 0;
int lightselector = 0;

Octree* octree = nullptr;
Grid* grid = nullptr;
Tree4D* tree4D;
BasicGridRenderer gridRenderer;
BasicTree4DRenderer tree4DRenderer;

unsigned char* renderdata;

// OpenGL
GLuint texid;

GLFWwindow* window;
bool showImGuiInfoWindow = true;

void loadOctreeRenderers(){
	rmanager = RendererManager();
	rmanager.addRenderer(new DiffuseOctreeRenderer());
	rmanager.addRenderer(new DebugRenderer());
	rmanager.addRenderer(new BaseOctreeRenderer());
	rmanager.addRenderer(new WorkOctreeRenderer());
	rmanager.addRenderer(new NormalRenderer());
	rmanager.addRenderer(new DepthRenderer());
	rmanager.addRenderer(new LevelRenderer());
	rmanager.addRenderer(new TopLevelRenderer());
	rendername = rmanager.getCurrentRenderer()->name;	
}

// Keyboard
void keyboardfunc(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		//TwEventKeyboardGLUT(key,x,y);
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_KP_0:
			render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(0, -0.1, 0);
			break;
		case GLFW_KEY_KP_2:
			render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(0, 0.1, 0);
			break;
		case GLFW_KEY_KP_1:
			render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(-0.1, 0, 0);
			break;
		case GLFW_KEY_KP_3:
			render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(0.1, 0, 0);
			break;
		case GLFW_KEY_KP_9:
		case GLFW_KEY_KP_SUBTRACT:
			camera.eye = camera.eye + vec3(0, 0, -0.2);
			break;
		case GLFW_KEY_KP_7:
		case GLFW_KEY_KP_ADD:
			camera.eye = camera.eye + vec3(0, 0, 0.2);
			break;
		case GLFW_KEY_KP_6:
		case GLFW_KEY_RIGHT:
			camera.eye = camera.eye + vec3(0.2, 0, 0);
			break;
		case GLFW_KEY_KP_4:
		case GLFW_KEY_LEFT:
			camera.eye = camera.eye + vec3(-0.2, 0, 0);
			break;
		case GLFW_KEY_KP_8:
		case GLFW_KEY_UP:
			camera.eye = camera.eye + vec3(0, 0.2, 0);
			break;
		case GLFW_KEY_KP_5:
		case GLFW_KEY_DOWN:
			camera.eye = camera.eye + vec3(0, -0.2, 0);
			break;
		case GLFW_KEY_W:
			camera.gaze = camera.gaze + vec3(0.2, 0, 0);
			break;
		case GLFW_KEY_S:
			camera.gaze = camera.gaze + vec3(-0.2, 0, 0);
			break;
		case GLFW_KEY_A:
			camera.gaze = camera.gaze + vec3(0, 0.2, 0);
			break;
		case GLFW_KEY_D:
			camera.gaze = camera.gaze + vec3(0, -0.2, 0);
			break;
		case GLFW_KEY_R:
			camera.gaze = camera.gaze + vec3(0, 0, -0.2);
			break;
		case GLFW_KEY_F:
			camera.gaze = camera.gaze + vec3(0, 0, -0.2);
			break;
		case GLFW_KEY_N:
			lightselector = (lightselector + 1) % (render_context.lights.size());
			cout << "light selector:" << lightselector << endl;
			break;
		case GLFW_KEY_P:
			rmanager.switchRenderer();
			break;
		case GLFW_KEY_K:
		{LevelRenderer* lr = dynamic_cast<LevelRenderer*>(rmanager.getRenderer("level"));
		lr->maxlevel = (lr->maxlevel - 1) % (int)(log2(octree->gridlength) + 2);
		cout << "Max level for Level renderer: " << lr->maxlevel << endl;}
		break;
		case GLFW_KEY_L:
		{LevelRenderer* lr = dynamic_cast<LevelRenderer*>(rmanager.getRenderer("level"));
		lr->maxlevel = (lr->maxlevel + 1) % (int)(log2(octree->gridlength) + 2);
		cout << "Max level for Level renderer: " << lr->maxlevel << endl;}
		break;
		case GLFW_KEY_I:
		{string filename = "image" + getTimeString() + "";
		writePPM(render_context.n_x, render_context.n_y, renderdata, filename);
		std::cout << "Image file written: " << filename << ".ppm" << std::endl;}
		break;
		}
	}
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void display(void)
{
	ImGui_ImplGlfw_NewFrame();

	
	if(showImGuiInfoWindow){
		ImGui::Begin("General info", &showImGuiInfoWindow);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Camera eye: x:%.3f, y:%.3f, z:%.3f", camera.eye[0], camera.eye[1], camera.eye[2]);
		ImGui::Text("Camera gaze: x:%.3f, y:%.3f, z:%.3f", camera.gaze[0], camera.gaze[1], camera.gaze[2]);
		switch (inputformat)
		{
		case OCTREE:
			ImGui::Text("Current renderer: %s", rendername.c_str());
			ImGui::Text("Octree: minPoint: x:%.3f, y:%.3f, z:%.3f", octree->min[0], octree->min[1], octree->min[2]);
			ImGui::Text("Octree: maxPoint: x:%.3f, y:%.3f, z:%.3f", octree->max[0], octree->max[1], octree->max[2]);
			ImGui::Text("Octree size (1 direction): %d", octree->gridlength);
			break;
		case GRID:
			ImGui::Text("Grid: minPoint: x:%.3f, y:%.3f, z:%.3f", grid->min[0], grid->min[1], grid->min[2]);
			ImGui::Text("Grid: maxPoint: x:%.3f, y:%.3f, z:%.3f", grid->max[0], grid->max[1], grid->max[2]);
			ImGui::Text("Grid size (1 direction): %d", grid->gridlength);
			break;
		}
		if (ImGui::Button("Reset camera")) {
			initCamera(camera);
		}
		ImGui::End();
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	Timer t = Timer();

	camera.computeUVW();

	memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);

	switch(inputformat)
	{
	case OCTREE:
		rendername = rmanager.getCurrentRenderer()->name;
		rmanager.getCurrentRenderer()->Render(render_context, octree, renderdata);
		break;
	case GRID:
		gridRenderer.Render(render_context, grid, renderdata);
		break;
	case TREE4D:
		tree4DRenderer.Render(render_context, tree4D, renderdata);
	}

	
	generateTexture(texid, render_context, renderdata);
	drawFullsizeQuad();

	//TwDraw();
	//glPopMatrix();
	ImGui::Render();
	glfwSwapBuffers(window);

	std::stringstream out;
	out << "Voxel Renderer | Rendertime: " << t.getTimeMilliseconds() << " ms | FPS: " << 1000 / t.getTimeMilliseconds();
	string s = out.str();
	glfwSetWindowTitle(window, s.c_str());
}

int main(int argc, char **argv) {

	printInfo();
	printCurrentDirectory();
	printControls();

	// Input argument validation
	string datafile = "";
	unsigned int rendersize = 640;
	
	parseParameters(argc,argv,datafile,inputformat,rendersize);
	//datafile should now contain a String: "someFile.octree"
	// inputformat now is OCTREE (0)

	// Initialize render system
	unsigned int render_x = rendersize;
	unsigned int render_y = rendersize;
	initRenderSystem(render_x,render_y, render_context, frustrum, camera);
	
		
	if (inputformat == OCTREE)
	{
		loadOctreeRenderers();
		/*
		Input: datafile = String "someFile.octree"
		octree = pointer to place where an Octree object can be stored
		*/
		readOctree(datafile, octree);
		printOctree2(octree);
		// read the octree from cache

		octree->min = vec3(0, 0, 2);
		octree->max = vec3(2, 2, 0);
		octree->size = vec3(2, 2, 2);
	} 

		
	if (inputformat == TREE4D)
	{
		/*
		Input: datafile = String "someFile.octree"
		octree = pointer to place where an Octree object can be stored
		*/
		readTree4D(datafile, tree4D);
		//printTree4D(tree4D);
		//printTree4D2ToFile(tree4D, "tree4DStructure.txt");
		// read the tree4D from cache

		tree4D->min = vec4(0, 0, 0, 0);
		tree4D->max = vec4(1, 1, 1, 1);
		tree4D->size = vec4(1, 1, 1, 1);
	}
	if(inputformat == GRID)
	{
		grid = new Grid();
		grid->min = vec3(0, 0, 0);
		grid->max = vec3(1, 1, 1);
		grid->gridlength = 16;
		grid->initSparseColorsRand();
	}
	
	

	//cout << "Starting rendering ..." << endl;

	const int rgba_amount = render_x*render_y*4;
	renderdata = new unsigned char[rgba_amount]; // put this on heap, it's too big, captain

	// Init GLFW system
	if (!glfwInit()) exit(EXIT_FAILURE);
	// Init window
	window = glfwCreateWindow(render_x, render_y, "Voxel Ray Caster", NULL, NULL);
	if (!window){glfwTerminate();exit(EXIT_FAILURE);}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, keyboardfunc);

	setupTexture(texid, render_context, renderdata);

	// Setup ImGui binding
	ImGui_ImplGlfw_Init(window, false);
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 2.5;


	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwWaitEvents();
	}

	delete renderdata;
	if(inputformat == OCTREE)
	{
		delete octree;
	}if (inputformat == TREE4D)
	{
		delete tree4D;
	}if(inputformat == GRID)
	{
		delete grid;
	}
	
	ImGui_ImplGlfw_Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
	
}
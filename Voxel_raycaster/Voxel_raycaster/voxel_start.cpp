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
#include "octree_build.h"
#include <AntTweakBar.h>
#include "printInfo.h"
#include "Renderers/DepthRenderer.h"
#include "Renderers/WorkOctreeRenderer.h"
#include "Renderers/NormalRenderer.h"
#include "Renderers/BaseOctreeRenderer.h"
#include "Renderers/TopLevelRenderer.h"
#include "ImGui/imgui_impl_glfw.h"


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

Octree* octree = NULL;
unsigned char* renderdata;

// OpenGL
GLuint texid;

GLFWwindow* window;
bool showImGuiInfoWindow = false;

// Draw fullsize quad, regardless of camera standpoint
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

void generateTexture(){
   glBindTexture(GL_TEXTURE_2D, texid);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,render_context.n_x,render_context.n_y,0,GL_RGBA,GL_UNSIGNED_BYTE, renderdata);
   glEnable(GL_TEXTURE_2D);
}

void setupTexture(){
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glGenTextures(1, &texid);
   glBindTexture(GL_TEXTURE_2D, texid);
   glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   generateTexture();
}

void loadRenderers(){
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
	rendername = rmanager.getCurrentRenderer()->name;
}

// Keyboard
void keyboardfunc(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//TwEventKeyboardGLUT(key,x,y);
	switch (key) {
		case GLFW_KEY_ESCAPE:
			if(action == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
		case GLFW_KEY_KP_0:
			render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(0,-0.1,0);
			break;
		case GLFW_KEY_KP_2:
			render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(0,0.1,0);
			break;
		case GLFW_KEY_KP_1:
			render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(-0.1,0,0);
			break;
		case GLFW_KEY_KP_3:
			render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(0.1,0,0);
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
			lightselector = (lightselector+1) % (render_context.lights.size());
			cout << "light selector:" << lightselector << endl;
			break;
		case GLFW_KEY_P:
			rmanager.switchRenderer();
			break;
		case GLFW_KEY_K:
			{LevelRenderer* lr = dynamic_cast<LevelRenderer*>(rmanager.getRenderer("level"));
			lr->maxlevel = (lr->maxlevel -1) % (int) (log2(octree->gridlength)+2);
			cout << "Max level for Level renderer: " << lr->maxlevel << endl;}
			break;
		case GLFW_KEY_L:
			{LevelRenderer* lr = dynamic_cast<LevelRenderer*>(rmanager.getRenderer("level"));
			lr->maxlevel = (lr->maxlevel + 1) % (int) (log2(octree->gridlength) + 2);
			cout << "Max level for Level renderer: " << lr->maxlevel << endl;}
			break;
		case GLFW_KEY_I:
			{string filename = "image"+getTimeString()+"";
			writePPM(render_context.n_x,render_context.n_y, renderdata, filename);
			std::cout << "Image file written: " << filename << ".ppm" << std::endl;}
			break;
	}
}

void parseParameters(int argc, char **argv, string& file, FileFormat &inputformat, unsigned int& rendersize){
	if(argc < 2){ printInvalid(); exit(0);}
	for (int i = 1; i < argc; i++) {
		if (string(argv[i]) == "-f") {
			file = argv[i + 1]; 
			size_t check_octree = file.find(".octree");
			if(check_octree != string::npos){
				inputformat = OCTREE;
			} else{ 
				cout << "Data filename does not end in .octree - I only support that file format" << endl; 
				printInvalid();
				exit(0);
			}
			i++;
		} else if (string(argv[i]) == "-s") {
			rendersize = atoi(argv[i + 1]); 
			i++;
		} else {printInvalid(); exit(0);}
	}
}

void generateLightTWBars(TwBar* bar){
	TwStructMember lightMembers[] = // array used to describe tweakable variables of the Light structure
	{
		{ "Active",    TW_TYPE_BOOL32, offsetof(Light, active),"" },
		{ "Position",    TW_TYPE_DIR3F, offsetof(Light, position),"" },  
		{ "Diffuse",     TW_TYPE_COLOR3F, offsetof(Light, diffuse),"" },       
		{ "Specular",    TW_TYPE_COLOR3F,   offsetof(Light, specular),"" },
		{ "Constant Attenuation", TW_TYPE_FLOAT, offsetof(Light, CONSTANT_ATTENUATION), "" },
		{ "Linear Attenuation", TW_TYPE_FLOAT, offsetof(Light, LINEAR_ATTENUATION), "" },
		{ "Quadratic Attenuation", TW_TYPE_FLOAT, offsetof(Light, QUADRATIC_ATTENUATION), "" },
		{ "Shininess", TW_TYPE_FLOAT, offsetof(Light, SHININESS), "" }
	};
	TwType lightType = TwDefineStruct("Light", lightMembers, 7, sizeof(Light), NULL, NULL);

	for(int i = 0; i<render_context.lights.size(); i++){
		std::stringstream name;
		name << "Light " << i+1;
		TwAddVarRW(bar, name.str().c_str(), lightType, &render_context.lights[i], "group='Lights'"); // Add a lightType variable and group it into the 'Edit lights' group
		TwSetParam(bar, name.str().c_str(), "label", TW_PARAM_CSTRING, 1, name.str().c_str()); // Set label
	}
}



static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void initCamera()
{
	camera = Camera(vec3(0, 0, -2), vec3(0, 0, -1), vec3(0, 1, 0));
}

void initRenderSystem(unsigned int render_x, unsigned int render_y) {
	initCamera();
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

void display(void)
{
	ImGui_ImplGlfw_NewFrame();
	if(showImGuiInfoWindow){
		ImGui::Begin("General info", &showImGuiInfoWindow);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Camera eye: x:%.3f, y:%.3f, z:%.3f", camera.eye[0], camera.eye[1], camera.eye[2]);
		ImGui::Text("Camera gaze: x:%.3f, y:%.3f, z:%.3f", camera.gaze[0], camera.gaze[1], camera.gaze[2]);

		ImGui::Text("Current renderer: %s", rendername.c_str());




		ImGui::Text("Octree: minPoint: x:%.3f, y:%.3f, z:%.3f", octree->min[0], octree->min[1], octree->min[2]);
		ImGui::Text("Octree: maxPoint: x:%.3f, y:%.3f, z:%.3f", octree->max[0], octree->max[1], octree->max[2]);
		ImGui::Text("Octree size (1 direction): %d", octree->gridlength);
		if (ImGui::Button("Reset camera")) {
			initCamera();
		}
		ImGui::End();
	}


	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	Timer t = Timer();
	rendername = rmanager.getCurrentRenderer()->name;
	camera.computeUVW();

	memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
	rmanager.getCurrentRenderer()->Render(render_context, octree, renderdata);
	generateTexture();
	drawFullsizeQuad();

	//TwDraw();
	//glPopMatrix();
	ImGui::Render();
	glfwSwapBuffers(window);

	glfwSwapBuffers(window);

	std::stringstream out;
	out << "Voxelicious v0.2 | Rendertime: " << t.getTimeMilliseconds() << " ms | FPS: " << 1000 / t.getTimeMilliseconds();
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
	FileFormat inputformat;
	parseParameters(argc,argv,datafile,inputformat,rendersize);

	//datafile should now contain a String: "someFile.octree"
	// inputformat now is OCTREE (0)

	// Initialize render system
	unsigned int render_x = rendersize;
	unsigned int render_y = rendersize;
	initRenderSystem(render_x,render_y);
	loadRenderers();

	if (inputformat == OCTREE)
	{
		/*
			Input: datafile = String "someFile.octree"
			octree = pointer to place where an Octree object can be stored
		*/
		readOctree(datafile, octree);
	} // read the octree from cache
	
	octree->min = vec3(0,0,2);
	octree->max = vec3(2,2,0);
	octree->size = vec3(2,2,2);

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

	setupTexture();

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
	delete octree;
	ImGui_ImplGlfw_Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
	
}
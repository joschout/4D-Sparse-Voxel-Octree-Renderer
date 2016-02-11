//#include <TriMesh.h>
//#include <GL\glew.h>
//#include <GLFW\glfw3.h>
//
//#include <vector>
//#include <iostream>
//
//#include "Camera.h"
//#include "Frustrum.h"
//#include "RenderContext.h"
//#include "Grid.h"
//#include "Renderers/BasicGridRenderer.h"
//#include "util.h"
//#include "ImGui/imgui.h"
//#include "ImGui/imgui_impl_glfw.h"
//#include "printInfo.h"
//#include <ratio>
//#include "Renderers/BasicGridRenderer.h"
//#include "GLHandler.h"
//
//using namespace std;
//
//// viewpoint
//Camera camera;
//Frustrum frustrum;
//RenderContext render_context;
//
//// renderer
//BasicGridRenderer gridRenderer;
//int current_r = 0;
//int lightselector = 0;
//
//
//Grid* grid = nullptr;
//unsigned char* renderdata;
//
//// OpenGL
//GLuint texid;
//
//GLFWwindow* window;
//
//bool showImGuiInfoWindow = true;
//
//
//// Keyboard
//void keyboardfunc(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	//TwEventKeyboardGLUT(key,x,y);
//	switch (key) {
//	case GLFW_KEY_ESCAPE:
//		if(action == GLFW_PRESS)
//		{
//			glfwSetWindowShouldClose(window, GL_TRUE);
//		}
//	case GLFW_KEY_KP_0:
//		render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(0, -0.1, 0);
//		break;
//	case GLFW_KEY_KP_2:
//		render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(0, 0.1, 0);
//		break;
//	case GLFW_KEY_KP_1:
//		render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(-0.1, 0, 0);
//		break;
//	case GLFW_KEY_KP_3:
//		render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(0.1, 0, 0);
//		break;
//	case GLFW_KEY_KP_9:
//	case GLFW_KEY_KP_SUBTRACT:
//		camera.eye = camera.eye + vec3(0, 0, -0.2);
//		break;
//	case GLFW_KEY_KP_7:
//	case GLFW_KEY_KP_ADD:
//		camera.eye = camera.eye + vec3(0, 0, 0.2);
//		break;
//	case GLFW_KEY_KP_6:
//	case GLFW_KEY_RIGHT:
//		camera.eye = camera.eye + vec3(0.2, 0, 0);
//		break;
//	case GLFW_KEY_KP_4:
//	case GLFW_KEY_LEFT:
//		camera.eye = camera.eye + vec3(-0.2, 0, 0);
//		break;
//	case GLFW_KEY_KP_8:
//	case GLFW_KEY_UP:
//		camera.eye = camera.eye + vec3(0, 0.2, 0);
//		break;
//	case GLFW_KEY_KP_5:
//	case GLFW_KEY_DOWN:
//		camera.eye = camera.eye + vec3(0, -0.2, 0);
//		break;
//	case GLFW_KEY_W:
//		camera.gaze = camera.gaze + vec3(0.2, 0, 0);
//		break;
//	case GLFW_KEY_S:
//		camera.gaze = camera.gaze + vec3(-0.2, 0, 0);
//		break;
//	case GLFW_KEY_A:
//		camera.gaze = camera.gaze + vec3(0, 0.2, 0);
//		break;
//	case GLFW_KEY_D:
//		camera.gaze = camera.gaze + vec3(0, -0.2, 0);
//		break;
//	case GLFW_KEY_R:
//		camera.gaze = camera.gaze + vec3(0, 0, -0.2);
//		break;
//	case GLFW_KEY_F:
//		camera.gaze = camera.gaze + vec3(0, 0, -0.2);
//		break;
//	case GLFW_KEY_N:
//		lightselector = (lightselector + 1) % (render_context.lights.size());
//		cout << "light selector:" << lightselector << endl;
//		break;
//	case GLFW_KEY_I:
//	{string filename = "image" + getTimeString() + "";
//	writePPM(render_context.n_x, render_context.n_y, renderdata, filename);
//	std::cout << "Image file written: " << filename << ".ppm" << std::endl;}
//	break;
//	}
//}
//
//void parseParameters(int argc, char **argv, string& file, FileFormat &inputformat, unsigned int& rendersize) {
//	if (argc < 2) { printInvalid(); exit(0); }
//	for (int i = 1; i < argc; i++) {
//		if (string(argv[i]) == "-f") {
//			file = argv[i + 1];
//			size_t check_octree = file.find(".octree");
//			if (check_octree != string::npos) {
//				inputformat = OCTREE;
//			}
//			else {
//				cout << "Data filename does not end in .octree - I only support that file format" << endl;
//				printInvalid();
//				exit(0);
//			}
//			i++;
//		}
//		else if (string(argv[i]) == "-s") {
//			rendersize = atoi(argv[i + 1]);
//			i++;
//		}
//		else { printInvalid(); exit(0); }
//	}
//}
//
//void initCamera()
//{
//	camera = Camera(vec3(0, 0, -2), vec3(0, 0, -1), vec3(0, 1, 0));
//}
//
//void initRenderSystem(unsigned int render_x, unsigned int render_y) {
//	initCamera();
//	float aspect_ratio = render_x / render_y;
//	frustrum = Frustrum(45, aspect_ratio, 1, 100); // THIS near and far SHOULD BE NEGATIVE
//	render_context = RenderContext(&camera, &frustrum, render_x, render_y);
//	Light mylight = Light(vec3(0, 0, 0), vec3(1.0, 1.0, 1.0));
//	mylight.SHININESS = 25.0f;
//	Light mylight2 = Light(vec3(4, 0, -3.0f), vec3(0.0, 0.0, 0.8));
//	Light mylight3 = Light(vec3(0, 0, -3.0f), vec3(0.0, 0.8, 0.0));
//	render_context.lights.push_back(mylight);
//	render_context.lights.push_back(mylight2);
//	render_context.lights.push_back(mylight3);
//}
//
//static void error_callback(int error, const char* description)
//{
//	fputs(description, stderr);
//}
////static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
////{
////	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
////		glfwSetWindowShouldClose(window, GL_TRUE);
////}
//
//
//void display(void)
//{
//
//	ImGui_ImplGlfw_NewFrame();
//	{
//		ImGui::Begin("General info", &showImGuiInfoWindow);
//		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//		ImGui::Text("Camera eye: x:%.3f, y:%.3f, z:%.3f", camera.eye[0], camera.eye[1], camera.eye[2]);
//		ImGui::Text("Camera gaze: x:%.3f, y:%.3f, z:%.3f", camera.gaze[0], camera.gaze[1], camera.gaze[2]);
//
//		ImGui::Text("Grid: minPoint: x:%.3f, y:%.3f, z:%.3f", grid->min[0], grid->min[1], grid->min[2]);
//		ImGui::Text("Grid: maxPoint: x:%.3f, y:%.3f, z:%.3f", grid->max[0], grid->max[1], grid->max[2]);
//		ImGui::Text("Grid size (1 direction): %d", grid->gridlength);
//		if (ImGui::Button("Reset camera")) {
//			initCamera();
//		}
//		ImGui::End();
//	}
//
//
//	//float ratio;
//	int width, height;
//	glfwGetFramebufferSize(window, &width, &height);
//	//ratio = width / (float)height;
//	glViewport(0, 0, width, height);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	Timer t = Timer();
//	camera.computeUVW();
//
//	memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
//	gridRenderer.Render(render_context, grid, renderdata);
//	generateTexture(texid, render_context, renderdata);
//	drawFullsizeQuad();
//
//	//TwDraw();
//	//glPopMatrix();
//	ImGui::Render();
//	glfwSwapBuffers(window);
//
//	std::stringstream out;
//	out << "Voxel Renderer | Rendertime: " << t.getTimeMilliseconds() << " ms | FPS: " << 1000 / t.getTimeMilliseconds();
//	string s = out.str();
//	glfwSetWindowTitle(window, s.c_str());
//}
//
//int main(int argc, char **argv) {
//	printInfo();
//	printCurrentDirectory();
//	printControls();
//
//	// Input argument validation
//	string datafile = "";
//	unsigned int rendersize = 640;
//	//FileFormat inputformat;
//	//parseParameters(argc,argv,datafile,inputformat,rendersize);
//
//	// Initialize render system
//	unsigned int render_x = rendersize;
//	unsigned int render_y = rendersize;
//	initRenderSystem(render_x, render_y);
//
//
//	grid = new Grid();
//	grid->min = vec3(0, 0, 0);
//	grid->max = vec3(1, 1, 1);
//	grid->gridlength = 16;
//	grid->initSparseColorsRand();
//
//
//	//cout << "Starting rendering ..." << endl;
//
//	const int rgba_amount = render_x*render_y * 4;
//	renderdata = new unsigned char[rgba_amount]; // put this on heap, it's too big, captain
//
//												 // Init GLFW system
//	if (!glfwInit()) exit(EXIT_FAILURE);
//	// Init window
//	window = glfwCreateWindow(render_x, render_y, "Voxel Ray Caster", NULL, NULL);
//	if (!window) { glfwTerminate();exit(EXIT_FAILURE); }
//	glfwMakeContextCurrent(window);
//	glfwSwapInterval(1);
//	glfwSetKeyCallback(window, keyboardfunc);
//
//	setupTexture(texid, render_context, renderdata);
//
//	// Setup ImGui binding
//	ImGui_ImplGlfw_Init(window, false);
//	ImGuiIO& io = ImGui::GetIO();
//	io.FontGlobalScale = 2.5;
//
//	while (!glfwWindowShouldClose(window))
//	{
//
//		display();
//		glfwWaitEvents();
//	}
//	delete renderdata;
//	delete grid;
//	ImGui_ImplGlfw_Shutdown();
//	glfwDestroyWindow(window);
//	glfwTerminate();
//	exit(EXIT_SUCCESS);
//
//}
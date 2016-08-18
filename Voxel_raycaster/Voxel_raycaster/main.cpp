#include <TriMesh.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

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
#include "Renderers/DepthTree4DRenderer.h"
#include "Renderers/RenderManager4D.h"
#include "Renderers/WorkTree4DRenderer.h"
#include "Renderers/DiffuseTree4DRenderer.h"
#include "Renderers/TimePoint4DRenderer.h"
#include "Renderers/NormalTree4DRenderer.h"
#include "CameraControl.h"
#include "Renderers/SingleColorTree4DRenderer.h"
#include <iomanip>
#include "Renderers/ColorTree4DRenderer.h"
#include "Renderers/LevelTree4DRenderer.h"
#include "PrintStatusBar.h"
#include "Renderers/DepthOfLeafRenderer.h"
#include "Renderers/PixelLODRenderer.h"
#include "Renderers/LODCheckTree4DRenderer.h"
#include "Renderers/LODWorkTree4DRenderer.h"
#include "Renderers/LODColorTree4DRenderer.h"
#include "Renderers/LODDepthTree4DRenderer.h"
#include "Renderers/LODNormalTree4DRenderer.h"
#include "TestRunner.h"
#include "Renderers/LODLevelTree4DRenderer.h"


//#include <afx.h>
FileFormat inputformat = GRID;

/*enum RenderType { octreeT, gridT , tree4DT};//, triangleT };
RenderType typeOfRenderer = octreeT;*/

using namespace std;

float tt_max;
float tt_min;

bool printTreeStructure = false;


string datafile = "";
// viewpoint
Camera camera;
Frustrum frustrum;
RenderContext render_context;

// renderer
RendererManager rmanager;
RendererManager4D rmanager4D;
TimePoint4DRenderer timePoint4DRenderer;


string rendername;
int current_r = 0;
int lightselector = 0;

Octree* octree = nullptr;
Grid* grid = nullptr;
Tree4D* tree4D;
BasicGridRenderer gridRenderer;

unsigned char* renderdata;

CameraController camera_controller;

// OpenGL
GLuint texid;

GLFWwindow* window;
bool showImGuiInfoWindow = true;
bool showImGuiKeyboardHints = true;

unsigned int render_x;
unsigned int render_y;


size_t max_step_count = 0;

bool debug = false;
bool run_tests = false;
bool aPixelIsSelected = false;
double selected_pixel_x;
double selected_pixel_y;

//measured in screen coordinates but relative to the top-left corner of the window client area
double current_cursor_x_pos;
double current_cursor_y_pos;


#define printNodeStructure

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

void loadTree4DRenderers() {
	rmanager4D = RendererManager4D();

//	rmanager4D.addRenderer(new PixelLODRenderer());
	rmanager4D.addRenderer(new LODDepthTree4DRenderer());
	rmanager4D.addRenderer(new LODColorTree4DRenderer());
	rmanager4D.addRenderer(new LODCheckTree4DRenderer());
	rmanager4D.addRenderer(new LODLevelTree4DRenderer());
	rmanager4D.addRenderer(new LODWorkTree4DRenderer());
	rmanager4D.addRenderer(new LODNormalTree4DRenderer());
	
	rmanager4D.addRenderer(new DepthOfLeafRenderer());
	
	rmanager4D.addRenderer(new SingleColorTree4DRenderer());
	rmanager4D.addRenderer(new DepthTree4DRenderer());
	rmanager4D.addRenderer(new WorkTree4DRenderer());
	rmanager4D.addRenderer(new DiffuseTree4DRenderer());
	rmanager4D.addRenderer(new ColorTree4DRenderer());
	rmanager4D.addRenderer(new NormalTree4DRenderer());
	rmanager4D.addRenderer(new LevelTree4DRenderer(1));

	rendername = rmanager4D.getCurrentRenderer()->name;
}

void setMaxLevelToRender()
{
	LevelTree4DRenderer* lr = dynamic_cast<LevelTree4DRenderer*>(rmanager4D.getRenderer("level"));
	if (lr != nullptr)
	{
		lr->max_level = camera_controller.level_to_render;
	}
	LODColorTree4DRenderer* lodr = dynamic_cast<LODColorTree4DRenderer*>(rmanager4D.getRenderer("LODColor"));
	if (lodr != nullptr)
	{
		lodr->max_level = camera_controller.level_to_render;
	}
	LODWorkTree4DRenderer* lodwr = dynamic_cast<LODWorkTree4DRenderer*>(rmanager4D.getRenderer("LODWork"));
	if (lodwr != nullptr)
	{
		lodwr->max_level = camera_controller.level_to_render;
	}
	LODCheckTree4DRenderer* lodcr = dynamic_cast<LODCheckTree4DRenderer*>(rmanager4D.getRenderer("LODCheck"));
	if (lodcr != nullptr)
	{
		lodcr->max_level = camera_controller.level_to_render;
	}
	LODLevelTree4DRenderer* lodlr = dynamic_cast<LODLevelTree4DRenderer*>(rmanager4D.getRenderer("LODLevel"));
	if (lodlr != nullptr)
	{
		lodlr->max_level = camera_controller.level_to_render;
	}
	LODDepthTree4DRenderer* loddr = dynamic_cast<LODDepthTree4DRenderer*>(rmanager4D.getRenderer("LODDepth"));
	if (loddr != nullptr)
	{
		loddr->max_level = camera_controller.level_to_render;
	}
	LODNormalTree4DRenderer* lodnr = dynamic_cast<LODNormalTree4DRenderer*>(rmanager4D.getRenderer("LODNormal"));
	if (lodnr != nullptr)
	{
		lodnr->max_level = camera_controller.level_to_render;
	}
	DepthOfLeafRenderer* dolr = dynamic_cast<DepthOfLeafRenderer*>(rmanager4D.getRenderer("depthOfLeaf"));
	if (dolr != nullptr)
	{
		dolr->max_level = camera_controller.level_to_render;
	}
}


void writeTimePointRenderImage()
{
	camera.computeUVW();
	memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
	timePoint4DRenderer.Render(render_context, tree4D, renderdata, camera_controller.time_point);
	string filename = datafile + "timepoint_image" + getTimeString() + "";
	writePPM(render_context.n_x, render_context.n_y, renderdata, filename);
	
}

void writePPMImageForEachTimePoint()
{
	camera.computeUVW();
	int nbOfDigitsInDecimal_gridsize = log10(tree4D->gridsize_T) + 1;

	cout << "Writing out PPM for each time point:" << endl;

	for (int time_i = 0; time_i <= tree4D->gridsize_T; time_i++) {
		showProgressBar(time_i, tree4D->gridsize_T, 10);


		//suffix with leading zero's
		stringstream ss;
		ss << setw(nbOfDigitsInDecimal_gridsize) << setfill('0') << time_i;
		string sequence_number_suffix = ss.str();
		
		float current_time_point = 0.0 + time_i * camera_controller.time_step;
		memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
		rendername = rmanager4D.getCurrentRenderer()->name;
		rmanager4D.getCurrentRenderer()->Render(render_context, tree4D, renderdata, current_time_point);
		string filename = datafile + "_seq" + sequence_number_suffix;
		writePPM(render_context.n_x, render_context.n_y, renderdata, filename);
	}
}


// Keyboard
void keyboardfunc(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	camera_controller.keyboardfunc(window, key, scancode, action, mods);
}

// Cursor
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

	int real_width, real_height;
	glfwGetWindowSize(window, &real_width, &real_height);
	//measured in screen coordinates but relative to the top-left corner of the window client area
	//BUT renderer measures from BOTTOM-left
	current_cursor_x_pos = xpos/real_width * render_x;
	current_cursor_y_pos = render_y - ypos / real_height * render_y;//render_y - ypos;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		cout << "current mouse position: x: " << to_string(current_cursor_x_pos) << ", : " << to_string(current_cursor_y_pos) << endl;
	}
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		selected_pixel_x = current_cursor_x_pos;
		selected_pixel_y = current_cursor_y_pos;
		cout << "selected pixel: x: " << to_string(selected_pixel_x) << ", : " << to_string(selected_pixel_y) << endl;
		aPixelIsSelected = true;
	}

}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void display(void)
{
	ImGui_ImplGlfw_NewFrame();

	
	if (showImGuiInfoWindow) {
		ImGui::Begin("General info", &showImGuiInfoWindow);
		ImGui::Text("Data file: %s", datafile.c_str());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Camera eye: x:%.3f, y:%.3f, z:%.3f", camera.eye[0], camera.eye[1], camera.eye[2]);
		ImGui::Text("Camera gaze: x:%.3f, y:%.3f, z:%.3f", camera.getGaze()[0], camera.getGaze()[1], camera.getGaze()[2]);
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
		case TREE4D:
			ImGui::Text("Current renderer: %s", rendername.c_str());
			ImGui::Text("Tree4D: minPoint: x:%.3f, y:%.3f, z:%.3f, t:%.3f", tree4D->min[0], tree4D->min[1], tree4D->min[2], tree4D->min[3]);
			ImGui::Text("Tree4D: maxPoint: x:%.3f, y:%.3f, z:%.3f, t:%.3f", tree4D->max[0], tree4D->max[1], tree4D->max[2], tree4D->max[3]);
			ImGui::Text("gridsize_T: %d", tree4D->gridsize_T);
			ImGui::Text("Time step: %.3f", camera_controller.time_step);
			ImGui::Text("Time point: %.3f", camera_controller.time_point);
			ImGui::Text("Min time for pixel: %.3f", tt_min);
			ImGui::Text("Max time for pixel: %.3f", tt_max);


			ImGui::SliderFloat("time", &(camera_controller.time_point), tree4D->min[3], tree4D->max[3], "%.3f", 1);
			ImGui::SliderFloat("eye movement speed", &(camera_controller.camera_eye_movement_speed), 0, 10, "%.1f", 1);
			ImGui::SliderFloat("gaze movement speed", &(camera_controller.camera_gaze_movement_speed), 0, 10, "%.1f", 1);
			ImGui::SliderFloat("time movement speed", &(camera_controller.time_movement_speed), 0, 10, "%.1f", 1);

			ImGui::SliderFloat("camera_u step", &(camera_controller.space_step_u), 0, 10, "%.1f", 1);
			ImGui::SliderFloat("camera_v step", &(camera_controller.space_step_v), 0, 10, "%.1f", 1);
			ImGui::SliderFloat("camera_w step", &(camera_controller.space_step_w), 0, 10, "%.1f", 1);


			if (ImGui::Button("Depth renderer")) {
				rmanager4D.setCurrentRenderer("depth");
			}
			if (ImGui::Button("#timepoints/pixel renderer")) {
				rmanager4D.setCurrentRenderer("timepoint");
			}
			if (ImGui::Button("write #timepoints/pixel image")) {
				writeTimePointRenderImage();
			}
			if (ImGui::Button("write tree structure")) {
				printTree4D2ToFile_alternate(tree4D, "nodeStructure_tree4d.txt");
			}
			if (ImGui::Button("write timepoint image sequence")) {
				writePPMImageForEachTimePoint();
			}

			break;
		}
		if (ImGui::Button("Reset camera")) {
			initCamera(camera);
		}
		ImGui::End();
	}

	if (showImGuiKeyboardHints) {
		ImGui::Begin("Keyboard hints", &showImGuiKeyboardHints);
		ImGui::Text("Arrow Left: Camera eye X  - 0.2");
		ImGui::Text("Arrow Right: Camera eye X  + 0.2");
		ImGui::Text("Arrow Down: Camera eye Y  - 0.2");
		ImGui::Text("Arrow Up: Camera eye Y  + 0.2");
		ImGui::Text("Keypad - : Camera eye Z  - 0.2");
		ImGui::Text("keypad +: Camera eye Z  + 0.2");
		ImGui::Text("W/S: Camera gaze X  + 0.2 /- 0.2");
		ImGui::Text("A/D: Camera gaze Y  + 0.2 /- 0.2");
		ImGui::Text("R/F: Camera gaze Z  + 0.2/- 0.2");
		ImGui::End();
	}


	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	Timer t = Timer();

	camera.computeUVW();

	
	setMaxLevelToRender();


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
		rendername = rmanager4D.getCurrentRenderer()->name;
		rmanager4D.getCurrentRenderer()->Render(render_context, tree4D, renderdata, camera_controller.time_point);
		break;
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

void runTests()
{
	

	unsigned int rendersize = 1080;// 2160;//640

	render_x = rendersize;
	render_y = rendersize;
	initRenderSystem(render_x, render_y, render_context, frustrum, camera);

	inputformat = TREE4D;

	const int rgba_amount = render_x*render_y * 4;
	renderdata = new unsigned char[rgba_amount]; // put this on heap, it's too big, captain

	loadTree4DRenderers();



	TestRunner test_runner = TestRunner(&camera, &camera_controller, &inputformat, &rmanager4D, tree4D, &render_context, renderdata);
	//test_runner.run_tests_dragon();
	max_step_count = 0;
	//test_runner.run_tests_translating_Suzanne();
	max_step_count = 0;
//	test_runner.run_tests_sphere_rotating();
//	max_step_count = 0;
//	test_runner.run_tests_AEK_24_cell();
//	max_step_count = 0;
//	test_runner.run_tests_exploding_dragon();
//	max_step_count = 0;
//	test_runner.run_tests_marbles();
//	max_step_count = 0;
	test_runner.run_tests_flag();
	max_step_count = 0;
	test_runner.run_tests_sintel_walk_cycle();
	max_step_count = 0;
	test_runner.run_tests_fairy_forest();
	max_step_count = 0;
	test_runner.run_tests_cloth();
	delete renderdata;
	
	rmanager4D.deleteAllRenderers();


	/* regular non-moving dragon

	voor dichtbij en veraf:
	work met LOD
	work zonder LOD
	color met lod
	color zonder lod
	kleur die aangeeft leafnode/lod or not
	level-renderer



	*/




	/*
	2.
	sphere on a circular path
	S 512, T 64
	tree building - number of 8-element queues: 4
	tree building - number of 16-element queues: 6
	tree building - total number of queues: 10
	total number of triangles: 61440
	total nb of voxels in grid: 8589934592
	sparseness (%): 0.999746
	4D SVO - number of nodes: 2854124
	4D SVO - number of data: 2
	---------------------------------------
	camera eye: xyz = -0.406, 0.4, 0.558
	camera gaze: xyz = -1.0, 0.0, 0.0
	largest stack size: 10, smallest stack size: 10


	---------------------------------------
	camear eye: xyz = (-8.006, 0.4, 0.558)
	camera gaze: xyz = (-1.0, 0.0, 0.0)
	largest stack size: 8, smallest stack size: 8
	*/


	// cloth

	// fairy forest

	// sintel walk cycle




}




int main(int argc, char **argv) {

//	afxMemDF |= checkAlwaysMemDF;


	printInfo();
	printCurrentDirectory();
	printControls();

	// Input argument validation
	datafile = "";
	unsigned int rendersize = 1080;//640;// 2160;//640
	
	parseParameters(argc,argv, datafile, inputformat, rendersize, printTreeStructure, debug, run_tests);
	//datafile should now contain a String: "someFile.octree"
	// inputformat now is OCTREE (0)

	// Initialize render system
	render_x = rendersize;
	render_y = rendersize;
	initRenderSystem(render_x,render_y, render_context, frustrum, camera);
	

	cout << "run tests: " << run_tests << endl;
	if(run_tests)
	{
		runTests();
	}else
	{
		
	

		if (inputformat == GRID) {
			grid = new Grid();
			grid->min = vec3_d(0, 0, 0);
			grid->max = vec3_d(1, 1, 1);
			grid->gridlength = 16;
			grid->initSparseColorsRand();
		}
		if (inputformat == OCTREE){
			loadOctreeRenderers();
			/*
			Input: datafile = String "someFile.octree"
			octree = pointer to place where an Octree object can be stored
			*/
			readOctree(datafile, octree);
			// read the octree from cache

			if(printTreeStructure)
			{
				printOctree2ToFile(octree, "nodeStructure_octree.txt");
			}
		
			octree->min = vec3(0, 0, 2);
			octree->max = vec3(2, 2, 0);
			octree->size = vec3(2, 2, 2);
		} 
		if (inputformat == TREE4D){
			loadTree4DRenderers();
			/*
			Input: datafile = String "someFile.octree"
			octree = pointer to place where an Octree object can be stored
			*/
			readTree4D(datafile, tree4D);
			// read the tree4D from cache

			//printTree4D(tree4D);
			if (printTreeStructure)
			{
				printTree4D2ToFile_alternate_different_sides(tree4D, "nodeStructure_tree4d.txt");
			}
	/*		tree4D->min = vec4(0, 0, 2, 0);
			tree4D->max = vec4(2, 2, 0, 1);
			tree4D->size = vec4(2, 2, 2, 1);*/


	//		tree4D->min = vec4_d(0, 0, tree4D->gridsize_S, 0);
	//		tree4D->max = vec4_d(tree4D->gridsize_S, tree4D->gridsize_S, 0, tree4D->gridsize_T);
//			tree4D->min = vec4_d(0, 0, 1, 0);
//			tree4D->max = vec4_d(1, 1, 0, tree4D->gridsize_T);

			tree4D->min = vec4_d(0, 0, 0.25, 0);
			tree4D->max = vec4_d(0.25, 0.25, 0, tree4D->gridsize_T);

			//tree4D->size = vec4(tree4D->gridsize_S, tree4D->gridsize_S, tree4D->gridsize_S, tree4D->gridsize_T);

		

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

		if(debug)
		{
			glfwSetCursorPosCallback(window, cursor_position_callback);
			glfwSetMouseButtonCallback(window, mouse_button_callback);
		}
	

		setupTexture(texid, render_context, renderdata);

		// Setup ImGui binding
		ImGui_ImplGlfw_Init(window, false);
		ImGuiIO& io = ImGui::GetIO();
		io.FontGlobalScale = 2.5;


		camera_controller = CameraController(&camera, &inputformat, &rmanager, &rmanager4D, tree4D, &render_context, renderdata);
		camera_controller.level_to_render = log(max(tree4D->gridsize_S, tree4D->gridsize_T)) / log(2) + 1;
		camera_controller.time_step = abs(tree4D->min[3] - tree4D->max[3]) / tree4D->gridsize_T;

	//	camera.eye = vec3_d(-1.0, 0.0, -2.0);

		while (!glfwWindowShouldClose(window))
		{
			camera_controller.moveCamera();
			display();
			glfwWaitEvents();
		}

		delete renderdata;
		if(inputformat == OCTREE)
		{
			delete octree;
			rmanager.deleteAllRenderers();
		}if (inputformat == TREE4D)
		{
			delete tree4D;
			rmanager4D.deleteAllRenderers();
		}if(inputformat == GRID)
		{
			delete grid;
		}
	
		ImGui_ImplGlfw_Shutdown();

		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
}
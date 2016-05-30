//#include "GUIManager.h"
//#include "ImGui/imgui_impl_glfw.h"
//#include <GLFW/glfw3.h>
//#include "util.h"
//#include "GLHandler.h"
//
//
//void GUIManager::setUp()
//{
//	// Init GLFW system
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
//}
//
//
//void GUIManager::display(void)
//{
//	ImGui_ImplGlfw_NewFrame();
//
//
//	if (showImGuiInfoWindow) {
//		ImGui::Begin("General info", &showImGuiInfoWindow);
//		ImGui::Text("Data file: %s", datafile.c_str());
//		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//		ImGui::Text("Camera eye: x:%.3f, y:%.3f, z:%.3f", camera.eye[0], camera.eye[1], camera.eye[2]);
//		ImGui::Text("Camera gaze: x:%.3f, y:%.3f, z:%.3f", camera.gaze[0], camera.gaze[1], camera.gaze[2]);
//		switch (inputformat)
//		{
//		case OCTREE:
//			ImGui::Text("Current renderer: %s", rendername.c_str());
//			ImGui::Text("Octree: minPoint: x:%.3f, y:%.3f, z:%.3f", octree->min[0], octree->min[1], octree->min[2]);
//			ImGui::Text("Octree: maxPoint: x:%.3f, y:%.3f, z:%.3f", octree->max[0], octree->max[1], octree->max[2]);
//			ImGui::Text("Octree size (1 direction): %d", octree->gridlength);
//			break;
//		case GRID:
//			ImGui::Text("Grid: minPoint: x:%.3f, y:%.3f, z:%.3f", grid->min[0], grid->min[1], grid->min[2]);
//			ImGui::Text("Grid: maxPoint: x:%.3f, y:%.3f, z:%.3f", grid->max[0], grid->max[1], grid->max[2]);
//			ImGui::Text("Grid size (1 direction): %d", grid->gridlength);
//			break;
//		case TREE4D:
//			ImGui::Text("Current renderer: %s", rendername.c_str());
//			ImGui::Text("Tree4D: minPoint: x:%.3f, y:%.3f, z:%.3f, t:%.3f", tree4D->min[0], tree4D->min[1], tree4D->min[2], tree4D->min[3]);
//			ImGui::Text("Tree4D: maxPoint: x:%.3f, y:%.3f, z:%.3f, t:%.3f", tree4D->max[0], tree4D->max[1], tree4D->max[2], tree4D->max[3]);
//			ImGui::Text("Tree4D size (1 direction): %d", tree4D->gridsize_T);
//			ImGui::Text("Time step: %.3f", camera_controller.time_step);
//			ImGui::Text("Time point: %.3f", camera_controller.time_point);
//			ImGui::Text("Min time for pixel: %.3f", tt_min);
//			ImGui::Text("Max time for pixel: %.3f", tt_max);
//
//
//			ImGui::SliderFloat("time", &(camera_controller.time_point), tree4D->min[3], tree4D->max[3], "%.3f", 1);
//			ImGui::SliderFloat("eye movement speed", &(camera_controller.camera_eye_movement_speed), 0, 10, "%.1f", 1);
//			ImGui::SliderFloat("gaze movement speed", &(camera_controller.camera_gaze_movement_speed), 0, 10, "%.1f", 1);
//			ImGui::SliderFloat("time movement speed", &(camera_controller.time_movement_speed), 0, 10, "%.1f", 1);
//			if (ImGui::Button("Depth renderer")) {
//				rmanager4D.setCurrentRenderer("depth");
//			}
//			if (ImGui::Button("#timepoints/pixel renderer")) {
//				rmanager4D.setCurrentRenderer("timepoint");
//			}
//			if (ImGui::Button("write #timepoints/pixel image")) {
//				writeTimePointRenderImage();
//			}
//			if (ImGui::Button("write tree structure")) {
//				printTree4D2ToFile_alternate(tree4D, "nodeStructure_tree4d.txt");
//			}
//
//			break;
//		}
//		if (ImGui::Button("Reset camera")) {
//			initCamera(camera);
//		}
//		ImGui::End();
//	}
//
//	if (showImGuiKeyboardHints) {
//		ImGui::Begin("Keyboard hints", &showImGuiKeyboardHints);
//		ImGui::Text("Arrow Left: Camera eye X  - 0.2");
//		ImGui::Text("Arrow Right: Camera eye X  + 0.2");
//		ImGui::Text("Arrow Down: Camera eye Y  - 0.2");
//		ImGui::Text("Arrow Up: Camera eye Y  + 0.2");
//		ImGui::Text("Keypad - : Camera eye Z  - 0.2");
//		ImGui::Text("keypad +: Camera eye Z  + 0.2");
//		ImGui::Text("W/S: Camera gaze X  + 0.2 /- 0.2");
//		//		ImGui::Text("S: Camera gaze X  - 0.2");
//		ImGui::Text("A/D: Camera gaze Y  + 0.2 /- 0.2");
//		//		ImGui::Text("D: Camera gaze Y  - 0.2");
//		ImGui::Text("R/F: Camera gaze Z  + 0.2/- 0.2");
//		//		ImGui::Text("F: Camera gaze Z  - 0.2");
//		ImGui::End();
//	}
//
//
//	int width, height;
//	glfwGetFramebufferSize(window, &width, &height);
//	glViewport(0, 0, width, height);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	Timer t = Timer();
//
//	camera.computeUVW();
//
//	memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
//
//	switch (inputformat)
//	{
//	case OCTREE:
//		rendername = rmanager.getCurrentRenderer()->name;
//		rmanager.getCurrentRenderer()->Render(render_context, octree, renderdata);
//		break;
//	case GRID:
//		gridRenderer.Render(render_context, grid, renderdata);
//		break;
//	case TREE4D:
//		rendername = rmanager4D.getCurrentRenderer()->name;
//		rmanager4D.getCurrentRenderer()->Render(render_context, tree4D, renderdata, camera_controller.time_point);
//		break;
//	}
//
//
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
//	std::string s = out.str();
//	glfwSetWindowTitle(window, s.c_str());
//}
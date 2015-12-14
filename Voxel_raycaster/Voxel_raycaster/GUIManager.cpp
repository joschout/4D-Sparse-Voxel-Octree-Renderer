//#include "GUIManager.h"
//#include "ImGui/imgui_impl_glfw.h"
//#include <GLFW/glfw3.h>
//
//
//GUIManager::GUIManager()
//{
//}
//
//
//GUIManager::~GUIManager()
//{
//}
//
//void GUIManager::drawFullsizeQuad()
//{
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	glBegin(GL_QUADS);
//	glTexCoord2f(0.0, 0.0);		glVertex3f(-1.0, -1.0, 0.0);
//	glTexCoord2f(1.0, 0.0);		glVertex3f(1.0, -1.0, 0.0);
//	glTexCoord2f(1.0, 1.0);		glVertex3f(1.0, 1.0, 0.0);
//	glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0, 1.0, 0.0);
//	glEnd();
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);
//	glPopMatrix();
//}
//
//void GUIManager::generateTexture(RenderContext* render_context, unsigned char* renderdata) {
//	glBindTexture(GL_TEXTURE_2D, texid);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, render_context->n_x, render_context->n_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, renderdata);
//	glEnable(GL_TEXTURE_2D);
//}
//
//void GUIManager::setupTexture(RenderContext* render_context, unsigned char* renderdata) {
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glShadeModel(GL_FLAT);
//	glGenTextures(1, &texid);
//	glBindTexture(GL_TEXTURE_2D, texid);
//	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	this->generateTexture(render_context, renderdata);
//}
//
//void GUIManager::initializeGUI(void)
//{
//	
//}
//
//
//void GUIManager::display(void)
//{
//
//	ImGui_ImplGlfw_NewFrame();
//	{
//		ImGui::Begin("General info", &showImGuiInfoWindow);
//		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//		ImGui::Text("Camera eye: x:%f, y:%f, z:%f", camera.eye[0], camera.eye[1], camera.eye[2]);
//		ImGui::Text("Camera gaze: x:%f, y:%f, z:%f", camera.gaze[0], camera.gaze[1], camera.gaze[2]);
//		if (ImGui::Button("Reset camera")) {
//			initCamera();
//		}
//		ImGui::End();
//	}
//
//
//	float ratio;
//	int width, height;
//	glfwGetFramebufferSize(window, &width, &height);
//	ratio = width / (float)height;
//	glViewport(0, 0, width, height);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	Timer t = Timer();
//
//	camera.computeUVW();
//
//	memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
//	gridRenderer.Render(render_context, grid, renderdata);
//	generateTexture();
//	drawFullsizeQuad();
//
//	//TwDraw();
//	//glPopMatrix();
//	ImGui::Render();
//	glfwSwapBuffers(window);
//
//	std::stringstream out;
//	out << "Voxelicious v0.2 | Rendertime: " << t.getTimeMilliseconds() << " ms | FPS: " << 1000 / t.getTimeMilliseconds();
//	string s = out.str();
//	glfwSetWindowTitle(window, s.c_str());
//}
//
//
//
//
//
//
//
//void GUIManager::endGUI()
//{
//	
//}
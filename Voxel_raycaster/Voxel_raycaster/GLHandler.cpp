#include <GL\glew.h>
#include "GLHandler.h"


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

void generateTexture(GLuint &texid, RenderContext &render_context, unsigned char* renderdata) {
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, render_context.n_x, render_context.n_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, renderdata);
	glEnable(GL_TEXTURE_2D);
}

void setupTexture(GLuint &texid, RenderContext &render_context, unsigned char* renderdata) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	generateTexture(texid, render_context, renderdata);
}

//void generateLightTWBars(TwBar* bar){
//	TwStructMember lightMembers[] = // array used to describe tweakable variables of the Light structure
//	{
//		{ "Active",    TW_TYPE_BOOL32, offsetof(Light, active),"" },
//		{ "Position",    TW_TYPE_DIR3F, offsetof(Light, position),"" },  
//		{ "Diffuse",     TW_TYPE_COLOR3F, offsetof(Light, diffuse),"" },       
//		{ "Specular",    TW_TYPE_COLOR3F,   offsetof(Light, specular),"" },
//		{ "Constant Attenuation", TW_TYPE_FLOAT, offsetof(Light, CONSTANT_ATTENUATION), "" },
//		{ "Linear Attenuation", TW_TYPE_FLOAT, offsetof(Light, LINEAR_ATTENUATION), "" },
//		{ "Quadratic Attenuation", TW_TYPE_FLOAT, offsetof(Light, QUADRATIC_ATTENUATION), "" },
//		{ "Shininess", TW_TYPE_FLOAT, offsetof(Light, SHININESS), "" }
//	};
//	TwType lightType = TwDefineStruct("Light", lightMembers, 7, sizeof(Light), NULL, NULL);
//
//	for(int i = 0; i<render_context.lights.size(); i++){
//		std::stringstream name;
//		name << "Light " << i+1;
//		TwAddVarRW(bar, name.str().c_str(), lightType, &render_context.lights[i], "group='Lights'"); // Add a lightType variable and group it into the 'Edit lights' group
//		TwSetParam(bar, name.str().c_str(), "label", TW_PARAM_CSTRING, 1, name.str().c_str()); // Set label
//	}
//}
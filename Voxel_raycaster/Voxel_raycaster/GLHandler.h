#ifndef GLHANDLER_H
#define GLHANDLER_H
#include <GL\glew.h>
#include "RenderContext.h"

// Draw fullsize quad, regardless of camera standpoint
void drawFullsizeQuad();

void generateTexture(GLuint &texid, RenderContext &render_context, unsigned char* renderdata);

void setupTexture(GLuint &texid, RenderContext &render_context, unsigned char* renderdata);

//void generateLightTWBars(TwBar* bar);

#endif

#include "DebugRenderer.h"

DebugRenderer::DebugRenderer(void) : Renderer("debug")
{
}

using namespace std;

void DebugRenderer::Render(const RenderContext& rc,  unsigned char* texture_array) const {
	// declare variables we use in loop
	int index, partindex;
	int amountOfColorComponents = 3;
  //#pragma omp parallel for private(x,index)
	for (int y = 0; y < rc.n_y; y++) {

		partindex = y*(rc.n_y * amountOfColorComponents);
		for (int x = 0; x < rc.n_y; x++) {
			
			index = partindex + x * amountOfColorComponents;
			Ray ray = rc.getRayForPixel(x, y);
			Color color = (rc.tracer)->traceRay(rc,ray);
			
			texture_array[index] = static_cast<unsigned char>(color[0]);
			texture_array[index + 1] = static_cast<unsigned char>(color[1]);
			texture_array[index + 2] = static_cast<unsigned char>(color[2]);
		}
	}
}

void DebugRenderer::RenderWork(const RenderContext& rc,  unsigned char* texture_array) const {
	//
}

DebugRenderer::~DebugRenderer(void)
{
}

#ifndef	DEPTHTREE4DRENDERER_H
#define DEPTHTREE4DRENDERER_H
#include "../RenderContext.h"
#include "../Tree4D.h"
#include "Tree4DRenderer.h"

class DepthTree4DRenderer : public Tree4DRenderer

{
public:
	DepthTree4DRenderer(void);
	~DepthTree4DRenderer(void);
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, float time_point) const;

	void DepthTree4DRenderer::calculateAndStoreColorForThisPixel(Tree4D const* tree, unsigned char* texture_array, int indexInTextureArray, vec3 currentPosition) const;
};
#endif
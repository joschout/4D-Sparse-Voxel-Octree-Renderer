#ifndef	NORMALTREE4DRENDERER_H
#define NORMALTREE4DRENDERER_H
#include "../RenderContext.h"
#include "../Tree4D.h"
#include "Tree4DRenderer.h"

class NormalTree4DRenderer : public Tree4DRenderer
{
public:
	NormalTree4DRenderer(void);
	~NormalTree4DRenderer(void);
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const;

	//void NormalTree4DRenderer::calculateAndStoreColorForThisPixel(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, int indexInTextureArray, vec3_d currentPosition) const;
};
#endif

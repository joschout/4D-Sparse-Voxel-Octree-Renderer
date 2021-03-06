#ifndef DEPTHOFLEAFRENDERER_H
#define DEPTHOFLEAFRENDERER_H

#include "../RenderContext.h"
#include "../Tree4D.h"
#include "Tree4DRenderer.h"
#include "../Tree4DTraverserDifferentSides.h"

class DepthOfLeafRenderer : public Tree4DRenderer
{
public:

	size_t max_level;

	DepthOfLeafRenderer(void);
	~DepthOfLeafRenderer(void);
	void calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, Tree4DTraverserDifferentSides &treeTraverser) const;
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const;
};
#endif
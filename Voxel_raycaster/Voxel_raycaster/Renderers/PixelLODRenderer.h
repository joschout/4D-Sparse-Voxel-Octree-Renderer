#ifndef PIXELLODRENDERER_H
#define PIXELLODRENDERER_H

#include "../RenderContext.h"
#include "../Tree4D.h"
#include "Tree4DRenderer.h"
#include "../Tree4DTraverserDifferentSides.h"

class PixelLODRenderer : public Tree4DRenderer
{
public:
	static size_t max_step_count;

	size_t max_level;
	static int stack_size_node_too_small;

	PixelLODRenderer(void);
	~PixelLODRenderer(void);
	void calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, Tree4DTraverserDifferentSides &treeTraverser) const;
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const;
};
#endif
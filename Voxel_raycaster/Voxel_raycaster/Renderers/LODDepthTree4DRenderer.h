#ifndef LODDEPTHTREE4DRENDERER_H
#define LODDEPTHTREE4DRENDERER_H

#include "../RenderContext.h"
#include "../Tree4D.h"
#include "Tree4DRenderer.h"
#include "../Tree4DTraverserDifferentSides.h"
#include "LODTree4DRenderer_Interface.h"

class LODDepthTree4DRenderer : public Tree4DRenderer, LODTree4DRenderer_Interface
{
public:
	static size_t max_level;

	static size_t smallest_stack_size;
	static size_t largest_stack_size;

	LODDepthTree4DRenderer(void);
	~LODDepthTree4DRenderer(void);
	void calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, Tree4D const* tree, Tree4DTraverserDifferentSides &treeTraverser) const;
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const;
	void Render_optimized(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point, double pixel_size) const;

};
#endif
#ifndef LODWORKTREE4DRENDERER_H
#define LODWORKTREE4DRENDERER_H

#include "../RenderContext.h"
#include "../Tree4D.h"
#include "Tree4DRenderer.h"
#include "../Tree4DTraverserDifferentSides.h"

class LODWorkTree4DRenderer : public Tree4DRenderer
{
public:
	static size_t max_step_count; //temp

	static size_t max_level;

	static size_t smallest_stack_size;
	static size_t largest_stack_size;

	LODWorkTree4DRenderer(void);
	~LODWorkTree4DRenderer(void);
	void calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, Tree4DTraverserDifferentSides &treeTraverser) const;
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const;
	void Render_test(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const;
};
#endif
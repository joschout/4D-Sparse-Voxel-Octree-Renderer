#ifndef LODCHECKTREE4DRENDERER_H
#define LODCHECKTREE4DRENDERER_H

#include "../RenderContext.h"
#include "../Tree4D.h"
#include "Tree4DRenderer.h"
#include "../Tree4DTraverserDifferentSides.h"

class LODCheckTree4DRenderer : public Tree4DRenderer
{
public:
	static size_t max_level;

	static size_t smallest_stack_size;
	static size_t largest_stack_size;

	LODCheckTree4DRenderer(void);
	~LODCheckTree4DRenderer(void);
	void calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index_in_texture_array, bool dataLeafNodeHasBeenFound, bool reachedMaxLevelToRender, bool nodeIsToSmall) const;
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const;
};
#endif

#ifndef LEVELTREE4DRENDERER_H
#define LEVELTREE4DRENDERER_H

#include "../RenderContext.h"
#include "../Tree4D.h"
#include "../Tree4DTraverserDifferentSides.h"
#include "Tree4DRenderer.h"

class LevelTree4DRenderer : public Tree4DRenderer
{
public:
	static size_t max_step_count;

	size_t max_level;

	LevelTree4DRenderer(size_t max_level);
	~LevelTree4DRenderer(void);
	void calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, Tree4D const* tree, Tree4DTraverserDifferentSides &treeTraverser) const;
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point)const;
};
#endif

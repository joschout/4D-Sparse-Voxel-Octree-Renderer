#ifndef WORKTREE4DRENDERER_H
#define WORKTREE4DRENDERER_H
#include "../RenderContext.h"
#include "../Tree4D.h"
#include "Tree4DRenderer.h"
#include "../Tree4DTraverserDifferentSides.h"

class WorkTree4DRenderer : public Tree4DRenderer
{
public:
//	static size_t max_step_count;


	WorkTree4DRenderer(void);
	~WorkTree4DRenderer(void);
	void calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, Tree4DTraverserDifferentSides &treeTraverser) const;
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const;
};
#endif
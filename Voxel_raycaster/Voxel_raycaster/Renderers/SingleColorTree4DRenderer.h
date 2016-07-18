#ifndef SINGLECOLORTREE4DRENDERER_H_
#define SINGLECOLOR4DRENDERER_H_
#include "Tree4DRenderer.h"
#include "../Tree4D.h"


class Tree4DTraverserDifferentSides;

class SingleColorTree4DRenderer : public Tree4DRenderer
{
public:
	SingleColorTree4DRenderer(void);
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const;
	~SingleColorTree4DRenderer(void);
	void calculateAndStoreColorForThisPixel(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, int index, Tree4DTraverserDifferentSides &treeTraverser) const;
};

#endif /* DIFFUSEOCTREERENDERER_H_ */
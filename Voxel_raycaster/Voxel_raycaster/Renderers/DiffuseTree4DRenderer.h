#ifndef DIFFUSETREE4DRENDERER_H_
#define DIFFUSETREE4DRENDERER_H_
#include "Tree4DRenderer.h"
#include "../Tree4D.h"


class Tree4DTraverserDifferentSides;

class DiffuseTree4DRenderer : public Tree4DRenderer
{
public:
	DiffuseTree4DRenderer(void);
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, float time_point) const;
	~DiffuseTree4DRenderer(void);
	void calculateAndStoreColorForThisPixel(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, int index, Tree4DTraverserDifferentSides &treeTraverser) const;
};

#endif /* DIFFUSEOCTREERENDERER_H_ */
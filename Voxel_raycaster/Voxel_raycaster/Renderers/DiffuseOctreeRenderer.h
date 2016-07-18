#ifndef DIFFUSEOCTREERENDERER_H_
#define DIFFUSEOCTREERENDERER_H_
#include "OctreeRenderer.h"
#include "../Octree.h"
#include "../TreeTraverser.h"


class DiffuseOctreeRenderer : public OctreeRenderer
{
public:
	DiffuseOctreeRenderer(void);
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	~DiffuseOctreeRenderer(void);
	void calculateAndStoreColorForThisPixel(RenderContext const& rc, Octree const* tree, unsigned char* texture_array, int index, TreeTraverser& treeTraverser) const;
};

#endif /* DIFFUSEOCTREERENDERER_H_ */
#ifndef DIFFUSEOCTREERENDERER_H_
#define DIFFUSEOCTREERENDERER_H_
#include "OctreeRenderer.h"
#include "../Octree.h"


class DiffuseOctreeRenderer : public OctreeRenderer
{
public:
	DiffuseOctreeRenderer(void);
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	~DiffuseOctreeRenderer(void);
};

#endif /* DIFFUSEOCTREERENDERER_H_ */
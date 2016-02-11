#ifndef BASEOCTREERENDERER_H_
#define BASEOCTREERENDERER_H_
#include "OctreeRenderer.h"
#include "../Octree.h"


class BaseOctreeRenderer : public OctreeRenderer
{
public:
	BaseOctreeRenderer(void);
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	~BaseOctreeRenderer(void);
};

#endif /* BASEOCTREERENDERER_H_ */
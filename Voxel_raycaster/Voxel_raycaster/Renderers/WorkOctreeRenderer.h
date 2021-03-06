#ifndef WORKOCTREERENDERER_H_
#define WORKOCTREERENDERER_H_
#include "OctreeRenderer.h"
#include "../Octree.h"

class WorkOctreeRenderer : public OctreeRenderer
{
public:
	WorkOctreeRenderer(void);
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	~WorkOctreeRenderer(void);
};

#endif /* WORKOCTREERENDERER_H_ */
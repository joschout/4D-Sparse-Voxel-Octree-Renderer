#ifndef DEBUGRENDERER_H_
#define DEBUGRENDERER_H_
#include "OctreeRenderer.h"
#include "../Octree.h"

class DebugRenderer : public OctreeRenderer
{
public:
	DebugRenderer(void);
	void Render(RenderContext const& rc,  Octree const* tree, unsigned char* texture_array) const;
	void RenderWork(RenderContext const& rc,  Octree const* tree, unsigned char* texture_array) const;
	~DebugRenderer(void);
};

#endif /* SIMPLEOCTREERENDERER_H_ */
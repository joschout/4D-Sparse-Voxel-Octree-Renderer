#ifndef DEPTHRENDERER_H_
#define DEPTHRENDERER_H_
#include "OctreeRenderer.h"
#include "../Octree.h"


class DepthRenderer : public OctreeRenderer
{
public:
	DepthRenderer(void);
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	~DepthRenderer(void);
};

#endif /* DEPTHRENDERER_H_ */
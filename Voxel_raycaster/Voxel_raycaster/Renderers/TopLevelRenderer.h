#ifndef TOPLEVELRENDERER_H_
#define TOPLEVELRENDERER_H_
#include "OctreeRenderer.h"
#include "../Octree.h"

class TopLevelRenderer : public OctreeRenderer
{
public:

	int maxlevel;

	TopLevelRenderer();
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	std::string getName() const;
	~TopLevelRenderer(void);
};

#endif /* TOPLEVELRENDERER_H_ */
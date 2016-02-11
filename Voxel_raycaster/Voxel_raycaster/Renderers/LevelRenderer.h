#ifndef LEVELRENDERER_H_
#define LEVELRENDERER_H_
#include "OctreeRenderer.h"
#include "../Octree.h"

class LevelRenderer : public OctreeRenderer
{
public:

	int maxlevel;

	LevelRenderer();
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	std::string getName() const;
	~LevelRenderer(void);
};

#endif /* LEVELRENDERER_H_ */
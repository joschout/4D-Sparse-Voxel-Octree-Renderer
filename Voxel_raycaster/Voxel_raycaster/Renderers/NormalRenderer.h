#ifndef NORMALRENDERER_H_
#define NORMALRENDERER_H_
#include "Renderer.h"
#include "../Octree.h"


class NormalRenderer : public Renderer
{
public:
	NormalRenderer(void);
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	~NormalRenderer(void);
};

#endif /* NORMALRENDERER_H_ */
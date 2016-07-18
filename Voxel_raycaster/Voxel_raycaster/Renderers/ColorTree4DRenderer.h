#ifndef	COLORTREE4DRENDERER_H
#define COLORTREE4DRENDERER_H
#include "../RenderContext.h"
#include "../Tree4D.h"
#include "Tree4DRenderer.h"

class ColorTree4DRenderer : public Tree4DRenderer
{
public:
	ColorTree4DRenderer(void);
	~ColorTree4DRenderer(void);
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const;
};
#endif
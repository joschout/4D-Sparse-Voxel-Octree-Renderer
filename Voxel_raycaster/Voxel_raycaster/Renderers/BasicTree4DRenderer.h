#ifndef	BASICTREE4DRENDERER_H
#define BASICTREE4DRENDERER_H
#include "../RenderContext.h"
#include "../Tree4D.h"

class BasicTree4DRenderer
{
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array) const;
};
#endif
#ifndef WORKTREE4DRENDERER_H
#define WORKTREE4DRENDERER_H
#include "../RenderContext.h"
#include "../Tree4D.h"
#include "Tree4DRenderer.h"

class WorkTree4DRenderer : public Tree4DRenderer
{
public:
	WorkTree4DRenderer(void);
	~WorkTree4DRenderer(void);
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, float time_point) const;
};
#endif
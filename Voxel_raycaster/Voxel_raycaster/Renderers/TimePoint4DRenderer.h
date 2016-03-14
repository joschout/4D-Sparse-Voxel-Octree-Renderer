#ifndef TIMEPOINT4DRENDERER
#define TIMEPOINT4DRENDERER

#include "../RenderContext.h"
#include "../Tree4D.h"
#include "Tree4DRenderer.h"

class TimePoint4DRenderer : public Tree4DRenderer

{
public:
	TimePoint4DRenderer(void);
	~TimePoint4DRenderer(void);
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, float time_point) const;
};

#endif

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
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const;
	void calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, double nbOfTimeSteps, double maxNumberOfTimeSteps) const;

};

#endif

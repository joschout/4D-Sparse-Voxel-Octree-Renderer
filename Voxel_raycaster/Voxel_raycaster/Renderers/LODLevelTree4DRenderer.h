#ifndef LODLEVELTREE4DRENDERER_H
#define LODLEVELTREE4DRENDERER_H

#include "../RenderContext.h"
#include "../Tree4D.h"
#include "Tree4DRenderer.h"
#include "../Tree4DTraverserDifferentSides.h"
#include "LODTree4DRenderer_Interface.h"

class LODLevelTree4DRenderer : public Tree4DRenderer, public LODTree4DRenderer_Interface
{
public:
	static size_t max_level;

	static size_t smallest_stack_size;
	static size_t largest_stack_size;

	LODLevelTree4DRenderer(void);
	~LODLevelTree4DRenderer(void);
	void calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, size_t current_stack_size, size_t max_stack_size) const;
	void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const;
	void Render_optimized(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point, double pixel_size) const;
};
#endif

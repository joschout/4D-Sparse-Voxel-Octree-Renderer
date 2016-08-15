#ifndef	LODTREE4DRENDERER_INTERFACE_H
#define LODTREE4DRENDERER_INTERFACE_H

#include "../Tree4D.h"
#include "../RenderContext.h"

class LODTree4DRenderer_Interface {

public :
	virtual ~LODTree4DRenderer_Interface()
	{
	}

	virtual void Render_optimized(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point, double pixel_size) const = 0;

};


#endif

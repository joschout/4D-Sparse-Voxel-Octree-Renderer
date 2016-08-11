#ifndef NODEHITCHECKER_H
#define NODEHITCHECKER_H
#include "RenderContext.h"
#include "Tree4DTraverserDifferentSides.h"


class PixelNodeChecker
{
private:
	vector<Ray4D> corrected_pixel_corner_rays;

public: 
	


	PixelNodeChecker(int pixel_x, int pixel_y, RenderContext const& rc, double time_point, Tree4D const* tree4);

	bool hasNodeBeenHit(TraversalInfo_About_Node4D const& node_info);
	vector<Ray4D> correctRaysForNegativeDirectionComponents(vector<Ray4D>& rays_to_correct, Tree4D const* tree4D);
	static void safelyCalculateRayParametersForDirection(Ray4D ray, vec4_d& t0, vec4_d& t1, vec4_d node_min, vec4_d node_max);
};



#endif

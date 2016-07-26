#ifndef NODEHITCHECKER_H
#define NODEHITCHECKER_H
#include "RenderContext.h"
#include "Tree4DTraverserDifferentSides.h"

bool hasNodeBeenHit(int pixel_x, int pixel_y, RenderContext const& rc, TraversalInfo_About_Node4D const& node_info, double time_point, Tree4D const* tree4D);
vector<Ray4D> correctRaysForNegativeDirectionComponents(vector<Ray4D>& rays_to_correct, Tree4D const* tree4D);
void safelyCalculateRayParametersForDirection(Ray4D ray, vec4_d& t0, vec4_d& t1, vec4_d node_min, vec4_d node_max);

#endif

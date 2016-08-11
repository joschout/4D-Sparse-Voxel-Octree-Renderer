#include "NodeHitChecker.h"



PixelNodeChecker::PixelNodeChecker(int pixel_x, int pixel_y, RenderContext const& rc, double time_point, Tree4D const* tree4D)
{
	vector<Ray> pixel_corner_rays = rc.getPixelCornerRays(pixel_x, pixel_y);
	vector<Ray4D> pixel_corner_rays_4D;
	for (size_t i = 0; i < pixel_corner_rays.size(); i++)
	{
		pixel_corner_rays_4D.push_back(Ray4D::convertRayTo4D(pixel_corner_rays[i], time_point, 0.0f));
	}

	corrected_pixel_corner_rays = correctRaysForNegativeDirectionComponents(pixel_corner_rays_4D, tree4D);


}
bool PixelNodeChecker::hasNodeBeenHit(TraversalInfo_About_Node4D const& node_info)
{

	bool nodeCoversPixel = true;

	for (size_t i = 0; i < corrected_pixel_corner_rays.size(); i++)
	{
//		cout << "pixel corner ray direction: " << corrected_pixel_corner_rays[i].direction << endl;
		vec4_d t0, t1;
		safelyCalculateRayParametersForDirection(corrected_pixel_corner_rays[i], t0, t1, node_info.min, node_info.max);
		nodeCoversPixel  
			= nodeCoversPixel 
			&& max(max(max(t0[0], t0[1]), t0[2]), t0[3]) < min(min(min(t1[0], t1[1]), t1[2]), t1[3]);
	}
	return nodeCoversPixel;

}

vector<Ray4D> PixelNodeChecker::correctRaysForNegativeDirectionComponents(vector<Ray4D>& rays_to_correct, Tree4D const * tree4D)
{
	vector<Ray4D> corrected_rays;

	for (size_t i = 0; i < rays_to_correct.size(); i++)
	{
		Ray4D corrected_ray(rays_to_correct[i]);
		if (corrected_ray.direction[0] < 0.0f) {
			/*
			IF the ray direction is negative in the X coordinate
			THEN reflect the ray with respect to the middle plane of the 4D-tree

			*/

			//		ray.origin[0] = tree4D->size[0] - ray.origin[0];
			corrected_ray.origin[0] = abs(tree4D->min[0] - tree4D->max[0]) - corrected_ray.origin[0];
			corrected_ray.direction[0] = -corrected_ray.direction[0];
			//		a |= 4; //bitwise OR (latest bits are XYZ)
			// a = a OR 0000 0100
		}
		if (corrected_ray.direction[1] < 0.0f) {
			//		ray.origin[1] = tree4D->size[1] - ray.origin[1];
			corrected_ray.origin[1] = abs(tree4D->min[1] - tree4D->max[1]) - corrected_ray.origin[1];
			corrected_ray.direction[1] = -corrected_ray.direction[1];
			//		a |= 2;
			// a = a OR 0000 0010
		}
		if (corrected_ray.direction[2] > 0.0f) {
			//		ray.origin[2] = tree4D->size[2] - ray.origin[2];
			corrected_ray.origin[2] = abs(tree4D->min[2] - tree4D->max[2]) - corrected_ray.origin[2];
			corrected_ray.direction[2] = -corrected_ray.direction[2];
			//		a |= 1;
			// a = a OR 0000 0001
		}
		if (corrected_ray.direction[3] < 0.0f) {
			//		ray.origin[3] = tree4D->size[3] - ray.origin[3];
			corrected_ray.origin[3] = abs(tree4D->min[3] - tree4D->max[3]) - corrected_ray.origin[3];
			corrected_ray.direction[3] = -corrected_ray.direction[3];
			//		a |= 8;
			// a = a OR 0000 1000
		}

		corrected_rays.push_back(corrected_ray);
	}
	
	return corrected_rays;
}



void PixelNodeChecker::safelyCalculateRayParametersForDirection(Ray4D ray, vec4_d& t0, vec4_d& t1, vec4_d node_min, vec4_d node_max)
{
	for (size_t coord = 0; coord < 4; coord++)
	{
		double denominator = ray.direction[coord];
		double numerator_0 = node_min[coord] - ray.origin[coord];
		double numerator_1 = node_max[coord] - ray.origin[coord];

		if (denominator == 0.0f) {
			//OPGEPAST

			//numerator_0;
			if (numerator_0 > 0)
			{
				t0 = std::numeric_limits<double>::infinity();
			}
			else
			{
				if (numerator_0 < 0)
				{
					t0 = -1 * std::numeric_limits<double>::infinity();
				}
				else // numerator_0 == 0.0f
					 // tree4D->min[coord] = ray.origin[coord];
				{
					t0 = -1 * std::numeric_limits<double>::infinity();
					/*			if(numerator_1 > 0)
					{
					t0 = -1 * std::numeric_limits<double>::infinity();
					}else
					{
					t0 = std::numeric_limits<double>::infinity();
					}*/
				}
			}

			// numerator_1
			if (numerator_1 > 0)
			{
				t1 = std::numeric_limits<double>::infinity();
			}
			else
			{
				if (numerator_1 < 0)
				{
					t1 = -1 * std::numeric_limits<double>::infinity();
				}
				else // numerator_0 == 0.0f
					 // tree4D->min[coord] = ray.origin[coord];
				{
					t1 = -1 * std::numeric_limits<double>::infinity();
					/*				if(numerator_0 < 0)
					{
					t1 = std::numeric_limits<double>::infinity();
					}else
					{

					}*/
				}
			}
		}
		else // denominator != 0.0f
		{
			t0 = numerator_0 / denominator;
			t1 = numerator_1 / denominator;
		}
	}
	
}


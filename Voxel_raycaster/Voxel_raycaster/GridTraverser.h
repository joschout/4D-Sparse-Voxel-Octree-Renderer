#ifndef GRIDTRAVERSER_H
#define GRIDTRAVERSER_H

#include "Grid.h"
#include "Ray.h"

const double	kHugeValue = 1.0E10;

class GridTraverser
{
public:
	Grid const* grid;
	Ray ray;

	GridTraverser();
	GridTraverser(Grid const* grid, Ray ray);
	~GridTraverser();
		
	bool GridTraverser::hit( ivec3& foundColor);

private:
	void initializeTraverser();

	void calculateTBorders(float& tx_min, float& ty_min, float& tz_min,
	                       float& tx_max, float& ty_max, float& tz_max);

	static void setup_GridTraversalParameters(
		const Ray& ray,
		float tx_min, float ty_min, float tz_min,
		int ix, int iy, int iz,
		int nx, int ny, int nz,
		float dtx, float dty, float dtz,
		double& tx_next, double& ty_next, double& tz_next,
		int& ix_step, int& iy_step, int& iz_step,
		int& ix_stop, int& iy_stop, int& iz_stop);
};
#endif


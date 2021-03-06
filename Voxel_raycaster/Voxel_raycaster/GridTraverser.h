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
		
	bool GridTraverser::hit(vec3_d& foundColor);

private:
	void initializeTraverser();

	void calculateTBorders(double& tx_min, double& ty_min, double& tz_min,
	                       double& tx_max, double& ty_max, double& tz_max);

	static void setup_GridTraversalParameters(
		const Ray& ray,
		double tx_min, double ty_min, double tz_min,
		int ix, int iy, int iz,
		int nx, int ny, int nz,
		double dtx, double dty, double dtz,
		double& tx_next, double& ty_next, double& tz_next,
		int& ix_step, int& iy_step, int& iz_step,
		int& ix_stop, int& iy_stop, int& iz_stop);
};
#endif


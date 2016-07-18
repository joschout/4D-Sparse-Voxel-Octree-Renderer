#include "GridTraverser.h"
#include "misc_math.h"
#include <algorithm>

using std::max;
using std::min;

GridTraverser::GridTraverser()
{
}

GridTraverser::GridTraverser(Grid const * grid, Ray ray): grid(grid) , ray(ray)
{

}


GridTraverser::~GridTraverser()
{
}


void GridTraverser::initializeTraverser()
{
	
	
	//bereken de intervallen van t-waardes
}

void GridTraverser::setup_GridTraversalParameters(const Ray& ray, 
	double tx_min, double ty_min, double tz_min,
	int ix, int iy, int iz,
	int nx, int ny, int nz,
	double dtx, double dty, double dtz,
	double& tx_next, double& ty_next, double& tz_next,
	int& ix_step, int& iy_step, int& iz_step,
	int& ix_stop, int& iy_stop, int& iz_stop)
{
	if (ray.direction[0] > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}

	if (ray.direction[0] == 0.0) {
		tx_next = kHugeValue;
		ix_step = -1;
		ix_stop = -1;
	}


	if (ray.direction[1] > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}

	if (ray.direction[1] == 0.0) {
		ty_next = kHugeValue;
		iy_step = -1;
		iy_stop = -1;
	}

	if (ray.direction[2] > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}

	if (ray.direction[2] == 0.0) {
		tz_next = kHugeValue;
		iz_step = -1;
		iz_stop = -1;
	}
}

bool
GridTraverser::hit( vec3_d& foundColor) {

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	// the following code includes modifications from Shirley and Morley (2003)
	

	calculateTBorders(tx_min, ty_min, tz_min, tx_max, ty_max, tz_max);

	// check of de intervallen kloppen
	double t0, t1;
	t0 = max(max(tx_min, ty_min), tz_min);
	t1 = min(min(tx_max, ty_max), tz_max);
	if (t0 > t1) {
		return(false);
	}

	// initial cell coordinates
	int ix, iy, iz;

	int nx = grid->gridlength;
	int ny = grid->gridlength;
	int nz = grid->gridlength;


	if (grid->inside(ray.origin)) {  			// does the ray start inside the grid?
		ix = clampf((ray.origin[0] - grid->min[0]) * nx / (grid->max[0] - grid->min[0]), 0, nx - 1);
		iy = clampf((ray.origin[1] - grid->min[1]) * ny / (grid->max[1] - grid->min[1]), 0, ny - 1);
		iz = clampf((ray.origin[2] - grid->min[2]) * nz / (grid->max[2] - grid->min[2]), 0, nz - 1);
	}
	else {
		vec3_d p = ray.origin + (t0 * ray.direction);  // initial hit point with grid's bounding box
		ix = clampf((p[0] - grid->min[0]) * nx / (grid->max[0] - grid->min[0]), 0, nx - 1);
		iy = clampf((p[1] - grid->min[1]) * ny / (grid->max[1] - grid->min[1]), 0, ny - 1);
		iz = clampf((p[2] - grid->min[2]) * nz / (grid->max[2] - grid->min[2]), 0, nz - 1);
	}


	/*
	SETUP FOR GRID TRAVERSAL
	*/
	// ray parameter increments per cell in the x, y, and z directions
	/*
		even though the intersections of the ray with the cell faces are unequally spaced along the ray,
		the intersections in the x-, y- and z-directions are equally spaced.
	*/
	double dtx = (tx_max - tx_min) / nx;
	double dty = (ty_max - ty_min) / ny;
	double dtz = (tz_max - tz_min) / nz;

	/*
		tx_next = the t value where the ray hits the next x-face
		ty_next = the t value where the ray hits the next y-face
		tz_next = the t value where the ray hits the next z-face

		ix_stop : tells the algorithm when the ray is about to exit the grid.
	*/
	double 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;
	setup_GridTraversalParameters(
		ray, tx_min, ty_min, tz_min,
		ix, iy, iz,
		nx, ny, nz,
		dtx, dty, dtz,
		tx_next, ty_next, tz_next,
		ix_step, iy_step, iz_step,
		ix_stop, iy_stop, iz_stop);


	// traverse the grid

	while (true) {
		GridPoint gridPoint = grid->gridCells[ix + nx * iy + nx * ny * iz];
//		ivec3_d color = grid->gridCells[ix + nx * iy + nx * ny * iz];
		
//		return true;
		if (tx_next < ty_next && tx_next < tz_next) {
//			if (object_ptr && object_ptr->hit(ray, t, sr) && t < tx_next) {
//				material_ptr = object_ptr->get_material();
//				return (true);
//			}
			if (!gridPoint.isEmpty()) {
				foundColor = gridPoint.color;
				return (true);
			}


			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop)
				return (false);
		}
		else {
			if (ty_next < tz_next) {
//				if (object_ptr && object_ptr->hit(ray, t, sr) && t < ty_next) {
//					material_ptr = object_ptr->get_material();
//					return (true);
//				}
				if (!gridPoint.isEmpty()) {
					foundColor = gridPoint.color;
					return (true);
				}

				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop)
					return (false);
			}
			else {
//				if (object_ptr && object_ptr->hit(ray, t, sr) && t < tz_next) {
//					material_ptr = object_ptr->get_material();
//					return (true);
//				}
				if (!gridPoint.isEmpty()) {
					foundColor = gridPoint.color;
					return (true);
				}

				tz_next += dtz;
				iz += iz_step;

				if (iz == iz_stop)
					return (false);
			}
		}
	}
}	// end of hit

void
GridTraverser::calculateTBorders(
	double& tx_min, double& ty_min, double& tz_min,
	double& tx_max, double& ty_max, double&tz_max)
{
	double a = 1.0 / ray.direction[0];
	if (a >= 0) {
		tx_min = (grid->min[0] - ray.origin[0]) * a;
		tx_max = (grid->max[0] -ray.origin[0]) * a;
	}
	else {
		tx_min = (grid->max[0] - ray.origin[0]) * a;
		tx_max = (grid->min[0] - ray.origin[0]) * a;
	}

	double b = 1.0 / ray.direction[1];
	if (b >= 0) {
		ty_min = (grid->min[1] - ray.origin[1]) * b;
		ty_max = (grid->max[1] - ray.origin[1]) * b;
	}
	else {
		ty_min = (grid->max[1] - ray.origin[1]) * b;
		ty_max = (grid->min[1] - ray.origin[1]) * b;
	}

	double c = 1.0 / ray.direction[2];
	if (c >= 0) {
		tz_min = (grid->min[2] - ray.origin[2]) * c;
		tz_max = (grid->max[2] - ray.origin[2]) * c;
	}
	else {
		tz_min = (grid->max[2] - ray.origin[2]) * c;
		tz_max = (grid->min[2] - ray.origin[2]) * c;
	}
}

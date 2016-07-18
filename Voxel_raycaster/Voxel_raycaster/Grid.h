#ifndef GRID_H
#define GRID_H

#include <TriMesh.h>
#include "GridPoint.h"

using namespace trimesh;

class Grid
{
public:
	// Octree position/size
	vec3_d min; //minimal point of the octree cube
	vec3_d max; //maximal point of the octree cube
	size_t gridlength; //The length of one side of the cubical voxel grid. Should be a power of 2

	
	//std::vector<ivec3_d> gridCells;
	std::vector<GridPoint> gridCells;


	Grid();

	Grid(vec3_d min, vec3_d max, vec3_d size, size_t gridlength);
	~Grid();

	bool inside(const vec3_d & point) const;

	void initGridColorsRand();
	void initSparseColorsRand();
};

inline Grid::Grid(vec3_d min, vec3_d max, vec3_d size, size_t gridlength)
	: min(min), max(max), gridlength(gridlength) {
}

inline Grid::Grid() : min(vec3_d(0, 0, 0)), max(vec3_d(1, 1, 1)), gridlength(128) {
}


#endif

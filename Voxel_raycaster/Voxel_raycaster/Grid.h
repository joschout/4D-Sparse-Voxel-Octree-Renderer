#ifndef GRID_H
#define GRID_H

#include <TriMesh.h>
#include "GridPoint.h"

using namespace trimesh;

class Grid
{
public:
	// Octree position/size
	vec3 min; //minimal point of the octree cube
	vec3 max; //maximal point of the octree cube
	size_t gridlength; //The length of one side of the cubical voxel grid. Should be a power of 2

	
	//std::vector<ivec3> gridCells;
	std::vector<GridPoint> gridCells;


	Grid();

	Grid(vec3 min, vec3 max, vec3 size, size_t gridlength);
	~Grid();

	bool inside(const vec3 & point) const;

	void initGridColorsRand();
	void initSparseColorsRand();
};

inline Grid::Grid(vec3 min, vec3 max, vec3 size, size_t gridlength)
	: min(min), max(max), gridlength(gridlength) {
}

inline Grid::Grid() : min(vec3(0, 0, 0)), max(vec3(1, 1, 1)), gridlength(128) {
}


#endif

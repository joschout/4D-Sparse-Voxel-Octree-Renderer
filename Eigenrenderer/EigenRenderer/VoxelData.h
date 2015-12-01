#ifndef VOXELDATA_H_
#define VOXELDATA_H_

#include <TriMesh.h>
using trimesh::vec3;

const size_t VOXELDATA_SIZE = sizeof(VoxelData);

/*
	This struct defines VoxelData for our voxelizer.
	This is the main memory hogger: the less data you store here, the better.
*/
struct VoxelData {

	//z-order curve number or morton
	uint64_t morton;
	// color of the voxel
	vec3 color;
	//normal of the voxel
	vec3 normal;

	VoxelData() : morton(0), normal(vec3()), color(vec3()) {}
	VoxelData(uint64_t morton, vec3 normal, vec3 color) : morton(morton), normal(normal), color(color) {}

	bool operator >(VoxelData &a) {
		return morton > a.morton;
	}

	bool operator <(VoxelData &a) {
		return morton < a.morton;
	}
};

#endif // VOXELDATA_H_

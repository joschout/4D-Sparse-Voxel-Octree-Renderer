#ifndef VOXELDATA_H_
#define VOXELDATA_H_

#include <TriMesh.h>
#include <stdint.h>
#include "util.h"

using namespace trimesh;
using namespace std;

// sizeof(VoxelData would work too)
const size_t VOXELDATA_SIZE = sizeof(uint64_t)+2 * (3 * sizeof(float));

// This struct defines VoxelData for our voxelizer.
// This is the main memory hogger: the less data you store here, the better.
struct VoxelData{
	uint64_t morton;
	vec3_d color;
	vec3_d normal;
	
	VoxelData() : morton(0), normal(vec3_d()), color(vec3_d()){}
	VoxelData(uint64_t morton, vec3_d normal, vec3_d color) : morton(morton), color(color), normal(normal){}

	bool operator > (VoxelData &a) const{
		return morton > a.morton;
	}

	bool operator < (VoxelData &a) const{
		return morton < a.morton;
	}

	void print() const{
		cout << "VoxelData:" << endl
			<< "  morton: " << morton << endl
			<< "  color: " << color << endl
			<< "  normal: " << normal << endl;
	}
};


struct VoxelData_float
{
	uint64_t morton;
	vec3 color;
	vec3 normal;

	static VoxelData toVoxelData(VoxelData_float& voxel_data_float)
	{
		return VoxelData(
			voxel_data_float.morton,
			vec3_d(static_cast<double>(voxel_data_float.normal[0]), static_cast<double>(voxel_data_float.normal[1]), static_cast<double>(voxel_data_float.normal[2])),
			vec3_d(static_cast<double>(voxel_data_float.color[0]), static_cast<double>(voxel_data_float.color[1]), static_cast<double>(voxel_data_float.color[2])));
	}

};
#endif // VOXELDATA_H_
#ifndef OCTREE_BUILD_H_
#define OCTREE_BUILD_H_
#pragma once

#include "Octree.h"
#include "octree_io.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

void readOctreeData(OctreeInfo const &octree_info, VoxelData** data);
void readOctreeNodes(OctreeInfo const &octree_info, std::vector<Node> &nodes);
int readOctreeNodes(Octree* octree, std::string filename);
int readOctreeHeader(Octree* octree, std::string filename);
int readOctree(std::string basefilename, Octree*& octree);

#endif /* OCTREE_H_ */
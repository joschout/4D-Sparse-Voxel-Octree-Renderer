#ifndef TREE4DREADER_H
#define TREE4DREADER_h
#include <string>
#include "Tree4D.h"
#include "tree4d_io.h"


int readTree4D(const std::string& basefilename, Tree4D*& tree4D);

//int readTree4DHeader(const std::string& basefilename, Tree4D*& tree4D));
void readTree4DNodes(Tree4DInfo const &tree4d_info, std::vector<Node4D> &nodes);
//void readTree4DData(Tree4DInfo const &tree4d_info, VoxelData** data);
void readTree4DData(Tree4DInfo const &tree4d_info, std::vector<unique_ptr<VoxelData>> &data);

#endif

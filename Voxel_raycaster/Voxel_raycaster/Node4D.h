#ifndef NODE4D_H_
#define NODE4D_H_

#pragma once

#include "VoxelData.h"

#define NOCHILD -1
#define NODATA 0

// This is how an array of a leaf node will look
const char LEAF4D[16] = { NOCHILD, NOCHILD, NOCHILD, NOCHILD, NOCHILD, NOCHILD, NOCHILD, NOCHILD,
NOCHILD, NOCHILD, NOCHILD, NOCHILD, NOCHILD, NOCHILD, NOCHILD, NOCHILD };

// An SVO node. Only contains child pointers, extend this if you want parent pointers as well
class Node4D
{
public:
	size_t data;
	size_t children_base;
	char children_offset[16];

	VoxelData data_cache; // only if you want to refine octree (clustering)

	Node4D();
	bool hasChild(unsigned int i) const;
	size_t getChildPos(unsigned int i) const;
	bool isLeaf() const;
	bool hasData() const;
	bool isNull() const;
	bool isBinaryNode() const;
	bool isOctaryNode() const;
};

// Default constructor
inline Node4D::Node4D() : data(NODATA), children_base(0), data_cache(VoxelData()) {
	memset(children_offset, (char)NOCHILD, 16);
}

// Check if this Node has a child at position i
inline bool Node4D::hasChild(unsigned int i) const {
	return !(children_offset[i] == NOCHILD);
}

// Get the full index of the child at position i
inline size_t Node4D::getChildPos(unsigned int i) const {
	if (children_offset[i] == NOCHILD) {
		return 0;
	}
	else {
		return children_base + children_offset[i];
	}
}

// If this node doesn't have data and is a leaf node, it's a null node
inline bool Node4D::isNull() const {
	return isLeaf() && !hasData();
}

// If this node doesn;t have any children, it's a leaf node
inline bool Node4D::isLeaf() const {
	if (memcmp(children_offset, LEAF4D, 16 * sizeof(char)) == 0) {
		return true;
	}
	return false;
}

// If the data pointer is NODATA, there is no data
inline bool Node4D::hasData() const {
	return !(data == NODATA);
}


inline bool Node4D::isBinaryNode() const
{
	if (children_base == 0)
	{
		return false;
	}
	for (int i = 0; i < 7; i++)
	{
		if (children_offset[i] != children_offset[i + 1])
		{
			return false;
		}
	}
	for (int i = 8; i < 15; i++)
	{
		if (children_offset[i] != children_offset[i + 1])
		{
			return false;
		}
	}
	return true;
}

inline bool Node4D::isOctaryNode() const
{
	//	if (children_base == 0)
	//	{
	//		return false;
	//	}
	//	for (int i = 0; i < 7; i++)
	//	{
	//		if (children_offset[2 * i] != children_offset[2 * i + 1])
	//		{
	//			return false;
	//		}
	//	}
	//	return true;

	if (children_base == 0)
	{
		return false;
	}
	for (int i = 0; i < 8; i++)
	{
		if (children_offset[i] != children_offset[8 + i])
		{
			return false;
		}
	}
	return true;
}

#endif /* NODE_H_ */

#ifndef TRIE4D_h
#define TRIE4D_h

#include "Node4D.h"
#include <TriMesh.h>
#include <vector>

using trimesh::vec4;

class Tree4D
{
public:
	// Tree4D position/size
	vec4 min; //minimal point of the tree4D cube
	vec4 max; //maximal point of the tree4D cube
	//vec4 size; // unnecessary -> vloeit voort uit min en max
	size_t gridsize_S; //The length of one side of the cubical voxel grid. Should be a power of 2
	size_t gridsize_T;

	/*
	n_nodes (n): (int) The total amount of SVO nodes.
	n_data (n): (int) The total amount of data payloads.
	This is not automatically the same as n_nodes,
	you can have several nodes point to the same data.
	In the case of a geometry-only SVO,
	all nodes refer to the same voxel payload, at position 1*/
	// Octree stats
	size_t n_nodes;
	size_t n_data;

	// The data
	VoxelData* data;

	// The octree nodes
	std::vector<Node4D> nodes;

	//Tree4D(vec4 min, vec4 max, vec4 size, size_t gridsize_S, size_t gridsize_T);
	Tree4D(vec4 min, vec4 max, size_t gridsize_S, size_t gridsize_T);
	Tree4D();
	size_t storeNode(Node4D n);
	const Node4D* getNode(size_t index) const;
	const Node4D* getRootNode() const;
	Node4D* getNode(size_t index);
	Node4D* getRootNode();
	~Tree4D(void);
};

inline Tree4D::Tree4D(vec4 min, vec4 max, size_t gridsize_S, size_t gridsize_T)
	: min(min), max(max), gridsize_S(gridsize_S), gridsize_T(gridsize_T),
	n_nodes(0), n_data(0), data(nullptr)
{
	nodes.push_back(Node4D()); // push back NULL node
}

inline Tree4D::Tree4D()
	: min(vec4(0, 0, 0, 0)), max(vec4(1, 1, 1, 1)),
	gridsize_S(128), gridsize_T(128), n_nodes(0), n_data(0), data(nullptr)
{
}

/*inline Tree4D::Tree4D(vec4 min, vec4 max, vec4 size, size_t gridsize_S, size_t gridsize_T)
	: min(min), max(max), size(size), gridsize_S(gridsize_S), gridsize_T(gridsize_T),
	n_nodes(0), n_data(0), data(nullptr)
{
	nodes.push_back(Node4D()); // push back NULL node
}

inline Tree4D::Tree4D() 
	: min(vec4(0, 0, 0, 0)), max(vec4(1, 1, 1, 1)), size(vec4(1, 1, 1, 1)),
	gridsize_S(128), gridsize_T(128), n_nodes(0), n_data(0), data(nullptr)
{
}*/

inline Tree4D::~Tree4D(void) {
}


inline size_t Tree4D::storeNode(Node4D n) {
	nodes.push_back(n);
	return nodes.size() - 1;
}

inline const Node4D* Tree4D::getNode(size_t index) const {
	return &nodes[index];
}

inline const Node4D* Tree4D::getRootNode() const {
	return &nodes.back();
}

inline Node4D* Tree4D::getNode(size_t index) {
	return &nodes[index];
}

inline Node4D* Tree4D::getRootNode() {
	return &nodes.back();
}


#endif

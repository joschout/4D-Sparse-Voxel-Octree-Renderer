#ifndef TREETRAVERSER_H_
#define TREETRAVERSER_H_

#pragma once
#include "Octree.h"
#include <vector>
#include "Ray.h"
//#include "util.h"
//#include <math.h>
//#include <iostream>

/*
Based on "An Efficient Parameric Algorithm for Octree Traversal
*/
// the struct we will use to communicate current traversal info
struct TraversalNodeInfo_ {
	const Node* node;
	vec3_d t0,t1,tm;
	int nextchild;
};

class TreeTraverser
{
public:
	unsigned char a;
	Octree const* octree;
	Ray ray;
	Ray original_ray;
	std::vector<TraversalNodeInfo_> stack;
	int stepcount;

	TreeTraverser(void);
	TreeTraverser(Octree const* octree, Ray ray);
	void step();
	bool isTerminated() const;
	const Node* getCurrentNode() const;
	vec3_d getCurrentPosition() const;
	~TreeTraverser(void);

private:
	TraversalNodeInfo_ buildNodeInfo(double tx0, double ty0, double tz0, double tx1, double ty1, double tz1, const Node* node);
	int newNode(double txm, int x, double tym, int y, double tzm, int z);
	int firstNode(double tx0, double ty0, double tz0, double txm, double tym, double tzm);
	void initTraversal();
};

inline TreeTraverser::TreeTraverser(void){
}

inline TreeTraverser::TreeTraverser(Octree const* octree, Ray ray): octree(octree), ray(ray){
	initTraversal();
}

inline int TreeTraverser::newNode(double txm, int x, double tym, int y, double tzm, int z){
	if(txm < tym){
		if(txm < tzm)
		{ // txm minimal
			return x;
		} // YZ plane
	} else {
		if(tym < tzm)
		{ // tym minimal
			return y;
		} // XZ plane
	}
	return z; // XY plane;
}

inline TraversalNodeInfo_ TreeTraverser::buildNodeInfo(double tx0, double ty0, double tz0, double tx1, double ty1, double tz1, const Node* node){
	TraversalNodeInfo_ info;
	info.node = node;
	info.t0 = vec3_d(tx0,ty0,tz0);
	info.t1 = vec3_d(tx1,ty1,tz1);
	info.nextchild = -1;
	return info;
}

inline bool TreeTraverser::isTerminated() const{
	return (stack.empty());
}

inline vec3_d TreeTraverser::getCurrentPosition() const{
	double t = stack.back().t0.max();
	// TODO: better implementation, use a.
	vec3_d answer = original_ray.getRayPoint(t);
	answer[2] = -answer[2];
	return answer;
}

inline const Node* TreeTraverser::getCurrentNode() const{
	return stack.back().node;
}

#endif

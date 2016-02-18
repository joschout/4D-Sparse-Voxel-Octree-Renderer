#ifndef TREE4DTRAVERSER_H_
#define TREE4DTRAVERSER_H_

#include "Tree4D.h"
#include <vector>
#include "Ray.h"

/*
Based on "An Efficient Parameric Algorithm for Octree Traversal
*/
// the struct we will use to communicate current traversal info
struct TraversalNode4DInfo_ {
	const Node4D* node;
	vec4 t0, t1, tm;
	int nextchild;
};

class Tree4DTraverser
{
public:
	unsigned char a;
	Tree4D const* tree4D;
	Ray ray;
	Ray original_ray;
	std::vector<TraversalNode4DInfo_> stack;
	int stepcount;

	Tree4DTraverser(void);
	Tree4DTraverser(Tree4D const* octree, Ray ray);
	void step();
	bool isTerminated() const;
	const Node4D* getCurrentNode() const;
	vec3 getCurrentPosition() const;
	~Tree4DTraverser(void);

private:
	TraversalNode4DInfo_ buildNodeInfo(float tx0, float ty0, float tz0, float tt0, float tx1, float ty1, float tz1, float tt1, const Node4D* node);
	int newNode(float txm, int x, float tym, int y, float tzm, int z);
	int firstNode(float tx0, float ty0, float tz0, float txm, float tym, float tzm);
	void initTraversal();
};

inline Tree4DTraverser::Tree4DTraverser(void) {
}

inline Tree4DTraverser::Tree4DTraverser(Tree4D const* tree4D, Ray ray) : tree4D(tree4D), ray(ray) {
	initTraversal();
}

inline int Tree4DTraverser::newNode(float txm, int x, float tym, int y, float tzm, int z) {
	if (txm < tym) {
		if (txm < tzm) { return x; }  // YZ plane
	}
	else {
		if (tym < tzm) { return y; } // XZ plane
	}
	return z; // XY plane;
}

inline TraversalNode4DInfo_ Tree4DTraverser::buildNodeInfo(
	float tx0, float ty0, float tz0, float tt0,
	float tx1, float ty1, float tz1, float tt1,
	const Node4D* node)
{
	TraversalNode4DInfo_ info;
	info.node = node;
	info.t0 = vec4(tx0, ty0, tz0, tt0);
	info.t1 = vec4(tx1, ty1, tz1, tt1);
	info.nextchild = -1;
	return info;
}

inline bool Tree4DTraverser::isTerminated() const {
	return (stack.empty());
}

inline vec3 Tree4DTraverser::getCurrentPosition() const {
	float t = stack.back().t0.max();
	// TODO: better implementation, use a.
	vec3 answer = original_ray.getRayPoint(t);
	answer[2] = -answer[2];
	return answer;
}

inline const Node4D* Tree4DTraverser::getCurrentNode() const {
	return stack.back().node;
}

#endif
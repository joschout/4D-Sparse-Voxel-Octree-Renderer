#ifndef TREE4DTRAVERSER_H_
#define TREE4DTRAVERSER_H_

#include "Tree4D.h"
#include <vector>
#include "Ray4D.h"

/*
Based on "An Efficient Parameric Algorithm for Octree Traversal
*/
// the struct we will use to communicate current traversal info
struct TraversalNode4DInfo_ {
	const Node4D* node;
	vec4_d t0, t1, tm;
	int nextchild;
	vec4_d min, max;
};

class Tree4DTraverser
{
public:
	unsigned char a;
	Tree4D const* tree4D;
	Ray4D ray;
	Ray4D original_ray;
	std::vector<TraversalNode4DInfo_> stack;
	int stepcount;

	Tree4DTraverser(void);
	Tree4DTraverser(Tree4D const* octree, Ray4D ray);
	void step();
	bool isTerminated() const;
	const Node4D* getCurrentNode() const;
	vec3 getCurrentPosition() const;
	~Tree4DTraverser(void);

private:
	TraversalNode4DInfo_ buildNodeInfo(
		double tx0, double ty0, double tz0, double tt0,
		double tx1, double ty1, double tz1, double tt1,
		vec4_d min, vec4_d max,
		const Node4D* node);
	TraversalNode4DInfo_ buildNodeInfo(
		int nextChildNumber,
		vec4_d &t0, vec4_d &t1, vec4_d &tm,
		vec4_d min, vec4_d max,
		const Node4D* node);
	int newNode(
		double txm, int x,
		double tym, int y,
		double tzm, int z,
		double ttm, int t);
	int newNode(int nextChildNumber, vec4_d &t0, vec4_d &t1, vec4_d &tm);
	int firstNode(
		double tx0, double ty0, double tz0, double tt0,
		double txm, double tym, double tzm, double ttm);
	void initTraversal();
	void initRayParameters(int coord, double &t0, double &t1);
	 vec4_d calculateMidpoint(vec4_d &t0, vec4_d &t1);
};

inline Tree4DTraverser::Tree4DTraverser(void) {
}

inline Tree4DTraverser::Tree4DTraverser(Tree4D const* tree4D, Ray4D ray):
	tree4D(tree4D), ray(ray) {
	initTraversal();
}

inline bool Tree4DTraverser::isTerminated() const {
	return (stack.empty());
}

inline vec3 Tree4DTraverser::getCurrentPosition() const {
	double t = stack.back().t0.max();
	// TODO: better implementation, use a.
	vec4_d answer4D = original_ray.getRayPoint(t);
	vec3 answer = vec3(answer4D[0], answer4D[1], answer4D[2]);
	answer[2] = -answer[2];
	return answer;
}

inline const Node4D* Tree4DTraverser::getCurrentNode() const {
	return stack.back().node;
}

#endif
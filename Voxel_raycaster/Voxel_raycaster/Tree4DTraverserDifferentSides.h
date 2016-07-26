#ifndef TREE4DTRAVERSERDIFFERENTSIDES_H_
#define TREE4DTRAVERSERDIFFERENTSIDES_H_

#include "Tree4D.h"
#include <vector>
#include "Ray4D.h"
#include "Tree4DUtils.h"

/*
Based on "An Efficient Parameric Algorithm for Octree Traversal
*/
// the struct we will use to communicate current traversal info
struct TraversalInfo_About_Node4D {
	const Node4D* node;
	vec4_d t0, t1, tm;
	int nextChildToCheck;
	vec4_d min, max;

	AmountOfChildren maxAmountOfChildren;

	//true if this node had max 2 child nodes
//	bool isBinary;
};

class Tree4DTraverserDifferentSides
{
public:
	//If the original ray had negative components, a corrects which child node we have to travers next.
	unsigned char a;
	Tree4D const* tree4D;
	Ray4D ray;
	Ray4D original_ray;
	LongestDimension longestDimension;
	std::vector<TraversalInfo_About_Node4D> stack_TraversalInfo_about_Node4Ds;
	int stepcount;

	bool hasBeenHitByRay = false;


	Tree4DTraverserDifferentSides(void);
	Tree4DTraverserDifferentSides(Tree4D const* octree, Ray4D ray);
	void step();
	bool isTerminated() const;
	const Node4D* getCurrentNode() const;
	TraversalInfo_About_Node4D getCurrentNodeInfo();

	vec3_d getCurrentPosition() const;
	~Tree4DTraverserDifferentSides(void);

//	static AmountOfChildren getMaxAmountOfChildren(const Node4D* node, const LongestDimension& longest_dimention);

private:
	static TraversalInfo_About_Node4D buildNodeInfo_struct(
		double tx0, double ty0, double tz0, double tt0,
		double tx1, double ty1, double tz1, double tt1,
		vec4_d min, vec4_d max,
		const Node4D* node,
		AmountOfChildren maxAmountOfChildren);
	TraversalInfo_About_Node4D buildNodeInfo_16(
		int nextChildNumber,
		vec4_d &t0, vec4_d &t1, vec4_d &tm,
		vec4_d min, vec4_d max,
		const Node4D* node);
	TraversalInfo_About_Node4D buildNodeInfo_8(
		int nextChildNumber,
		vec4_d &t0, vec4_d &t1, vec4_d &tm,
		vec4_d min, vec4_d max,
		const Node4D* node);
	TraversalInfo_About_Node4D buildNodeInfo_2(
		int nextChildNumber,
		vec4_d &t0, vec4_d &t1, vec4_d &tm,
		vec4_d min, vec4_d max,
		const Node4D* node);
	TraversalInfo_About_Node4D buildNodeInfo_general(
		int nextChildNumber,
		vec4_d &t0, vec4_d &t1, vec4_d &tm,
		vec4_d min, vec4_d max,
		const Node4D* node,
		AmountOfChildren maxAmountOfChildrenOfParent);
	
	// Next child node to check
	int nextChildNodeToCheck_16(
		double txm, int x,
		double tym, int y,
		double tzm, int z,
		double ttm, int t);
	int nextChildNodeToCheck_8(
		double txm, int x,
		double tym, int y,
		double tzm, int z);
//	int nextChildNodeToCheck_2(
//		double txm, int x,
//		double tym, int y,
//		double tzm, int z);
	int nextChildNodeToCheck_16(int nextChildNumber, vec4_d &t0, vec4_d &t1, vec4_d &tm);
	int nextChildNodeToCheck_8(int nextChildNumber, vec4_d &t0, vec4_d &t1, vec4_d &tm);
	int nextChildNodeToCheck_2(int nextChildNumber, vec4_d &t0, vec4_d &t1, vec4_d &tm);
	int nextChildNodeToCheck(int currentNextChildNumber, vec4_d &t0, vec4_d &t1, vec4_d &tm);
	
	//first child node to check
	static int firstChildNodeToCheck_16(
		double tx0, double ty0, double tz0, double tt0,
		double txm, double tym, double tzm, double ttm);
	static int firstChildNodeToCheck_8(
		double tx0, double ty0, double tz0, double tt0,
		double txm, double tym, double tzm, double ttm);
//	static int firstChildNodeToCheck_2(
//		double tx0, double ty0, double tz0, double tt0,
//		double txm, double tym, double tzm, double ttm);
	int firstChildNodeToCheck(
		double tx0, double ty0, double tz0, double tt0,
		double txm, double tym, double tzm, double ttm);
	
	vec4_d calculateMidpoint(vec4_d &t0, vec4_d &t1);
	
	// Initialization of the tree traverser
	void inititializeBeforeTraversal();
	void correctRayForNegativeDirectionComponents();
	void safelyCalculateInitialRayParameters(
		double &tx0, double &tx1, double &ty0, double &ty1,
		double &tz0, double &tz1, double &tt0, double &tt1);
	void safelyCalculateRayParametersForDirection(int coord, double &t0, double &t1);

//	AmountOfChildren getMaxAmountOfChildren(const Node4D *node) const;
	
};

inline Tree4DTraverserDifferentSides::Tree4DTraverserDifferentSides(void): a(0), tree4D(nullptr), longestDimension(TIME), stepcount(0)
{
}

inline Tree4DTraverserDifferentSides::Tree4DTraverserDifferentSides(Tree4D const* tree4D, Ray4D ray) :
	tree4D(tree4D), ray(ray) {
	inititializeBeforeTraversal();
}

inline bool Tree4DTraverserDifferentSides::isTerminated() const {
	return (stack_TraversalInfo_about_Node4Ds.empty());
}

inline vec3_d Tree4DTraverserDifferentSides::getCurrentPosition() const {
	double t = stack_TraversalInfo_about_Node4Ds.back().t0.max();
	// TODO: better implementation, use a.
	vec4_d answer4D = original_ray.getRayPoint(t);
	vec3_d answer = vec3_d(answer4D[0], answer4D[1], answer4D[2]);
	answer[2] = -answer[2];
	return answer;
}

inline const Node4D* Tree4DTraverserDifferentSides::getCurrentNode() const {
	return stack_TraversalInfo_about_Node4Ds.back().node;
}

inline  TraversalInfo_About_Node4D Tree4DTraverserDifferentSides::getCurrentNodeInfo() {
	return stack_TraversalInfo_about_Node4Ds.back();
}
#endif
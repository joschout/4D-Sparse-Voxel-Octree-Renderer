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
	vec4 t0, t1, tm;
	int nextChildToCheck;
	vec4 min, max;

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

	Tree4DTraverserDifferentSides(void);
	Tree4DTraverserDifferentSides(Tree4D const* octree, Ray4D ray);
	void step();
	bool isTerminated() const;
	const Node4D* getCurrentNode() const;
	TraversalInfo_About_Node4D getCurrentNodeInfo();

	vec3 getCurrentPosition() const;
	~Tree4DTraverserDifferentSides(void);

//	static AmountOfChildren getMaxAmountOfChildren(const Node4D* node, const LongestDimension& longest_dimention);

private:
	static TraversalInfo_About_Node4D buildNodeInfo_struct(
		float tx0, float ty0, float tz0, float tt0,
		float tx1, float ty1, float tz1, float tt1,
		vec4 min, vec4 max,
		const Node4D* node,
		AmountOfChildren maxAmountOfChildren);
	TraversalInfo_About_Node4D buildNodeInfo_16(
		int nextChildNumber,
		vec4 &t0, vec4 &t1, vec4 &tm,
		vec4 min, vec4 max,
		const Node4D* node);
	TraversalInfo_About_Node4D buildNodeInfo_8(
		int nextChildNumber,
		vec4 &t0, vec4 &t1, vec4 &tm,
		vec4 min, vec4 max,
		const Node4D* node);
	TraversalInfo_About_Node4D buildNodeInfo_2(
		int nextChildNumber,
		vec4 &t0, vec4 &t1, vec4 &tm,
		vec4 min, vec4 max,
		const Node4D* node);
	TraversalInfo_About_Node4D buildNodeInfo_general(
		int nextChildNumber,
		vec4 &t0, vec4 &t1, vec4 &tm,
		vec4 min, vec4 max,
		const Node4D* node,
		AmountOfChildren maxAmountOfChildrenOfParent);
	
	// Next child node to check
	int nextChildNodeToCheck_16(
		float txm, int x,
		float tym, int y,
		float tzm, int z,
		float ttm, int t);
	int nextChildNodeToCheck_8(
		float txm, int x,
		float tym, int y,
		float tzm, int z);
	int nextChildNodeToCheck_16(int nextChildNumber, vec4 &t0, vec4 &t1, vec4 &tm);
	int nextChildNodeToCheck_8(int nextChildNumber, vec4 &t0, vec4 &t1, vec4 &tm);
	int nextChildNodeToCheck_2(int currentNextChildNumber, vec4 &t0, vec4 &t1, vec4 &tm);
	int nextChildNodeToCheck(int currentNextChildNumber, vec4 &t0, vec4 &t1, vec4 &tm);
	
	//first child node to check
	static int firstChildNodeToCheck_16(
		float tx0, float ty0, float tz0, float tt0,
		float txm, float tym, float tzm, float ttm);
	static int firstChildNodeToCheck_8(
		float tx0, float ty0, float tz0, float tt0,
		float txm, float tym, float tzm, float ttm);
	int firstChildNodeToCheck(
		float tx0, float ty0, float tz0, float tt0,
		float txm, float tym, float tzm, float ttm);
	
	vec4 calculateMidpoint(vec4 &t0, vec4 &t1);
	
	// Initialization of the tree traverser
	void inititializeBeforeTraversal();
	void correctRayForNegativeDirectionComponents();
	void safelyCalculateInitialRayParameters(
		float &tx0, float &tx1, float &ty0, float &ty1,
		float &tz0, float &tz1, float &tt0, float &tt1);
	void safelyCalculateRayParametersForDirection(int coord, float &t0, float &t1);

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

inline vec3 Tree4DTraverserDifferentSides::getCurrentPosition() const {
	float t = stack_TraversalInfo_about_Node4Ds.back().t0.max();
	// TODO: better implementation, use a.
	vec4 answer4D = original_ray.getRayPoint(t);
	vec3 answer = vec3(answer4D[0], answer4D[1], answer4D[2]);
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
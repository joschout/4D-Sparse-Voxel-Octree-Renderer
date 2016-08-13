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

	string to_string() const
	{
		stringstream ss;
		ss
			<< "TraversalInfo_About_Node4D" << endl
			<< "   t0: " << t0 << endl
			<< "   t1: " << t1 << endl
			<< "   nextChildToCheck: " << nextChildToCheck << endl
			<< "   min: " << min << endl
			<< "   max: " << max << endl;
		return ss.str();
	}
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
	double getProjectedSizeOfCurrentNode(double t_pixel);
	vec3_d getCurrentPosition() const;
	~Tree4DTraverserDifferentSides(void);


	vec4_d calculateMidpoint(vec4_d &t0, vec4_d &t1);

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

inline double Tree4DTraverserDifferentSides::getProjectedSizeOfCurrentNode(double t_pixel)
{
	// tc_max = min(min(tx1, ty1), tz1)
	//vec4_d& t0 = treeTraverser.stack_TraversalInfo_about_Node4Ds.back().t0;
	vec4_d& t1 = stack_TraversalInfo_about_Node4Ds.back().t1;
	//vec4_d& t1 = treeTraverser.stack_TraversalInfo_about_Node4Ds.back().t1;
	//				vec4_d& t0 = treeTraverser.stack_TraversalInfo_about_Node4Ds.back().t0;
	//				double t_min = max(max(t0[0], t0[1]), t0[2]);
	double t_max = min(min(t1[0], t1[1]), t1[2]);
	vec4_d& min = stack_TraversalInfo_about_Node4Ds.back().min;
	vec4_d& max = stack_TraversalInfo_about_Node4Ds.back().max;

	double t_pixel_corrected = t_pixel;
	if (t_max < 0)
	{
		t_pixel_corrected = -t_pixel;
	}


	double voxelDiameter = abs(max[0] - min[0]);//len(max - min);
	double voxelRadius = 0.5*voxelDiameter;

	double voxelSizeToProject = voxelRadius * voxelRadius; // * PI
	double projectedSize = voxelSizeToProject * t_pixel_corrected / t_max;

	return projectedSize;
}

#endif
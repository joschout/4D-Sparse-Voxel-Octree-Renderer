#include "Tree4DTraverser.h"


using namespace std;

int Tree4DTraverser::firstNode(float tx0, float ty0, float tz0, float txm, float tym, float tzm) {
	unsigned char answer = 0;	// initialize to 00000000
								// select the entry plane and set bits
	if (tx0 > ty0) {
		if (tx0 > tz0) { // PLANE YZ
			if (tym < tx0) answer |= 2;	// set bit at position 1
			if (tzm < tx0) answer |= 1;	// set bit at position 0
			return (int)answer;s
		}
	}
	else {
		if (ty0 > tz0) { // PLANE XZ
			if (txm < ty0) answer |= 4;	// set bit at position 2
			if (tzm < ty0) answer |= 1;	// set bit at position 0
			return (int)answer;
		}
	}
	// PLANE XY
	if (txm < tz0) answer |= 4;	// set bit at position 2
	if (tym < tz0) answer |= 2;	// set bit at position 1
	return (int)answer;
}

// perform PUSH, POP or ADVANCE
void Tree4DTraverser::step() {
	stepcount++;
	// if the stack is empty, we're done
	if (stack.empty()) return;

	// define some aliases to make code readable
	vec4& t0 = stack.back().t0;
	vec4& t1 = stack.back().t1;
	vec4& tm = stack.back().tm;

	//POP
	// if we're in a terminal node or if we visited all children of that node (next child = 16), 
	// or if the t1 values have become negative, pop the current node from the stack
	if (stack.back().nextchild == 16 || stack.back().node->isLeaf()) {
		stack.pop_back();
		return;
	}

	// PUSH
	// we haven't looked at any child in this voxel yet: pick the first and push down
	if (stack.back().nextchild == -1) {
		// calculate midpoint and save it in stack
		tm = 0.5f*(t0 + t1);
		// calculate first node
		stack.back().nextchild = firstNode(t0[0], t0[1], t0[2], tm[0], tm[1], tm[2]);
	}

	// ADVANCE
	// let's look at the next child in this voxel
	switch (stack.back().nextchild)
	{
	case 0: {
		stack.back().nextchild = newNode(tm[0], 4, tm[1], 2, tm[2], 1, tm[3], 8);
		if (stack.back().node->hasChild(a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					t0[0], t0[1], t0[2], t0[3],
					tm[0], tm[1], tm[2], tm[3], tree4D->getNode(stack.back().node->getChildPos(a)));
			stack.push_back(info);
		} return;}
	case 1: {
		stack.back().nextchild = newNode(tm[0], 5, tm[1], 3, t1[2], 16, tm[3], 9);
		if (stack.back().node->hasChild(1 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					t0[0], t0[1], tm[2], t0[3],
					tm[0], tm[1], t1[2], tm[3], tree4D->getNode(stack.back().node->getChildPos(1 ^ a)));
			stack.push_back(info);
		} return;}
	case 2: {
		stack.back().nextchild = newNode(tm[0], 6, t1[1], 16, tm[2], 3, tm[3], 10);
		if (stack.back().node->hasChild(2 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					t0[0], tm[1], t0[2], t0[3],
					tm[0], t1[1], tm[2], tm[3], tree4D->getNode(stack.back().node->getChildPos(2 ^ a)));
			stack.push_back(info);
		} return;}
	case 3: {
		stack.back().nextchild = newNode(tm[0], 7, t1[1], 16, t1[2], 16, tm[3], 11);
		if (stack.back().node->hasChild(3 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					t0[0], tm[1], tm[2], t0[3],
					tm[0], t1[1], t1[2], tm[3], tree4D->getNode(stack.back().node->getChildPos(3 ^ a)));
			stack.push_back(info);
		} return;}
	case 4: {
		stack.back().nextchild = newNode(t1[0], 16, tm[1], 6, tm[2], 5, tm[3], 12);
		if (stack.back().node->hasChild(4 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					tm[0], t0[1], t0[2], t0[3],
					t1[0], tm[1], tm[2], tm[3], tree4D->getNode(stack.back().node->getChildPos(4 ^ a)));
			stack.push_back(info);
		} return;}
	case 5: {
		stack.back().nextchild = newNode(t1[0], 16, tm[1], 7, t1[2], 16, tm[3], 13);
		if (stack.back().node->hasChild(5 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					tm[0], t0[1], tm[2], t0[3],
					t1[0], tm[1], t1[2], tm[3], tree4D->getNode(stack.back().node->getChildPos(5 ^ a)));
			stack.push_back(info);
		} return;}
	case 6: {
		stack.back().nextchild = newNode(t1[0], 16, t1[1], 16, tm[2], 7, tm[3], 14);
		if (stack.back().node->hasChild(6 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					tm[0], tm[1], t0[2], t0[3],
					t1[0], t1[1], tm[2], tm[3], tree4D->getNode(stack.back().node->getChildPos(6 ^ a)));
			stack.push_back(info);
		} return;}
	case 7: {
		stack.back().nextchild = newNode(t1[0], 16, t1[1], 16, t1[2], 16, tm[3], 15);
		if (stack.back().node->hasChild(7 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					tm[0], tm[1], tm[2], t0[3],
					t1[0], t1[1], t1[2], tm[3], tree4D->getNode(stack.back().node->getChildPos(7 ^ a)));
			stack.push_back(info);
		} return;}
	case 8: {
		stack.back().nextchild = newNode(tm[0], 12, tm[1], 10, tm[2], 9, t1[3], 16);
		if (stack.back().node->hasChild(8 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					t0[0], t0[1], t0[2], tm[3],
					tm[0], tm[1], tm[2], t1[3], tree4D->getNode(stack.back().node->getChildPos(7 ^ a)));
			stack.push_back(info);
		} return;}
	case 9: {
		stack.back().nextchild = newNode(tm[0], 13, tm[1], 11, t1[2], 16, t1[3], 16);
		if (stack.back().node->hasChild(9 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					t0[0], t0[1], tm[2], tm[3],
					tm[0], tm[1], t1[2], t1[3], tree4D->getNode(stack.back().node->getChildPos(7 ^ a)));
			stack.push_back(info);
		} return;}
	case 10: {
		stack.back().nextchild = newNode(tm[0], 14, t1[1], 16, tm[2], 11, t1[3], 16);
		if (stack.back().node->hasChild(10 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					t0[0], tm[1], t0[2], tm[3],
					tm[0], t1[1], tm[2], t1[3], tree4D->getNode(stack.back().node->getChildPos(7 ^ a)));
			stack.push_back(info);
		} return;}
	case 11: {
		stack.back().nextchild = newNode(tm[0], 15, t1[1], 16, t1[2], 16, t1[3], 16);
		if (stack.back().node->hasChild(11 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					t0[0], tm[1], tm[2], tm[3],
					tm[0], t1[1], t1[2], t1[3], tree4D->getNode(stack.back().node->getChildPos(7 ^ a)));
			stack.push_back(info);
		} return;}
	case 12: {
		stack.back().nextchild = newNode(t1[0], 16, tm[1], 14, tm[2], 13, t1[3], 16);
		if (stack.back().node->hasChild(12 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					tm[0], t0[1], t0[2], tm[3],
					t1[0], tm[1], tm[2], t1[3], tree4D->getNode(stack.back().node->getChildPos(7 ^ a)));
			stack.push_back(info);
		} return;}
	case 13: {
		stack.back().nextchild = newNode(t1[0], 16, tm[1], 15, t1[2], 16, t1[3], 16);
		if (stack.back().node->hasChild(13 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					tm[0], t0[1], tm[2], tm[3],
					t1[0], tm[1], t1[2], t1[3], tree4D->getNode(stack.back().node->getChildPos(7 ^ a)));
			stack.push_back(info);
		} return;}
	case 14: {
		stack.back().nextchild = newNode(t1[0], 16, t1[1], 16, tm[2], 15, t1[3], 16);
		if (stack.back().node->hasChild(14 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					tm[0], tm[1], t0[2], tm[3],
					t1[0], t1[1], tm[2], t1[3], tree4D->getNode(stack.back().node->getChildPos(7 ^ a)));
			stack.push_back(info);
		} return;}
	case 15: {
		stack.back().nextchild = 16;
		if (stack.back().node->hasChild(15 ^ a)) {
			TraversalNode4DInfo_ info 
				= buildNodeInfo(
					tm[0], tm[1], tm[2], tm[3],
					t1[0], t1[1], t1[2], t1[3], tree4D->getNode(stack.back().node->getChildPos(7 ^ a)));
			stack.push_back(info);
		} return;}

	}
}

void Tree4DTraverser::initTraversal() {
	stepcount = 0;
	a = 0;

	original_ray = ray;

	// fixes for rays with negative direction
	if (ray.direction[0] < 0.0f) {
		ray.origin[0] = tree4D->size[0] - ray.origin[0];
		ray.direction[0] = -ray.direction[0];
		a |= 4; //bitwise OR (latest bits are XYZ)
		// a = a OR 0000 0100
	}
	if (ray.direction[1] < 0.0f) {
		ray.origin[1] = tree4D->size[1] - ray.origin[1];
		ray.direction[1] = -ray.direction[1];
		a |= 2;
		// a = a OR 0000 0010
	}
	if (ray.direction[2] > 0.0f) {
		ray.origin[2] = tree4D->size[2] - ray.origin[2];
		ray.direction[2] = -ray.direction[2];
		a |= 1;
		// a = a OR 0000 0001
	}

	float tx0 = (tree4D->min[0] - ray.origin[0]) * (1.0f / ray.direction[0]);
	float tx1 = (tree4D->max[0] - ray.origin[0]) * (1.0f / ray.direction[0]);
	float ty0 = (tree4D->min[1] - ray.origin[1]) * (1.0f / ray.direction[1]);
	float ty1 = (tree4D->max[1] - ray.origin[1]) * (1.0f / ray.direction[1]);
	float tz0 = (tree4D->min[2] - ray.origin[2]) * (1.0f / ray.direction[2]);
	float tz1 = (tree4D->max[2] - ray.origin[2]) * (1.0f / ray.direction[2]);

	float tt0 = tree4D->min[3];
	float tt1 = tree4D->max[3];

	if (max(max(tx0, ty0), tz0) < min(min(tx1, ty1), tz1)) {
		// push root node on stack
		stack.push_back(buildNodeInfo(tx0, ty0, tz0,tt0, tx1, ty1, tz1, tt1, tree4D->getRootNode()));
		return;
	}
	// push nothing on the stack
}

Tree4DTraverser::~Tree4DTraverser(void)
{
}
#include "Tree4DTraverser.h"


using namespace std;

int Tree4DTraverser::newNode(int nextChildNumber, vec4 &t0, vec4 &t1, vec4 &tm)
{
	switch(nextChildNumber)
	{
	case 0:  return newNode(tm[0],  4, tm[1],  2, tm[2],  1, tm[3],  8);
	case 1:  return newNode(tm[0],  5, tm[1],  3, t1[2], 16, tm[3],  9);
	case 2:  return newNode(tm[0],  6, t1[1], 16, tm[2],  3, tm[3], 10);
	case 3:  return newNode(tm[0],  7, t1[1], 16, t1[2], 16, tm[3], 11);
	case 4:  return newNode(t1[0], 16, tm[1],  6, tm[2],  5, tm[3], 12);
	case 5:  return newNode(t1[0], 16, tm[1],  7, t1[2], 16, tm[3], 13);
	case 6:  return newNode(t1[0], 16, t1[1], 16, tm[2],  7, tm[3], 14);
	case 7:  return newNode(t1[0], 16, t1[1], 16, t1[2], 16, tm[3], 15);
	case 8:  return newNode(tm[0], 12, tm[1], 10, tm[2],  9, t1[3], 16);
	case 9:  return newNode(tm[0], 13, tm[1], 11, t1[2], 16, t1[3], 16);
	case 10: return newNode(tm[0], 14, t1[1], 16, tm[2], 11, t1[3], 16);
	case 11: return newNode(tm[0], 15, t1[1], 16, t1[2], 16, t1[3], 16);
	case 12: return newNode(t1[0], 16, tm[1], 14, tm[2], 13, t1[3], 16);
	case 13: return newNode(t1[0], 16, tm[1], 15, t1[2], 16, t1[3], 16);
	case 14: return newNode(t1[0], 16, t1[1], 16, tm[2], 15, t1[3], 16);
	case 15: return 16;
	}
	return 0;
}

TraversalNode4DInfo_ Tree4DTraverser::buildNodeInfo(int nextChildNumber, vec4& t0, vec4& t1, vec4& tm, const Node4D* node)
{
	switch(nextChildNumber)
	{
	case 0:  return buildNodeInfo(t0[0], t0[1], t0[2], t0[3], tm[0], tm[1], tm[2], tm[3], node);
	case 1:  return buildNodeInfo(t0[0], t0[1], tm[2], t0[3], tm[0], tm[1], t1[2], tm[3], node);
	case 2:  return buildNodeInfo(t0[0], tm[1], t0[2], t0[3], tm[0], t1[1], tm[2], tm[3], node);
	case 3:  return buildNodeInfo(t0[0], tm[1], tm[2], t0[3], tm[0], t1[1], t1[2], tm[3], node);
	case 4:  return buildNodeInfo(tm[0], t0[1], t0[2], t0[3], t1[0], tm[1], tm[2], tm[3], node);
	case 5:  return buildNodeInfo(tm[0], t0[1], tm[2], t0[3], t1[0], tm[1], t1[2], tm[3], node);
	case 6:  return buildNodeInfo(tm[0], tm[1], t0[2], t0[3], t1[0], t1[1], tm[2], tm[3], node);
	case 7:  return buildNodeInfo(tm[0], tm[1], tm[2], t0[3], t1[0], t1[1], t1[2], tm[3], node);
	case 8:  return buildNodeInfo(t0[0], t0[1], t0[2], tm[3], tm[0], tm[1], tm[2], t1[3], node);
	case 9:  return buildNodeInfo(t0[0], t0[1], tm[2], tm[3], tm[0], tm[1], t1[2], t1[3], node);
	case 10: return buildNodeInfo(t0[0], tm[1], t0[2], tm[3], tm[0], t1[1], tm[2], t1[3], node);
	case 11: return buildNodeInfo(t0[0], tm[1], tm[2], tm[3], tm[0], t1[1], t1[2], t1[3], node);
	case 12: return buildNodeInfo(tm[0], t0[1], t0[2], tm[3], t1[0], tm[1], tm[2], t1[3], node);
	case 13: return buildNodeInfo(tm[0], t0[1], tm[2], tm[3], t1[0], tm[1], t1[2], t1[3], node);
	case 14: return buildNodeInfo(tm[0], tm[1], t0[2], tm[3], t1[0], t1[1], tm[2], t1[3], node);
	case 15: return buildNodeInfo(tm[0], tm[1], tm[2], tm[3], t1[0], t1[1], t1[2], t1[3], node);
	}
	return {};
}

int Tree4DTraverser::firstNode(
	float tx0, float ty0, float tz0, float tt0,
	float txm, float tym, float tzm, float ttm)
{
/*	unsigned char answer = 0;	// initialize to 00000000

	//calculate the entry volume of the current voxel
	// => max(tx0, ty0, tz0, tt0)

	// select the entry plane and set bits
	if (tx0 > ty0) {
		if (tx0 > tz0) {
			if (tx0 > tt0){
				// tx0 is maximum
				// VOLUME YZT
				// -> VOXELS 0, 1, 2, 3, 8, 9, 10, 11
				if (tym < tx0) answer |= 2;	// set bit at position 1
				// answer = answer OR 0000 0010

				if (tzm < tx0) answer |= 1;	// set bit at position 0
				// answer = answer OR 000 0001

				if (ttm < tx0) answer |= 8;
				// answer = answer OR 0000 1000

				return (int)answer;
			}
		}
	}
	else { // ty0 > tx0
		if (ty0 > tz0) {
			if(ty0 > tt0){
				// ty0 is maximum
				// VOLUME XZT -> VOXELS  0, 1, 4, 5, 8, 9, 12, 13
				if (txm < ty0) answer |= 4;	// set bit at position 2
				// answer = answer OR 0000 0100

				if (tzm < ty0) answer |= 1;	// set bit at position 0
				// answer = answer OR 0000 0001
				
				if (ttm < ty0) answer |= 8;
				// answer = answer OR 0000 1000

				return (int)answer;
			}
		}
		else {//tz0 > ty0 > tx0
			if (tz0 > tt0) {
				// tz0 is maximum
				// VOLUME XYT -> VOXELS 0, 2, 4, 6, 8, 10, 12, 14
				if (txm < tz0) answer |= 4;	// set bit at position 2
				// answer = answer OR 0000 0100

				if (tym < tz0) answer |= 2;	// set bit at position 1
				// answer = answer OR 0000 0010

				if (ttm < tz0) answer |= 8;
				//answer = answer OR 0000 1000

				return (int)answer;
			}
		}
	}

	// tt0 is maximum
	//VOLUME XYZ -> VOXELS 0, 1, 2, 3, 4, 5, 6, 7
	if (txm < tt0) answer |= 4;	// set bit at position 2
	// answer = answer OR 0000 0100
	if (tym < tt0) answer |= 2;	// set bit at position 1
	// answer = answer OR 0000 0010
	if (tzm < tt0) answer |= 1;

	return (int)answer;*/

	unsigned char answer = 0;	// initialize to 00000000

								//calculate the entry face of the current voxel
								// => max(tx0, ty0, tz0)

								// select the entry plane and set bits
	if (tx0 > ty0) {
		if (tx0 > tz0) {
			// tx0 is maximum
			// PLANE YZ -> VOXELS 0,1,2,3
			if (tym < tx0) answer |= 2;	// set bit at position 1
										// answer = answer OR 0000 0010
			if (tzm < tx0) answer |= 1;	// set bit at position 0
										// answer = answer OR 000 0001
			return (int)answer;
		}
	}
	else {
		if (ty0 > tz0) {
			// ty0 is maximum
			// PLANE XZ -> VOXELS  0, 1, 4, 5
			if (txm < ty0) answer |= 4;	// set bit at position 2
										// answer = answer OR 0000 1000
			if (tzm < ty0) answer |= 1;	// set bit at position 0
										// answer = answer OR 0000 0001
			return (int)answer;
		}
	}//tz0 is maximum
	 // PLANE XY -> VOXELS 0, 2, 4, 6
	if (txm < tz0) answer |= 4;	// set bit at position 2
								// answer = answer OR 0000 0100
	if (tym < tz0) answer |= 2;	// set bit at position 1
								// answer = answer OR 0000 0010

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
		stack.back().nextchild 
			= firstNode(
				t0[0], t0[1], t0[2], t0[3],
				tm[0], tm[1], tm[2], tm[3]);
	}

	// ADVANCE
	// let's look at the next child in this voxel
	int nextChildNumber = stack.back().nextchild;

	if (nextChildNumber > 7 && nextChildNumber != 16)
	{
		cout << nextChildNumber << endl;
	}



	const Node4D* node = tree4D->getNode(stack.back().node->getChildPos(nextChildNumber ^ a));
	stack.back().nextchild = newNode(nextChildNumber, t0, t1, tm);
	if (stack.back().node->hasChild(nextChildNumber ^ a)) {
		TraversalNode4DInfo_ info
			= buildNodeInfo(nextChildNumber, t0, t1, tm, node);
		stack.push_back(info);
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
	if(ray.direction[3] < 0.0f)
	{
		ray.origin [3] = tree4D->size[3] - ray.origin[3];
		ray.direction[3] = -ray.direction[3];
		a |= 8;
		// a = a OR 0000 1000
	}

	float tx0 = (tree4D->min[0] - ray.origin[0]) * (1.0f / ray.direction[0]);
	float tx1 = (tree4D->max[0] - ray.origin[0]) * (1.0f / ray.direction[0]);
	float ty0 = (tree4D->min[1] - ray.origin[1]) * (1.0f / ray.direction[1]);
	float ty1 = (tree4D->max[1] - ray.origin[1]) * (1.0f / ray.direction[1]);
	float tz0 = (tree4D->min[2] - ray.origin[2]) * (1.0f / ray.direction[2]);
	float tz1 = (tree4D->max[2] - ray.origin[2]) * (1.0f / ray.direction[2]);
/*	cout << "tree4D min Z: " << tree4D->min[2] << endl;
	cout << "tree4D max Z: " << tree4D->max[2] << endl;
	cout << "ray origin Z: " << ray.origin[2] << endl;
	cout << "ray direction Z: " << ray.direction[2] << endl;

	cout << "tz0 = (" << tree4D->min[2] << " - " << ray.origin[2] << ")*" << 1.0f / ray.direction[2] << endl;
	cout << "tz0 = " << tz0 << endl;
	cout << "tz1 = (" << tree4D->max[2] << " - " << ray.origin[2] << ")#1#" << 1.0f / ray.direction[2] << endl;
	cout << "tz1 = " << tz1 << endl;
	cout << endl;*/


	float tt0 = 0; //(tree4D->min[3] - ray.origin[3]) * (1.0f / ray.direction[3]);
	float tt1 = 2;// (tree4D->max[3] - ray.origin[3]) * (1.0f / ray.direction[3]);
	
//	cout << "tx0: " << tx0 << ", ty0: " << ty0 << ", tz0: " << tz0 << ", tt0: " << tt0 << endl;
//	cout << "tx1: " << tx1 << ", ty1: " << ty1 << ", tz1: " << tz1 << ", tt1: " << tt1 << endl;

	bool condition3D = max(max(tx0, ty0), tz0) < min(min(tx1, ty1), tz1);
	bool condition4D = max(max(max(tx0, ty0), tz0), tt0) < min(min(min(tx1, ty1), tz1), tt1);
	if (condition3D) {
		// push root node on stack
		const Node4D* root = tree4D->getRootNode();
		TraversalNode4DInfo_ info = buildNodeInfo(tx0, ty0, tz0, tt0, tx1, ty1, tz1, tt1, root);
		stack.push_back(info);
		return;
	}
	// push nothing on the stack
}

Tree4DTraverser::~Tree4DTraverser(void)
{
}


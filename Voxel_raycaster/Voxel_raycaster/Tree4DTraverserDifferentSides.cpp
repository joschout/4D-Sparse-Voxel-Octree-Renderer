#include "Tree4DTraverserDifferentSides.h"
#include <cassert>

//#define use3D

//#define showDebug
using namespace std;

int Tree4DTraverserDifferentSides::nextChildNodeToCheck_16(
	float txm, int x, float tym, int y, float tzm, int z,
	float ttm, int t) {
#ifndef use3D

#ifdef showDebug
	if (t == 8)
	{
		cout << "t = 8, opgepast!" << endl;
	}
	if (t == 8 && (txm == tym || txm == tym || txm == ttm
		|| tym == tzm || txm == ttm
		|| tzm == ttm))
	{
		cout << "EQUALITY DETECTED, PANIC NOW" << endl;
		cout << "txm: " << txm << ", tym: " << tym << ", tzm: " << tzm << ", ttm: " << ttm << endl;
	}
#endif

	/*
	if(txm <= tym)
	{
	if(txm < tzm)
	{
	if (txm < ttm)
	{
	return x;
	}
	}
	}else
	{
	return;
	}
	*/




	if (txm < tym) {
#ifdef showDebug
		cout << "txm: " << txm << ", tym: " << tym << ", tzm: " << tzm << ", ttm: " << ttm << endl;
#endif
		if (txm < tzm)
		{
			if (txm < ttm)
			{
#ifdef showDebug
				cout << "txm is minimum" << endl;
#endif
				// txm is minimum
				return x; // YZT volume
			}
		}
	}
	// tym <= txm
	// ==> tym OR tzm OR ttm is min
	if (tym < tzm)
	{
		if (tym < ttm) {
#ifdef showDebug
			cout << "tym is minimum" << endl;
#endif
			// tym is minimum
			return y;
			// XZT volume
		}
	}
	// tzm <= tym <= txm
	//==> tzm or ttm is min
	if (tzm < ttm)
	{
#ifdef showDebug
		cout << "tzm is minimum" << endl;
#endif
		// tzm is minimum
		return z; // XYT volume;
	}

#ifdef showDebug
	cout << "ttm is minimum" << endl;
#endif
	//ttm is minimum
	return t; // XYZ volume;*/





#else
	if (txm < tym) {
		if (txm < tzm)
		{ // txm minimal
			return x;
		} // YZ plane
	}
	else {
		if (tym < tzm)
		{ // tym minimal
			return y;
		} // XZ plane
	}
	return z; // XY plane;
#endif
}

int Tree4DTraverserDifferentSides::nextChildNodeToCheck_8(float txm, int x, float tym, int y, float tzm, int z)
{
	if (txm < tym) {
		if (txm < tzm)
		{ // txm minimal
			return x;
		} // YZ plane
	}

	// tym <= txm
	// ==> tym OR tzm is min
	if (tym < tzm)
	{ // tym minimal
		return y;
	} // XZ plane
	
	// tzm <= tym <= txm
	return z; // XY plane;
}

/*
Returns the number of the child node we have to check 
after we have checked the current child node to check.
*/
int Tree4DTraverserDifferentSides::nextChildNodeToCheck_16(int currentNextChildNumber, vec4 &t0, vec4 &t1, vec4 &tm)
{
	assert(currentNextChildNumber < 17);
	switch (currentNextChildNumber)
	{
	case 0:  return nextChildNodeToCheck_16(tm[0], 4, tm[1], 2, tm[2], 1, tm[3], 8);
	case 1:  return nextChildNodeToCheck_16(tm[0], 5, tm[1], 3, t1[2], 16, tm[3], 9);
	case 2:  return nextChildNodeToCheck_16(tm[0], 6, t1[1], 16, tm[2], 3, tm[3], 10);
	case 3:  return nextChildNodeToCheck_16(tm[0], 7, t1[1], 16, t1[2], 16, tm[3], 11);
	case 4:  return nextChildNodeToCheck_16(t1[0], 16, tm[1], 6, tm[2], 5, tm[3], 12);
	case 5:  return nextChildNodeToCheck_16(t1[0], 16, tm[1], 7, t1[2], 16, tm[3], 13);
	case 6:  return nextChildNodeToCheck_16(t1[0], 16, t1[1], 16, tm[2], 7, tm[3], 14);
	case 7:  return nextChildNodeToCheck_16(t1[0], 16, t1[1], 16, t1[2], 16, tm[3], 15);
	case 8:  return nextChildNodeToCheck_16(tm[0], 12, tm[1], 10, tm[2], 9, t1[3], 16);
	case 9:  return nextChildNodeToCheck_16(tm[0], 13, tm[1], 11, t1[2], 16, t1[3], 16);
	case 10: return nextChildNodeToCheck_16(tm[0], 14, t1[1], 16, tm[2], 11, t1[3], 16);
	case 11: return nextChildNodeToCheck_16(tm[0], 15, t1[1], 16, t1[2], 16, t1[3], 16);
	case 12: return nextChildNodeToCheck_16(t1[0], 16, tm[1], 14, tm[2], 13, t1[3], 16);
	case 13: return nextChildNodeToCheck_16(t1[0], 16, tm[1], 15, t1[2], 16, t1[3], 16);
	case 14: return nextChildNodeToCheck_16(t1[0], 16, t1[1], 16, tm[2], 15, t1[3], 16);
	case 15: return 16;
	}
	return 0;
}

int Tree4DTraverserDifferentSides::nextChildNodeToCheck_8(int currentNextChildNumber, vec4 &t0, vec4 &t1, vec4 &tm)
{
	
	/* The case of an octary node
		0 -> 0 -> 0,1
		1 -> 2 -> 2,3
		2 -> 4 -> 4,5
		3 -> 6 -> 6,7
		4 -> 8 -> 8,9
		5 -> 10 -> 10,11
		6 -> 12 -> 12,13
		7 -> 14 -> 14,15
	*/
	assert(currentNextChildNumber < 17);
	switch (currentNextChildNumber)
	{
	case 0:  return nextChildNodeToCheck_8(tm[0], 8, tm[1], 4, tm[2], 2);
	case 2:  return nextChildNodeToCheck_8(tm[0], 10, tm[1], 6, t1[2], 16);
	case 4:  return nextChildNodeToCheck_8(tm[0], 12, t1[1], 16, tm[2], 6);
	case 6:  return nextChildNodeToCheck_8(tm[0], 14, t1[1], 16, t1[2], 16);
	case 8:  return nextChildNodeToCheck_8(t1[0], 16, tm[1], 12, tm[2], 10);
	case 10:  return nextChildNodeToCheck_8(t1[0], 16, tm[1], 14, t1[2], 16);
	case 12:  return nextChildNodeToCheck_8(t1[0], 16, t1[1], 16, tm[2], 14);
	case 14:  return 16;
	}
	return 0;
}


int Tree4DTraverserDifferentSides::nextChildNodeToCheck_2(int currentNextChildNumber, vec4 &t0, vec4 &t1, vec4 &tm)
{
	assert(currentNextChildNumber < 17);
	switch (currentNextChildNumber)
	{
	case 0: return 8;
	case 8: return 16;
	}
	return 0;
}



int Tree4DTraverserDifferentSides::nextChildNodeToCheck(int currentNextChildNumber, vec4 &t0, vec4 &t1, vec4 &tm)
{
	if(stack_TraversalInfo_about_Node4Ds.back().isBinary) //binary node
	{
		return nextChildNodeToCheck_2(currentNextChildNumber, t0, t1, tm);
	}else //one of 16 child nodes
	{
		return nextChildNodeToCheck_16(currentNextChildNumber, t0, t1, tm);
	}

}


TraversalInfo_About_Node4D Tree4DTraverserDifferentSides::buildNodeInfo_16(int nextChildNumber, vec4& t0, vec4& t1, vec4& tm, vec4 min, vec4 max, const Node4D* node)
{
	float x_min = min[0];
	float y_min = min[1];
	float z_min = min[2];
	float t_min = min[3];

	float x_max = max[0];
	float y_max = max[1];
	float z_max = max[2];
	float t_max = max[3];

	float x_mid = (max[0] + min[0]) / 2;
	float y_mid = (max[1] + min[1]) / 2;
	float z_mid = (max[2] + min[2]) / 2;
	float t_mid = (max[3] + min[3]) / 2;

/*	if(x_mid < 0 || y_mid < 0 || z_mid > 0 || t_mid < 0) //Remember: for Z, it is the opposite
	{
		std::cout << "x_mid: " << x_mid << ", y_mid: " << y_mid << ", z_mid: " << z_mid << ", t_mid: " << t_mid << endl;
	}*/

	switch (nextChildNumber)
	{
	case 0:  return buildNodeInfo(t0[0], t0[1], t0[2], t0[3], tm[0], tm[1], tm[2], tm[3],
		vec4(x_min, y_min, z_min, t_min), vec4(x_mid, y_mid, z_mid, t_mid), node, false);
	case 1:  return buildNodeInfo(t0[0], t0[1], tm[2], t0[3], tm[0], tm[1], t1[2], tm[3],
		vec4(x_min, y_min, z_mid, t_min), vec4(x_mid, y_mid, z_max, t_mid), node, false);
	case 2:  return buildNodeInfo(t0[0], tm[1], t0[2], t0[3], tm[0], t1[1], tm[2], tm[3],
		vec4(x_min, y_mid, z_min, t_min), vec4(x_mid, y_max, z_mid, t_mid), node, false);
	case 3:  return buildNodeInfo(t0[0], tm[1], tm[2], t0[3], tm[0], t1[1], t1[2], tm[3],
		vec4(x_min, y_mid, z_mid, t_min), vec4(x_mid, y_max, z_max, t_mid), node, false);
	case 4:  return buildNodeInfo(tm[0], t0[1], t0[2], t0[3], t1[0], tm[1], tm[2], tm[3],
		vec4(x_mid, y_min, z_min, t_min), vec4(x_max, y_mid, z_mid, t_mid), node, false);
	case 5:  return buildNodeInfo(tm[0], t0[1], tm[2], t0[3], t1[0], tm[1], t1[2], tm[3],
		vec4(x_mid, y_min, z_mid, t_min), vec4(x_max, y_mid, z_max, t_mid), node, false);
	case 6:  return buildNodeInfo(tm[0], tm[1], t0[2], t0[3], t1[0], t1[1], tm[2], tm[3],
		vec4(x_mid, y_mid, z_min, t_min), vec4(x_max, y_max, z_mid, t_mid), node, false);
	case 7:  return buildNodeInfo(tm[0], tm[1], tm[2], t0[3], t1[0], t1[1], t1[2], tm[3],
		vec4(x_mid, y_mid, z_mid, t_min), vec4(x_max, y_max, z_max, t_mid), node, false);
	case 8:  return buildNodeInfo(t0[0], t0[1], t0[2], tm[3], tm[0], tm[1], tm[2], t1[3],
		vec4(x_min, y_min, z_min, t_mid), vec4(x_mid, y_mid, z_mid, t_max), node, false);
	case 9:  return buildNodeInfo(t0[0], t0[1], tm[2], tm[3], tm[0], tm[1], t1[2], t1[3],
		vec4(x_min, y_min, z_mid, t_mid), vec4(x_mid, y_mid, z_max, t_max), node, false);
	case 10: return buildNodeInfo(t0[0], tm[1], t0[2], tm[3], tm[0], t1[1], tm[2], t1[3],
		vec4(x_min, y_mid, z_min, t_mid), vec4(x_mid, y_max, z_mid, t_max), node, false);
	case 11: return buildNodeInfo(t0[0], tm[1], tm[2], tm[3], tm[0], t1[1], t1[2], t1[3],
		vec4(x_min, y_mid, z_mid, t_mid), vec4(x_mid, y_max, z_max, t_max), node, false);
	case 12: return buildNodeInfo(tm[0], t0[1], t0[2], tm[3], t1[0], tm[1], tm[2], t1[3],
		vec4(x_mid, y_min, z_min, t_mid), vec4(x_max, y_mid, z_mid, t_max), node, false);
	case 13: return buildNodeInfo(tm[0], t0[1], tm[2], tm[3], t1[0], tm[1], t1[2], t1[3],
		vec4(x_mid, y_min, z_mid, t_mid), vec4(x_max, y_mid, z_max, t_max), node, false);
	case 14: return buildNodeInfo(tm[0], tm[1], t0[2], tm[3], t1[0], t1[1], tm[2], t1[3],
		vec4(x_mid, y_mid, z_min, t_mid), vec4(x_max, y_max, z_mid, t_max), node, false);
	case 15: return buildNodeInfo(tm[0], tm[1], tm[2], tm[3], t1[0], t1[1], t1[2], t1[3],
		vec4(x_mid, y_mid, z_mid, t_mid), vec4(x_max, y_max, z_max, t_max), node, false);
	}
	return{};
}

TraversalInfo_About_Node4D Tree4DTraverserDifferentSides::buildNodeInfo_2_16(int nextChildNumber, vec4& t0, vec4& t1, vec4& tm, vec4 min, vec4 max, const Node4D* node, bool isParentBinary)
{

	if(isParentBinary)
	{
		float x_min = min[0];
		float y_min = min[1];
		float z_min = min[2];
		float t_min = min[3];

//		float x_mid = (max[0] - min[0]) / 2;
//		float y_mid = (max[1] - min[1]) / 2;
//		float z_mid = (max[2] - min[2]) / 2;
		float t_mid = (max[3] - min[3]) / 2;

		float x_max = max[0];
		float y_max = max[1];
		float z_max = max[2];
		float t_max = max[3];

		bool isChildNodeBinary = node->isBinaryNode();

		switch(nextChildNumber)
		{
		case 0: return buildNodeInfo(t0[0], t0[1], t0[2], t0[3], t1[0], t1[1], t1[2], tm[3],
			vec4(x_min, y_min, z_min, t_min), vec4(x_max, y_max, z_max, t_mid), node, isChildNodeBinary);
		case 8: return buildNodeInfo(t0[0], t0[1], t0[2], tm[3], t1[0], t1[1], t1[2], t1[3],
			vec4(x_min, y_min, z_min, t_mid), vec4(x_max, y_max, z_max, t_max), node, isChildNodeBinary);
		}
		return{};
	}else
	{
		return buildNodeInfo_16(nextChildNumber, t0, t1, tm, min, max, node);
	}
}

TraversalInfo_About_Node4D Tree4DTraverserDifferentSides::buildNodeInfo(
	float tx0, float ty0, float tz0, float tt0,
	float tx1, float ty1, float tz1, float tt1,
	vec4 min, vec4 max,
	const Node4D* node, bool isBinary)
{
	TraversalInfo_About_Node4D info;
	info.node = node;
	info.t0 = vec4(tx0, ty0, tz0, tt0);
	info.t1 = vec4(tx1, ty1, tz1, tt1);
	info.nextChildToCheck = -1;
	info.min = min;
	info.max = max;
	info.isBinary = isBinary;
	return info;
}


int Tree4DTraverserDifferentSides::firstChildNodeToCheck(float tx0, float ty0, float tz0, float tt0,
	float txm, float tym, float tzm, float ttm)
{
	/*
	Herinner:
		
		1D -> 1 possible entry point (1 possible entry child node)
		2D -> 2 possible entry lines (3 possible entry child nodes)
		3D -> 3 possible entry planes (7 possible entry child nodes)
		4D -> 4 possible entry hyperplanes (15 possible entry child nodes)
	*/
	if(stack_TraversalInfo_about_Node4Ds.back().isBinary)
	{
		return 0;
	}else
	{
		return firstChildNodeToCheck_16( tx0,  ty0,  tz0,  tt0,	 txm,  tym,  tzm,  ttm);
	}


}


int Tree4DTraverserDifferentSides::firstChildNodeToCheck_16(
	float tx0, float ty0, float tz0, float tt0,
	float txm, float tym, float tzm, float ttm)
{
#ifndef use3D
	unsigned char answer = 0;	// initialize to 00000000
								/*
								cout << "deciding first node:" << endl;
								cout << "tx0: " << tx0 << ", ty0: " << ty0 << ", tz0: " << tz0 << ", tt0: " << tt0 << endl;
								cout << "txm: " << txm << ", tym: " << tym << ", tzm: " << tzm << ", ttm: " << ttm << endl;
								*/
								//calculate the entry volume of the current voxel
								// => max(tx0, ty0, tz0, tt0)

								// select the entry plane and set bits
	if (tx0 > ty0) {
		if (tx0 > tz0) {
			if (tx0 > tt0) {
				// tx0 is maximum
				// VOLUME YZT
				// -> VOXELS 0, 1, 2, 3, 8, 9, 10, 11
				if (tym < tx0) answer |= 2;	// set bit at position 1
											// answer = answer OR 0000 0010

				if (tzm < tx0) answer |= 1;	// set bit at position 0
											// answer = answer OR 000 0001

				if (ttm < tx0) answer |= 8;
				// answer = answer OR 0000 1000
#ifdef showDebug
				cout << "tx0 is max" << endl;
				cout << "first node is " << to_string((int)answer) << endl << endl;
#endif showDebug
				return (int)answer;
			}
		}
	}
	// ty0 > tx0
	if (ty0 > tz0) {
		if (ty0 > tt0) {
			// ty0 is maximum
			// VOLUME XZT -> VOXELS  0, 1, 4, 5, 8, 9, 12, 13
			if (txm < ty0) answer |= 4;	// set bit at position 2
										// answer = answer OR 0000 0100

			if (tzm < ty0) answer |= 1;	// set bit at position 0
										// answer = answer OR 0000 0001

			if (ttm < ty0) answer |= 8;
			// answer = answer OR 0000 1000
#ifdef showDebug
			cout << "ty0 is max" << endl;
			cout << "first node is " << to_string((int)answer) << endl << endl;
#endif
			return (int)answer;
		}
	}
	//tz0 > ty0 > tx0
	if (tz0 > tt0) {
		// tz0 is maximum
		// VOLUME XYT -> VOXELS 0, 2, 4, 6, 8, 10, 12, 14
		if (txm < tz0) answer |= 4;	// set bit at position 2
									// answer = answer OR 0000 0100

		if (tym < tz0) answer |= 2;	// set bit at position 1
									// answer = answer OR 0000 0010

		if (ttm < tz0) answer |= 8;
		//answer = answer OR 0000 1000
#ifdef showDebug
		cout << "tz0 is max" << endl;
		cout << "first node is " << to_string((int)answer) << endl << endl;
#endif	
		return (int)answer;
	}


#ifdef showDebug
	cout << "tt0 is max" << endl;
	cout << "first node is " << to_string((int)answer) << endl << endl;
#endif
	// tt0 is maximum
	//VOLUME XYZ -> VOXELS 0, 1, 2, 3, 4, 5, 6, 7
	if (txm < tt0) answer |= 4;	// set bit at position 2
								// answer = answer OR 0000 0100
	if (tym < tt0) answer |= 2;	// set bit at position 1
								// answer = answer OR 0000 0010
	if (tzm < tt0) answer |= 1;

	return (int)answer;
#else
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
#endif
}

int Tree4DTraverserDifferentSides::firstChildNodeToCheck_8(float tx0, float ty0, float tz0, float tt0, float txm, float tym, float tzm, float ttm)
{
	unsigned char answer = 0;	// initialize to 00000000

								//calculate the entry face of the current voxel
								// => max(tx0, ty0, tz0)

								// select the entry plane and set bits
	if (tx0 > ty0) {
		if (tx0 > tz0) {
			// tx0 is maximum
			// PLANE YZ -> VOXELS 0, 1, 2, 3 (in 3D) -> VOXELS 0, 2, 4, 6 (in 4D)
			if (tym < tx0) answer |= 2;	// set bit at position 1
										// answer = answer OR 0000 0010
			if (tzm < tx0) answer |= 1;	// set bit at position 0
										// answer = answer OR 000 0001
			return static_cast<int>(answer << 1); // times 2 (=2^1)
		}
	}
	else {
		if (ty0 > tz0) {
			// ty0 is maximum
			// PLANE XZ -> VOXELS  0, 1, 4, 5 (in 3D) -> VOXELS 0, 2, 8, 10 (in 4D)
			if (txm < ty0) answer |= 4;	// set bit at position 2
										// answer = answer OR 0000 1000
			if (tzm < ty0) answer |= 1;	// set bit at position 0
										// answer = answer OR 0000 0001
			return static_cast<int>(answer << 1); // times 2 (=2^1)
		}
	}//tz0 is maximum
	 // PLANE XY -> VOXELS 0, 2, 4, 6 (in 3D) -> VOXELS 0, 4, 8, 12 (in 4D)
	if (txm < tz0) answer |= 4;	// set bit at position 2
								// answer = answer OR 0000 0100
	if (tym < tz0) answer |= 2;	// set bit at position 1
								// answer = answer OR 0000 0010

	return static_cast<int>(answer << 1); // times 2 (=2^1)
}

void Tree4DTraverserDifferentSides::correctRayForNegativeDirectionComponents()
{
	if (ray.direction[0] < 0.0f) {
		/*
		IF the ray direction is negative in the X coordinate
		THEN reflect the ray with respect to the middle plane of the 4D-tree
		
		*/

//		ray.origin[0] = tree4D->size[0] - ray.origin[0];
		ray.origin[0] = abs(tree4D->min[0] - tree4D->max[0]) - ray.origin[0];
		ray.direction[0] = -ray.direction[0];
		a |= 4; //bitwise OR (latest bits are XYZ)
				// a = a OR 0000 0100
	}
	if (ray.direction[1] < 0.0f) {
//		ray.origin[1] = tree4D->size[1] - ray.origin[1];
		ray.origin[1] = abs(tree4D->min[1] - tree4D->max[1]) - ray.origin[1];
		ray.direction[1] = -ray.direction[1];
		a |= 2;
		// a = a OR 0000 0010
	}
	if (ray.direction[2] > 0.0f) {
//		ray.origin[2] = tree4D->size[2] - ray.origin[2];
		ray.origin[2] = abs(tree4D->min[2] - tree4D->max[2]) - ray.origin[2];
		ray.direction[2] = -ray.direction[2];
		a |= 1;
		// a = a OR 0000 0001
	}
	if (ray.direction[3] < 0.0f){
//		ray.origin[3] = tree4D->size[3] - ray.origin[3];
		ray.origin[3] = abs(tree4D->min[3] - tree4D->max[3]) - ray.origin[3];
		ray.direction[3] = -ray.direction[3];
		a |= 8;
		// a = a OR 0000 1000
	}
}

void Tree4DTraverserDifferentSides::safelyCalculateInitialRayParameters(float& tx0, float& tx1, float& ty0, float& ty1, float& tz0, float& tz1, float& tt0, float& tt1)
{
	safelyCalculateRayParametersForDirection(0, tx0, tx1);
	safelyCalculateRayParametersForDirection(1, ty0, ty1);
	safelyCalculateRayParametersForDirection(2, tz0, tz1);
	safelyCalculateRayParametersForDirection(3, tt0, tt1);

	/*	float tx0 = (tree4D->min[0] - ray.origin[0]) * (1.0f / ray.direction[0]);
	float tx1 = (tree4D->max[0] - ray.origin[0]) * (1.0f / ray.direction[0]);
	float ty0 = (tree4D->min[1] - ray.origin[1]) * (1.0f / ray.direction[1]);
	float ty1 = (tree4D->max[1] - ray.origin[1]) * (1.0f / ray.direction[1]);
	float tz0 = (tree4D->min[2] - ray.origin[2]) * (1.0f / ray.direction[2]);
	float tz1 = (tree4D->max[2] - ray.origin[2]) * (1.0f / ray.direction[2]);
	float tt0 = (tree4D->min[3] - ray.origin[3]) * (1.0f / ray.direction[3]);
	float tt1 = (tree4D->max[3] - ray.origin[3]) * (1.0f / ray.direction[3]);*/
	/*	cout << "tree4D min Z: " << tree4D->min[2] << endl;
	cout << "tree4D max Z: " << tree4D->max[2] << endl;
	cout << "ray origin Z: " << ray.origin[2] << endl;
	cout << "ray direction Z: " << ray.direction[2] << endl;

	cout << "tz0 = (" << tree4D->min[2] << " - " << ray.origin[2] << ")*" << 1.0f / ray.direction[2] << endl;
	cout << "tz0 = " << tz0 << endl;
	cout << "tz1 = (" << tree4D->max[2] << " - " << ray.origin[2] << ")#1#" << 1.0f / ray.direction[2] << endl;
	cout << "tz1 = " << tz1 << endl;
	cout << endl;*/
	//	cout << "tx0: " << tx0 << ", ty0: " << ty0 << ", tz0: " << tz0 << ", tt0: " << tt0 << endl;
	//	cout << "tx1: " << tx1 << ", ty1: " << ty1 << ", tz1: " << tz1 << ", tt1: " << tt1 << endl;

}

// perform PUSH, POP or ADVANCE
void Tree4DTraverserDifferentSides::step() {
	stepcount++;
	// if the stack is empty, we're done
	if (stack_TraversalInfo_about_Node4Ds.empty())
	{
		return;
	}

	// define some aliases to make code readable
	vec4& t0 = stack_TraversalInfo_about_Node4Ds.back().t0;
	vec4& t1 = stack_TraversalInfo_about_Node4Ds.back().t1;
	vec4& tm = stack_TraversalInfo_about_Node4Ds.back().tm;

	//POP
	// if we're in a terminal node or if we visited all children of that node (next child = 16), 
	// or if the t1 values have become negative, pop the current node from the stack
	if (stack_TraversalInfo_about_Node4Ds.back().nextChildToCheck == 16 || stack_TraversalInfo_about_Node4Ds.back().node->isLeaf()) {

#ifdef showDebug
		cout << "POP" << endl;
		if (stack.back().nextchild == 16)
		{
			cout << "reason: nextchild == 16" << endl;
		}
		if (stack.back().node->isLeaf())
		{
			cout << "reason: node is Leaf" << endl;
		}
		cout << endl;
#endif		
		stack_TraversalInfo_about_Node4Ds.pop_back();
		return;
	}

	// PUSH
	// we haven't looked at any child in this voxel yet: pick the first and push down
	if (stack_TraversalInfo_about_Node4Ds.back().nextChildToCheck == -1) {
		// calculate midpoint and save it in stack
#ifdef showDebug
		cout << "PUSH" << endl;
#endif

		//tm = 0.5f*(t0 + t1);
		tm = calculateMidpoint(t0, t1);
		// calculate first node
		stack_TraversalInfo_about_Node4Ds.back().nextChildToCheck
			= firstChildNodeToCheck(
				t0[0], t0[1], t0[2], t0[3],
				tm[0], tm[1], tm[2], tm[3]);
	}


	// ADVANCE
	// let's look at the next child in this voxel
	int nextChildNumber = getCurrentNodeInfo().nextChildToCheck;
	//number of the next child we will look into
#ifdef showDebug
	cout << "ADVANCE" << endl;
	cout << "next child: " << nextChildNumber << endl << endl;
#endif
	//assert(nextChildNumber < 7);
#ifdef showDebug
	if (nextChildNumber >8 && nextChildNumber != 16)
	{
		cout << nextChildNumber << endl;
	}
#endif

	/*
	set the next child to check of the current node
	to the childnode we need to check after we have checked the current child node to check
	*/
	stack_TraversalInfo_about_Node4Ds.back().nextChildToCheck = nextChildNodeToCheck(nextChildNumber, t0, t1, tm);
	if (getCurrentNode()->hasChild(nextChildNumber ^ a)) {

		const Node4D* nextChildNode
			= tree4D->getNode(
				getCurrentNode()->getChildPos(nextChildNumber ^ a));

		bool isCurrentNodeBinary = getCurrentNodeInfo().isBinary;

		TraversalInfo_About_Node4D info_about_nextChildNode
			= buildNodeInfo_2_16(nextChildNumber, t0, t1, tm, getCurrentNodeInfo().min, getCurrentNodeInfo().max, nextChildNode, isCurrentNodeBinary);
		stack_TraversalInfo_about_Node4Ds.push_back(info_about_nextChildNode);
	}
}

void Tree4DTraverserDifferentSides::inititializeBeforeTraversal() {
	stepcount = 0;
	a = 0;

	original_ray = ray;

	// fixes for rays with negative direction
	correctRayForNegativeDirectionComponents();
	
	//This line should be all, what you need to add to your code.
	static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 required");
	
	//Safe calculation of the ray parameters (check division by zero, use of inifinty)
	float tx0, tx1, ty0, ty1, tz0, tz1, tt0, tt1;
	safelyCalculateInitialRayParameters(tx0, tx1, ty0, ty1, tz0, tz1, tt0, tt1);
	
	//bool condition3D = max(max(tx0, ty0), tz0) < min(min(tx1, ty1), tz1);
	bool condition4D = max(max(max(tx0, ty0), tz0), tt0) < min(min(min(tx1, ty1), tz1), tt1);
	if (condition4D) {
		// push root node on stack
		const Node4D* root = tree4D->getRootNode();

		bool isRootBinary = root->isBinaryNode();

		TraversalInfo_About_Node4D info_about_root 
			= buildNodeInfo(tx0, ty0, tz0, tt0, tx1, ty1, tz1, tt1, tree4D->min, tree4D->max, root, isRootBinary);
		stack_TraversalInfo_about_Node4Ds.push_back(info_about_root);
	}
	// ELSE push nothing on the stack
}

void Tree4DTraverserDifferentSides::safelyCalculateRayParametersForDirection(int coord, float& t0, float& t1)
{
	float denominator = ray.direction[coord];
	float numerator_0 = tree4D->min[coord] - ray.origin[coord];
	float numerator_1 = tree4D->max[coord] - ray.origin[coord];

	if (denominator == 0.0f) {
		//OPGEPAST

		//numerator_0;
		if (numerator_0 > 0)
		{
			t0 = std::numeric_limits<float>::infinity();
		}
		else
		{
			if (numerator_0 < 0)
			{
				t0 = -1 * std::numeric_limits<float>::infinity();
			}
			else // numerator_0 == 0.0f
				 // tree4D->min[coord] = ray.origin[coord];
			{
				t0 = -1 * std::numeric_limits<float>::infinity();
				/*			if(numerator_1 > 0)
				{
				t0 = -1 * std::numeric_limits<float>::infinity();
				}else
				{
				t0 = std::numeric_limits<float>::infinity();
				}*/
			}
		}

		// numerator_1
		if (numerator_1 > 0)
		{
			t1 = std::numeric_limits<float>::infinity();
		}
		else
		{
			if (numerator_1 < 0)
			{
				t1 = -1 * std::numeric_limits<float>::infinity();
			}
			else // numerator_0 == 0.0f
				 // tree4D->min[coord] = ray.origin[coord];
			{
				t1 = -1 * std::numeric_limits<float>::infinity();
				/*				if(numerator_0 < 0)
				{
				t1 = std::numeric_limits<float>::infinity();
				}else
				{

				}*/
			}
		}
	}
	else // denominator != 0.0f
	{
		t0 = numerator_0 / denominator;
		t1 = numerator_1 / denominator;
	}
}

vec4 Tree4DTraverserDifferentSides::calculateMidpoint(vec4& t0, vec4& t1)
{
	vec4 tm;

	for (int coord = 0; coord < 4; coord++) {
		//Check for each of the axis
		if (std::isinf(t0[coord]) && std::isinf(t1[coord])
			&& t0[coord] < 0 && t1[coord] > 0)
		{

			float mid_coord = (stack_TraversalInfo_about_Node4Ds.back().min[coord] + stack_TraversalInfo_about_Node4Ds.back().max[coord]) / 2.0;
			if (ray.origin[coord] < mid_coord)
			{
				tm[coord] = std::numeric_limits<float>::infinity();
			}
			else
			{
				tm[coord] = -1 * std::numeric_limits<float>::infinity();
			}
		}
		else
		{
			tm[coord] = 0.5f*(t0[coord] + t1[coord]);
		}
	}
	return tm;
}

Tree4DTraverserDifferentSides::~Tree4DTraverserDifferentSides(void)
{
}


/*
 * util.h
 *
 *  Created on: Feb 14, 2012
 *      Author: jeroenb
 */
#ifndef UTIL_H_
#define UTIL_H_

#define NULL 0

#include <iostream>
#include <sstream>
#include "string.h"
#include <fstream>
#include <time.h>
#include <TriMesh.h>

enum FileFormat { OCTREE, TREE4D, GRID }; //, TREE4DDIFFERENTSIDES};


typedef trimesh::Vec<3, double> vec3_d;
typedef trimesh::Vec<4, double> vec4_d;

#define showDebugTemp

std::string getTimeString();
void writePPM(int w, int h, unsigned char* array, std::string name);

struct CoordCounter{
	size_t x;
	size_t y;
	size_t z;
	size_t gridlength;
	CoordCounter(size_t gridlength): x(0),y(0),z(0), gridlength(gridlength){}
	void count(){
		z++;
		if(z >= gridlength){y++;z=0;}
		if(y >= gridlength){x++;y=0;}
		if(x >= gridlength){x=0;}
	}
};

struct Timer {
	clock_t Begin;
	Timer(){
		Begin = clock() * CLK_TCK;
	}
	void reset(){
		Begin = clock() * CLK_TCK;
	}
	double getTimeMilliseconds(){
		clock_t End = clock() * CLK_TCK;
		return (End - Begin)/1000;
	}
};

#endif /* UTIL_H_ */

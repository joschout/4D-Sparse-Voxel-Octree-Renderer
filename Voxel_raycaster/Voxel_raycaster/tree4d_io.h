#ifndef TREE4D_IO_H
#define TREE_IO_H

#include <stdio.h>
#include <fstream>
//#include "../libs/libtri/include/file_tools.h"
#include "Node4D.h"

using namespace std;

// File containing all the tree4D IO methods

// Internal format to represent an tree4D
struct Tree4DInfo {
	int version;
	string base_filename;
	size_t gridsize_S;
	size_t gridsize_T;
	size_t n_nodes; // the total amount of SVO Nodes
	size_t n_data; // the total amount of data payloads.
				   // This is not automatically the same as n_nodes,
				   // as you can have several nodes point to the same data.
				   // In the case of a geometry-only SVO,
				   // all nodes refer to the same voxel payload, at position 1.

	Tree4DInfo() : version(1), base_filename(string("")), gridsize_S(1024), gridsize_T(1024), n_nodes(0), n_data(0) {}
	Tree4DInfo(int version, string base_filename, size_t gridsize_S, size_t gridsize_T, size_t n_nodes, size_t n_data) : version(version), base_filename(base_filename), gridsize_S(gridsize_S), gridsize_T(gridsize_T), n_nodes(n_nodes), n_data(n_data) {}

	void print() const {
		cout << "  version: " << version << endl;
		cout << "  base_filename: " << base_filename << endl;
		cout << "  grid length space: " << gridsize_S << endl;
		cout << "  grid length time: " << gridsize_T << endl;
		cout << "  n_nodes: " << n_nodes << endl;
		cout << "  n_data: " << n_data << endl;
	}

/*	// check if all files required by Tri exist
	bool filesExist() const {
		string header = base_filename + string(".tree4d");
		string nodes = base_filename + string(".tree4dnodes");
		string data = base_filename + string(".tree4ddata");
		return (file_exists(header) && file_exists(nodes) && file_exists(data));
	}*/
};

inline size_t writeNode4D(FILE* node_out, const Node4D &n, size_t &b_node_pos);
inline void readNode4D(FILE* f, Node4D &n);

void writeTree4DHeader(const std::string &filename, const Tree4DInfo &i);
int parseTree4DHeader(const std::string &filename, Tree4DInfo &i);

// Write a tree4D node to file
inline size_t writeNode4D(FILE* node_out, const Node4D &n, size_t &b_node_pos) {
	/* https://github.com/Forceflow/ooc_svo_builder
	An .tree4dnodes file is a binary file
	which describes the big flat array of octree nodes.
	In the nodes, there are only child pointers,
	which are constructed from a 64-bit base address
	combined with a child offset,
	since all nonempty children of a certain node are guaranteed by the algorithm
	to be stored next to eachother.
	The .tree4dnodes file contains an amount of n_nodes nodes.

	SO:
	For each node,
	write 3 elements to the file
	1) the base address of the children of this node
	=> size_t, 64 bits
	2) the child ofsets for each of the 8 children.
	=> 16 children * 8 bits offset = 128 bits
	3) the data address = Index of data payload in data array described in the .octreedata file (see further)
	=> size_t, 64 bits
	==> 4 * 64 bits
	*/
	fwrite(&n.data, sizeof(size_t), 4, node_out);
	b_node_pos++;
	return b_node_pos - 1;
}

// Read a Node4D from a file
inline void readNode4D(FILE* f, Node4D &n) {
	fread(&n.data, sizeof(size_t), 4, f);
}

// Write an octree header to a file
inline void writeOctreeHeader(const std::string &filename, const Tree4DInfo &i) {
	ofstream outfile;
	outfile.open(filename.c_str(), ios::out);
	outfile << "#tree4Dheader 1" << endl;
	outfile << "gridlength_S " << i.gridsize_S << endl;
	outfile << "gridlength_T " << i.gridsize_T << endl;
	outfile << "n_nodes " << i.n_nodes << endl;
	outfile << "n_data " << i.n_data << endl;
	outfile << "END" << endl;
	outfile.close();
}

// Parse a given octree header, store info in OctreeInfo struct
inline int parseTree4DHeader(const std::string &filename, Tree4DInfo &i) {
	cout << "  reading tree4D header from " << filename << " ... " << endl;
	ifstream headerfile;
	headerfile.open(filename.c_str(), ios::in);

	if(!headerfile.is_open())
	{
		cout << endl;
		cout << "Failed to open Tree4D header file:" << endl;
		cout << filename << endl;
		std::cout << "Press ENTER to exit...";
		cin.get();
		exit(EXIT_SUCCESS);
	}


	i.base_filename = filename.substr(0, filename.find_last_of("."));

	string line; bool done = false;
	headerfile >> line >> i.version;
	if (line.compare("#tree4Dheader") != 0) { cout << "    Error: first line reads [" << line << "] instead of [#tree4Dheader]" << endl; return 0; }

	while (headerfile.good() && !done) {
		headerfile >> line;
		if (line.compare("END") == 0) done = true; // when we encounter data keyword, we're at the end of the ASCII header
		else if (line.compare("gridlength_S") == 0) { headerfile >> i.gridsize_S; }
		else if (line.compare("gridlength_T") == 0) { headerfile >> i.gridsize_T; }
		else if (line.compare("n_nodes") == 0) { headerfile >> i.n_nodes; }
		else if (line.compare("n_data") == 0) { headerfile >> i.n_data; }
		else {
			cout << "  unrecognized keyword [" << line << "], skipping" << endl;
			char c; do { c = headerfile.get(); } while (headerfile.good() && (c != '\n'));
		}
	}

	headerfile.close();
	return 1;
}

#endif

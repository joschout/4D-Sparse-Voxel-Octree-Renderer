#include "Tree4DReader.h"
#include <string>
#include <iostream>
#include <sstream>
#include "octree_io.h"


int readTree4D(const std::string& basefilename, Tree4D*& tree4D)
{
	cout << "Reading Tree4D from file..." << endl;

	// compute inputfile
	size_t splitpoint = basefilename.find_last_of(".");
	stringstream octreecachefile, nodefile, datafile;
	octreecachefile << basefilename.substr(0, splitpoint) << ".tree4d";
	nodefile << basefilename.substr(0, splitpoint) << ".tree4dnodes";
	datafile << basefilename.substr(0, splitpoint) << ".tree4ddata";

	Tree4DInfo info;
	parseTree4DHeader(basefilename, info);

	// start reading octree
	tree4D = new Tree4D(); // create empty octree
	tree4D->gridlength = info.gridlength;
	tree4D->n_data = info.n_data;
	tree4D->n_nodes = info.n_nodes;

	// read header and print statistics
	readTree4DNodes(info, tree4D->nodes);
	readTree4DData(info, &(tree4D->data));

	return 1;
}

/*int Tree4DReader::readTree4DHeader()
{
	
}*/

void readTree4DNodes(Tree4DInfo const &tree4d_info, std::vector<Node4D> &nodes)
{
	string filename = tree4d_info.base_filename + string(".tree4dnodes");
	FILE* file;
	errno_t err = fopen_s(&file,
		filename.c_str(),
		"rb");
	if (err == 0)
	{
		printf("The .tree4dnodes file was opened\n");
		nodes.reserve(tree4d_info.n_nodes);

		for (size_t i = 0; i< tree4d_info.n_nodes; i++) {
			Node4D n = Node4D();
			readNode4D(file, n);
			nodes.push_back(n);
		}
		fclose(file);
	}
	else
	{
		printf("Failed to open the .tree4dnodes file\n");
	}
}

void readTree4DData(Tree4DInfo const &tree4d_info, VoxelData** data)
{
	string filename = tree4d_info.base_filename + string(".tree4ddata");
	FILE* file;
	errno_t err = fopen_s(&file,
		filename.c_str(),
		"rb");
	if (err == 0)
	{
		printf("The .tree4ddata file was opened\n");
		*data = new VoxelData[tree4d_info.n_data];

		// read data
		for (size_t i = 0; i< tree4d_info.n_data; i++) {
			(*data)[i] = VoxelData();
			readVoxelData(file, (*data)[i]);
		}
		fclose(file);
	}
	else
	{
		printf("Failed to open the .tree4ddata file\n");
	}
}
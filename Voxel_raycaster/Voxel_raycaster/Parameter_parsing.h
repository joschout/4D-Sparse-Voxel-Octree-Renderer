#ifndef PARAMETER_PARSING_H
#define PARAMETER_PARSING_H
#include "util.h"
#include "printInfo.h"

inline void parseParameters(
	int argc, char **argv, std::string& file, FileFormat &inputformat, unsigned int& rendersize,
	bool& printTreeStructure, bool& debug, bool& run_tests) {
	if (argc < 2) {
		printInvalid(); 
		std::cout << "Press ENTER to exit...";
		cin.get();
		exit(0);
	}
	for (int i = 1; i < argc; i++) {
		if (std::string(argv[i]) == "-f") {
			file = argv[i + 1];

			//check if it is an octree file
			size_t check_octree = file.find(".octree");
			// check if  it is a tree4d file
			size_t check_tree4d = file.find(".tree4d");
			if (check_octree != std::string::npos || check_tree4d != std::string::npos) {
				if(check_octree != std::string::npos)
				{
					inputformat = OCTREE;
				}
				if(check_tree4d != std::string::npos)
				{
					inputformat = TREE4D;
				}
				
			}else {
				std::cout << "Received as filename: " << string(argv[i + 1]) << endl;
				std::cout << "Data filename does not end in .octree or in .tree4d - I only support those file formats" << std::endl;
				printInvalid();
				std::cout << "Press ENTER to exit...";
				cin.get();
				exit(0);
			}
			i++;
		}
		else if (std::string(argv[i]) == "-s") {
			rendersize = atoi(argv[i + 1]);
			i++;
		}
		else if (std::string(argv[i]) == "-printTree") {
			printTreeStructure = true;
		}
		else if (std::string(argv[i]) == "-debug") {
			debug = true;
		}
		else if (std::string(argv[i]) == "-run_tests") {
			run_tests = true;
		}
		else
		{
			printInvalid(); 
			cout << "Did not recognize: " << std::string(argv[i]) << endl;
			std::cout << "Press ENTER to exit...";
			cin.get();
			exit(0);
		}
	}
}
#endif


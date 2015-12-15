#include <stdio.h>  /* defines FILENAME_MAX */
#define WINDOWS
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include <iostream>
#include <omp.h>
#include <errno.h>

int printCurrentDirectory()
{
	char cCurrentPath[FILENAME_MAX];

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		return errno;
	}

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	std::cout << "The current working directory is:" <<  std::endl << cCurrentPath <<  std::endl;
	std::cout << "(You should put your octree files here)" << std::endl;
	std::cout << "" << std::endl;
	return 0;
}


void printInfo() {
	std::cout << "Master thesis implementation" << std::endl;
	std::cout << "Jonas Schouterden - 2015-2016" << std::endl;
	std::cout << "Based on the Voxel Raycaster made by Jeroen Baert - 2012" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "I'll be using " << omp_get_num_procs() << " CPU cores for rendering" << std::endl << std::endl;
}

void printControls() {
	std::cout << "Controls (on numeric keypad):" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "4-6: Camera left-right" << std::endl;
	std::cout << "8-5: Camera up-down" << std::endl;
	std::cout << "7-9: Camera nearer-further" << std::endl;
	std::cout << "i: save screenshot in .PPM format" << std::endl;
	std::cout << "p: toggle work rendering" << std::endl;
	std::cout << "-----------------------------" << std::endl;
}

void printInvalid() {
	std::cout << "Not enough or invalid arguments, please try again.\n" << std::endl;
	std::cout << "At the bare minimum, I need a path to a data file (binvox/octree)" << std::endl;
	std::cout << "For Example: voxelraycaster.exe -f /home/bunny256.octree" << std::endl;
}

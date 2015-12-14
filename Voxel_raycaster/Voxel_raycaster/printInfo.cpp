
#include <iostream>
#include <omp.h>

void printInfo() {
	std::cout << "Voxel Renderer Proof Of Concept" << std::endl;
	std::cout << "Jeroen Baert - 2012" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "jeroen.baert@cs.kuleuven.be" << std::endl;
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
	std::cout << "At the bare minimum, I need a path to a data file (binvox/avox)" << std::endl;
	std::cout << "For Example: voxelraycaster.exe -f /home/jeroen/bunny256.avox" << std::endl;
}

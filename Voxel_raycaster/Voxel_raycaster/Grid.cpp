#include "Grid.h"
#include <cmath>
#include <random>
#include <functional>

Grid::~Grid()
{
}

void Grid::initGridColorsRand()
{
	//  http://www.cplusplus.com/reference/random/ 
	
	std::random_device generator;
	std::uniform_int_distribution<int> distribution(0, 255);
	//std::function<int()> rndColor = std::bind(distribution,generator);

	auto numberOfCells = std::pow(gridlength, 3);
	gridCells.reserve(numberOfCells);
	for (size_t i = 0; i < numberOfCells; i++)
	{
		//gridCells.push_back(ivec3(rndColor(), rndColor(), rndColor()));
		gridCells.push_back(ivec3(distribution(generator), distribution(generator), distribution(generator)));
	}

}

bool Grid::inside(const vec3& point) const {
	return ((point[0] > min[0] && point[0] < max[0])
		&& (point[1] > min[1] && point[1] < max[1])
		&& (point[2] > min[2] && point[2] < max[2]));
};
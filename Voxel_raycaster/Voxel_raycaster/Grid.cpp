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
	std::uniform_real_distribution<float> distribution(0, 1);

	auto numberOfCells = std::pow(gridlength, 3);
	gridCells.reserve(numberOfCells);

	for (size_t i = 0; i < numberOfCells; i++)
	{
		GridPoint gridPoint = GridPoint();
		gridPoint.opacity = 1.0f;
		gridPoint.color = vec3(distribution(generator), distribution(generator), distribution(generator));
		gridCells.push_back(gridPoint);
	}

}

void Grid::initSparseColorsRand()
{
	std::random_device generator1;
	std::uniform_real_distribution<float> distribution1(0, 1);

	std::random_device generator2;
	std::uniform_int_distribution<int> distribution2(1, 100);

	auto numberOfCells = std::pow(gridlength, 3);
	gridCells.reserve(numberOfCells);

	for (size_t i = 0; i < numberOfCells; i++)
	{
		int sparseInt = distribution2(generator2);
		GridPoint gridPoint = GridPoint();

		if(sparseInt > 90){ //not empty
			gridPoint.opacity = 1.0f;
			gridPoint.color = vec3(distribution1(generator1), distribution1(generator1), distribution1(generator1));
		}else
		{
			//empty
		}
		gridCells.push_back(gridPoint);
	}
}

bool Grid::inside(const vec3& point) const {
	return ((point[0] > min[0] && point[0] < max[0])
		&& (point[1] > min[1] && point[1] < max[1])
		&& (point[2] > min[2] && point[2] < max[2]));
};
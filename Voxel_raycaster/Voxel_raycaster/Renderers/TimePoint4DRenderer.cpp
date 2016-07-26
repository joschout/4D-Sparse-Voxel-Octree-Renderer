#include "TimePoint4DRenderer.h"
#include <omp.h>
#include "../Tree4DTraverserDifferentSides.h"

TimePoint4DRenderer::TimePoint4DRenderer() : Tree4DRenderer("timepoint")
{
}

TimePoint4DRenderer::~TimePoint4DRenderer()
{
}

void TimePoint4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const
{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index, partindex;
	Tree4DTraverserDifferentSides t;
	double maxNumberOfTimeSteps = tree->gridsize_T;
	double unitlength_time = (tree->max[3] - tree->min[3])/ tree->gridsize_T;
	double endtime = tree->max[3];
	int total_amount = rc.n_x * rc.n_y;

	cout << "Rendering a timepoint image:" << endl;
	cout << "  calculating a value for each pixel:" << endl;

#pragma omp parallel for private(x,t,v,index,factor)
	for (int y = 0; y < rc.n_y; y++) {
		partindex = y*(rc.n_y * 4);
		for (x = 0; x < rc.n_x; x++) {
			index = partindex + x * 4; // index in char array computation (part 2)
						
			int current_amount = y *rc.n_y + x;
			if (current_amount % (total_amount / 100) == 0) {

				double progress = (double)current_amount / (double)total_amount;
				int barWidth = 70;

				std::cout << '\r' << "[";
				int pos = barWidth * progress;
				for (int i = 0; i < barWidth; ++i) {
					if (i < pos) std::cout << "=";
					else if (i == pos) std::cout << ">";
					else std::cout << " ";
				}
				std::cout << "] " << int(progress * 100.0) << " %\r";
				std::cout.flush();
			}
			
			
			Ray ray3D = rc.getRayForPixel(x, y);


			// the number of time steps in which this pixel is filled.
			double nbOfTimeSteps = 0.0;
			for (double timestep = 0.0; timestep < endtime; timestep = timestep + unitlength_time) {
				Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, timestep, 0.0);
				t = Tree4DTraverserDifferentSides(tree, ray4D);

				bool pixelIsFilledInThisTimestep = false;
				while (!t.isTerminated() && !pixelIsFilledInThisTimestep) {
					if (t.getCurrentNode()->isLeaf() &&
						t.getCurrentNode()->hasData()) {
							nbOfTimeSteps++;
							pixelIsFilledInThisTimestep = true;
						
					}else{
						t.step();
					}	
				}
			}
			calculateAndStoreColorForThisPixel(texture_array, index, nbOfTimeSteps, maxNumberOfTimeSteps);
		}
	}
}

void TimePoint4DRenderer::calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, double nbOfTimeSteps, double maxNumberOfTimeSteps) const
{
	double R, G, B;

	if (nbOfTimeSteps < 0.0 || nbOfTimeSteps > maxNumberOfTimeSteps)
	{
		R = 1.0;
		G = 1.0;
		B = 1.0;
	}
	else if (nbOfTimeSteps < maxNumberOfTimeSteps / 3.0) {
		R = 0.0;
		G = 3.0 / maxNumberOfTimeSteps*nbOfTimeSteps;
		B = 1.0 - (3.0 / maxNumberOfTimeSteps)*nbOfTimeSteps;
	}
	//GREEN = in between
	else if (nbOfTimeSteps < maxNumberOfTimeSteps * 2.0 / 3.0) {
		R = (3.0 / maxNumberOfTimeSteps)*(nbOfTimeSteps - maxNumberOfTimeSteps / 3.0);
		G = 1.0 - (3.0 / maxNumberOfTimeSteps)*(nbOfTimeSteps - maxNumberOfTimeSteps / 3.0);
		B = 0.0f;
	}
	else
	{
		R = 1.0f;
		G = 0.0f;
		B = 0.0f;
	}
	double r = (255 * R);
	double g = (255 * G);
	double b = (255 * B);


	/*	double r = 255;
	double g = 0;
	double b = 0;*/
	texture_array[index] = (unsigned char)r;
	texture_array[index + 1] = (unsigned char)g;
	texture_array[index + 2] = (unsigned char)b;
	texture_array[index + 3] = (unsigned char)1;
}

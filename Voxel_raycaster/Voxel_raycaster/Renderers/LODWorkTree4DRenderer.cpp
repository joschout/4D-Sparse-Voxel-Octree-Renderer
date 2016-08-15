#include "LODWorkTree4DRenderer.h"
#include <omp.h>
#include "../Tree4DTraverserDifferentSides.h"
#include "../ColorMap.h"
#include "../Globals.h"


//size_t LODWorkTree4DRenderer::max_step_count = 0;

size_t LODWorkTree4DRenderer::max_level = 0;

size_t LODWorkTree4DRenderer::smallest_stack_size = 0;
size_t LODWorkTree4DRenderer::largest_stack_size = 0;


double pixel_size_optimized = 0.0;

LODWorkTree4DRenderer::LODWorkTree4DRenderer(void) : Tree4DRenderer("LODWork")
{


}

LODWorkTree4DRenderer::~LODWorkTree4DRenderer()
{
}

void LODWorkTree4DRenderer::calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index_in_texture_array, Tree4DTraverserDifferentSides& treeTraverser) const
{
	double stepcount = (double)treeTraverser.stepcount;
	MAP_COLOUR colour = GetColour(stepcount, 0.0, static_cast<double>(max_step_count));
	double& R = colour.r;
	double& G = colour.g;
	double& B = colour.b;

	texture_array[index_in_texture_array] = (unsigned char) int(R * 255.0);
	texture_array[index_in_texture_array + 1] = (unsigned char) int(G * 255.0);
	texture_array[index_in_texture_array + 2] = (unsigned char) int(B * 255.0);
	texture_array[index_in_texture_array + 3] = (unsigned char)1;
}

void LODWorkTree4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const
{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int index_in_texture_array, partial_index_in_texture_array;
	Tree4DTraverserDifferentSides treeTraverser;


	double frustrum_width = rc.frustrum->right - rc.frustrum->left;
	double pixel_diameter = frustrum_width / rc.n_x;
	double pixel_radius = 0.5 * pixel_diameter;
	double pixel_size = pixel_radius * pixel_radius; // * PI

	size_t size = (tree->gridsize_T > tree->gridsize_S ? tree->gridsize_T : tree->gridsize_S);
	size_t max_stack_size = log(size) / log(2) + 1;

	smallest_stack_size = max_stack_size;
	largest_stack_size = 0;

#pragma omp parallel for private(x,t,v,index,factor)
	for (int y = 0; y < rc.n_y; y++) {

		partial_index_in_texture_array = y*(rc.n_y * 4);
		for (int x = 0; x < rc.n_x; x++) {

			index_in_texture_array = partial_index_in_texture_array + x * 4; // index in char array computation (part 2)
			double t_pixel; // dit is eigenlijk de t (staalparameter) tot het scherm
			Ray ray3D = rc.getRayForPixel_rayparam(x, y, t_pixel);
			Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, time_point, 0.0);
			treeTraverser = Tree4DTraverserDifferentSides(tree, ray4D);
			
			bool dataLeafNodeHasBeenFound = false;
			bool nodeIsToSmall = false;
			bool reachedMaxLevelToRender = false;
			while (!treeTraverser.isTerminated() && !dataLeafNodeHasBeenFound && !nodeIsToSmall && !reachedMaxLevelToRender) {

				if (treeTraverser.getCurrentNode()->isLeaf()
					&& treeTraverser.getCurrentNode()->hasData())
				{
					dataLeafNodeHasBeenFound = true;
				}

				if (treeTraverser.stack_TraversalInfo_about_Node4Ds.size() >= max_level)
				{
					reachedMaxLevelToRender = true;
				}

				double projectedSizeOfCurrentNode = treeTraverser.getProjectedSizeOfCurrentNode(t_pixel);
				if (projectedSizeOfCurrentNode <= pixel_size)
				{
					nodeIsToSmall = true;
				}


				if (dataLeafNodeHasBeenFound || reachedMaxLevelToRender || nodeIsToSmall) {
						size_t current_stack_size = treeTraverser.stack_TraversalInfo_about_Node4Ds.size();

						if (current_stack_size < smallest_stack_size)
						{
							smallest_stack_size = current_stack_size;
						}
						if (current_stack_size > largest_stack_size)
						{
							largest_stack_size = current_stack_size;
						}				
				}
				else
				{
					treeTraverser.step();
				}

			}
			if (treeTraverser.stepcount > max_step_count)
			{
				max_step_count = treeTraverser.stepcount;
			}

			calculateAndStoreColorForThisPixel(texture_array, index_in_texture_array, treeTraverser);
		}
	}
	if (debug)
	{
		std::cout << "smallest stack size: " << smallest_stack_size << endl
			<< "largest stack size: " << largest_stack_size << endl;
	}
}



void LODWorkTree4DRenderer::Render_optimized(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point, double pixel_size) const
{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int index_in_texture_array, partial_index_in_texture_array;
	Tree4DTraverserDifferentSides treeTraverser;

#pragma omp parallel for private(x,t,v,index,factor)
	for (int y = 0; y < rc.n_y; y++) {

		partial_index_in_texture_array = y*(rc.n_y * 4);
		for (int x = 0; x < rc.n_x; x++) {

			index_in_texture_array = partial_index_in_texture_array + x * 4; // index in char array computation (part 2)
			double t_pixel; // dit is eigenlijk de t (staalparameter) tot het scherm
			Ray ray3D = rc.getRayForPixel_rayparam(x, y, t_pixel);
			Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, time_point, 0.0);
			treeTraverser = Tree4DTraverserDifferentSides(tree, ray4D);

			bool dataLeafNodeHasBeenFound = false;
			bool nodeIsToSmall = false;
			while (!treeTraverser.isTerminated() && !dataLeafNodeHasBeenFound && !nodeIsToSmall) {

				if (treeTraverser.getCurrentNode()->isLeaf()
					&& treeTraverser.getCurrentNode()->hasData())
				{
					dataLeafNodeHasBeenFound = true;
				}

				double projectedSizeOfCurrentNode = treeTraverser.getProjectedSizeOfCurrentNode(t_pixel);
				if (projectedSizeOfCurrentNode <= pixel_size)
				{
					nodeIsToSmall = true;
				}


				if (dataLeafNodeHasBeenFound ||  nodeIsToSmall) {

				}
				else
				{
					treeTraverser.step();
				}

			}
			if (treeTraverser.stepcount > max_step_count)
			{
				max_step_count = treeTraverser.stepcount;
			}

			calculateAndStoreColorForThisPixel(texture_array, index_in_texture_array, treeTraverser);
		}
	}
}
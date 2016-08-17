#include "LODLevelTree4DRenderer.h"
#include <omp.h>
#include "../Tree4DTraverserDifferentSides.h"
#include "../Globals.h"
#include "../ColorMap.h"


size_t LODLevelTree4DRenderer::max_level = 0;

size_t LODLevelTree4DRenderer::smallest_stack_size = 0;
size_t LODLevelTree4DRenderer::largest_stack_size = 0;

LODLevelTree4DRenderer::LODLevelTree4DRenderer(void) : Tree4DRenderer("LODLevel")
{
}

LODLevelTree4DRenderer::~LODLevelTree4DRenderer()
{
}

void LODLevelTree4DRenderer::calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index_in_texture_array, size_t current_stack_size, size_t max_stack_size) const
{

	MAP_COLOUR stack_colour = GetColour(static_cast<double>(current_stack_size), 0.0, static_cast<double>(max_stack_size));
	double& R = stack_colour.r;
	double& G = stack_colour.g;
	double& B = stack_colour.b;
							
							
	texture_array[index_in_texture_array] = (unsigned char) int(R * 255.0);
	texture_array[index_in_texture_array + 1] = (unsigned char) int(G * 255.0);
	texture_array[index_in_texture_array + 2] = (unsigned char) int(B * 255.0);
	texture_array[index_in_texture_array + 3] = (unsigned char)1;
}

void LODLevelTree4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const
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

					calculateAndStoreColorForThisPixel(texture_array, index_in_texture_array, current_stack_size, max_stack_size);
				}
				else
				{
					treeTraverser.step();
				}
			}

		}
	}
	if (debug)
	{
		std::cout << "smallest stack size: " << smallest_stack_size << endl
			<< "largest stack size: " << largest_stack_size << endl;
	}
}


void LODLevelTree4DRenderer::Render_optimized(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point, double pixel_size) const
{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int index_in_texture_array, partial_index_in_texture_array;
	Tree4DTraverserDifferentSides treeTraverser;

	size_t size = (tree->gridsize_T > tree->gridsize_S ? tree->gridsize_T : tree->gridsize_S);
	size_t max_stack_size = log(size) / log(2) + 1;

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
					calculateAndStoreColorForThisPixel(texture_array, index_in_texture_array, current_stack_size, max_stack_size);
				}
				else
				{
					treeTraverser.step();
				}
			}
		}
	}

}
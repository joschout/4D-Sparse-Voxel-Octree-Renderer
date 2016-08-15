#include "LODDepthTree4DRenderer.h"
#include <omp.h>
#include "../Tree4DTraverserDifferentSides.h"
#include "../Globals.h"


size_t LODDepthTree4DRenderer::max_level = 0;

size_t LODDepthTree4DRenderer::smallest_stack_size = 0;
size_t LODDepthTree4DRenderer::largest_stack_size = 0;

LODDepthTree4DRenderer::LODDepthTree4DRenderer(void) : Tree4DRenderer("LODDepth")
{
}

LODDepthTree4DRenderer::~LODDepthTree4DRenderer()
{
}

void LODDepthTree4DRenderer::calculateAndStoreColorForThisPixel(unsigned char* texture_array, int indexInTextureArray, Tree4D const* tree, Tree4DTraverserDifferentSides& treeTraverser) const
{
	// ORIGINAL 
	double factor = abs(treeTraverser.getCurrentPosition()[2]) / (abs(tree->max[2]) - abs(tree->min[2]));


	//const double& far = rc.frustrum->far;
	//const double& near = rc.frustrum->near;

	//double distance = len(currentPosition - rc.camera->eye);

	// z-value in [-1, 1]
	// see https://en.wikipedia.org/wiki/Z-buffering
	//double z_value = ((far + near) + 1.0f / distance * (-2.0f * far * near)) / (far - near);


	//double z_value = 2 * (distance - near) / (far - near);

	//double factor = (z_value + 1.0) / 2.0;
	//double factor = len(currentPosition - vec3_d(tree->min[0], tree->min[1], tree->min[2])) / len(vec3_d(tree->max[0], tree->max[1], tree->max[2])- vec3_d(tree->min[0], tree->min[1], tree->min[2]));

	double r = 255 - (255 * factor);
	double g = 255 - (255 * factor);
	double b = 255 - (255 * factor);


	/*	double r = 255;
	double g = 0;
	double b = 0;*/
	texture_array[indexInTextureArray] = (unsigned char)r;
	texture_array[indexInTextureArray + 1] = (unsigned char)g;
	texture_array[indexInTextureArray + 2] = (unsigned char)b;
	texture_array[indexInTextureArray + 3] = (unsigned char)1;
}

void LODDepthTree4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const
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
					if (debug)
					{
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


					//						
					//						MAP_COLOUR stack_colour = GetColour(static_cast<double>(current_stack_size), 0.0, static_cast<double>(max_stack_size));
					//						double& R = stack_colour.r;
					//						double& G = stack_colour.g;
					//						double& B = stack_colour.b;
					//						
					//						
					//						texture_array[index_in_texture_array] = (unsigned char) int(R * 255.0);
					//						texture_array[index_in_texture_array + 1] = (unsigned char) int(G * 255.0);
					//						texture_array[index_in_texture_array + 2] = (unsigned char) int(B * 255.0);
					//						texture_array[index_in_texture_array + 3] = (unsigned char)1;

					calculateAndStoreColorForThisPixel(texture_array, index_in_texture_array, tree, treeTraverser);

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

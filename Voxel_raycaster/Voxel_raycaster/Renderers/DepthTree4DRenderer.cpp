#include "DepthTree4DRenderer.h"
#include <omp.h>
#include "../Tree4DTraverser.h"
#include "../Tree4DTraverserDifferentSides.h"
#include "../Globals.h"

DepthTree4DRenderer::DepthTree4DRenderer() : Tree4DRenderer("depth")
{
}

DepthTree4DRenderer::~DepthTree4DRenderer()
{
}


void DepthTree4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const
{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index_in_texture_array, partial_index_in_texture_array;
	Tree4DTraverserDifferentSides treeTraverser;


#pragma omp parallel for private(x,t,v,index,factor)
	for (int y = 0; y < rc.n_y; y++) {

		partial_index_in_texture_array = y*(rc.n_y * 4);
		for (x = 0; x < rc.n_y; x++) {

			index_in_texture_array = partial_index_in_texture_array + x * 4; // index in char array computation (part 2)
			Ray ray3D = rc.getRayForPixel(x, y);
			Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, time_point, 0.0f);

			//create a new tree traverser for each pixel
			treeTraverser = Tree4DTraverserDifferentSides(tree, ray4D);
			bool colorFoundForThisPixel = false;

/*			if (x == 320 && y == 320)
			{
				std::cout << "time to break" << std::endl;
			}*/

			while (!treeTraverser.isTerminated() && !colorFoundForThisPixel) {
				/*if (x == 320 && y == 320)
				{
					
					std::cout << "time to break" << std::endl;
					std::cout << "size of stack: " << treeTraverser.stack_TraversalInfo_about_Node4Ds.size() << std::endl;

					std::cout << "next child: " << treeTraverser.getCurrentNodeInfo().nextChildToCheck << std::endl;
					for (auto i = 0; i < 16; i++)
					{
						std::cout << "children offset: " << treeTraverser.getCurrentNode()->children_offset[i] << std::endl;
					}
					std::cout << "isLeaf: " << treeTraverser.getCurrentNode()->isLeaf() << std::endl;
					std::cout << "hasData: " << treeTraverser.getCurrentNode()->hasData() << std::endl;
				}*/


				if (treeTraverser.getCurrentNode()->isLeaf() && 
					treeTraverser.getCurrentNode()->hasData()) {
						calculateAndStoreColorForThisPixel(rc, tree, texture_array, index_in_texture_array, treeTraverser.getCurrentPosition());
						colorFoundForThisPixel = true;

#ifdef showDebugTemp
						tt_max = treeTraverser.getCurrentNodeInfo().max[3];
						tt_min = treeTraverser.getCurrentNodeInfo().min[3];
#endif

				}else
				{
					treeTraverser.step();
				}
			}
		}
	}
}


void DepthTree4DRenderer::calculateAndStoreColorForThisPixel(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, int indexInTextureArray, vec3_d currentPosition) const
{

	// ORIGINAL 
	double factor = abs(currentPosition[2]) / (abs(tree->max[2]) - abs(tree->min[2]));


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

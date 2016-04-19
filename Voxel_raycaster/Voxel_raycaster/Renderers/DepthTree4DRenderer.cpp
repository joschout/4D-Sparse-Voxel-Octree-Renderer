#include "DepthTree4DRenderer.h"
#include <omp.h>
#include "../Tree4DTraverser.h"
#include "../Tree4DTraverserDifferentSides.h"

DepthTree4DRenderer::DepthTree4DRenderer() : Tree4DRenderer("depth")
{
}

DepthTree4DRenderer::~DepthTree4DRenderer()
{
}


void DepthTree4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, float time_point) const
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

			while (!treeTraverser.isTerminated() && !colorFoundForThisPixel) {
				if (treeTraverser.getCurrentNode()->isLeaf() && 
					treeTraverser.getCurrentNode()->hasData()) {
						calculateAndStoreColorForThisPixel(tree, texture_array, index_in_texture_array, treeTraverser.getCurrentPosition());
						colorFoundForThisPixel = true;
				}else
				{
					treeTraverser.step();
				}
			}
		}
	}
}


void DepthTree4DRenderer::calculateAndStoreColorForThisPixel(Tree4D const* tree, unsigned char* texture_array, int indexInTextureArray, vec3 currentPosition) const
{
	float factor = abs(currentPosition[2]) / (abs(tree->max[2]) - abs(tree->min[2]));
	float r = 255 - (255 * factor);
	float g = 255 - (255 * factor);
	float b = 255 - (255 * factor);


	/*	float r = 255;
	float g = 0;
	float b = 0;*/
	texture_array[indexInTextureArray] = (unsigned char)r;
	texture_array[indexInTextureArray + 1] = (unsigned char)g;
	texture_array[indexInTextureArray + 2] = (unsigned char)b;
	texture_array[indexInTextureArray + 3] = (unsigned char)1;
}

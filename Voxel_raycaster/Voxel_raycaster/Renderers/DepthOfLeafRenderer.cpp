#include "DepthOfLeafRenderer.h"
#include <omp.h>
#include "../Tree4DTraverserDifferentSides.h"
#include "../NodeHitChecker.h"


DepthOfLeafRenderer::DepthOfLeafRenderer(void) : Tree4DRenderer("depthOfLeaf"), max_level(0)
{
}

DepthOfLeafRenderer::~DepthOfLeafRenderer()
{
}

void DepthOfLeafRenderer::calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, Tree4DTraverserDifferentSides& treeTraverser) const
{
	texture_array[index] = (unsigned char)255;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[0] * 255.0f);
	texture_array[index + 1] = (unsigned char)0;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
	texture_array[index + 2] = (unsigned char)0;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[2] * 255.0f);
	texture_array[index + 3] = (unsigned char)1;
}

void DepthOfLeafRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const
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
			Ray ray3D = rc.getRayForPixel(x, y);
			Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, time_point, 0.0);
			treeTraverser = Tree4DTraverserDifferentSides(tree, ray4D);

			bool reachedMaxLevelToRender = false;
			while (!treeTraverser.isTerminated()  && !reachedMaxLevelToRender) {

				if(treeTraverser.stack_TraversalInfo_about_Node4Ds.size() == max_level)
				{
					reachedMaxLevelToRender = true;
				}

				if(reachedMaxLevelToRender){


					texture_array[index_in_texture_array] = (unsigned char) 255;
					texture_array[index_in_texture_array + 1] = (unsigned char) 0;
					texture_array[index_in_texture_array + 2] = (unsigned char) 0;
					texture_array[index_in_texture_array + 3] = (unsigned char)1;
				}
				else
				{
					treeTraverser.step();
				}

			}
			
		}
	}
}

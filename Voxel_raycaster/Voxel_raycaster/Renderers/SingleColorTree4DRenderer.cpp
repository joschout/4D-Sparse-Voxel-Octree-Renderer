#include "SingleColorTree4DRenderer.h"
#include <omp.h>
#include "../Tree4DTraverserDifferentSides.h"

using namespace std;

SingleColorTree4DRenderer::SingleColorTree4DRenderer(void) : Tree4DRenderer("single_color")
{
}

void SingleColorTree4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const {
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index, partindex;
	Tree4DTraverserDifferentSides treeTraverser;

#pragma omp parallel for private(x,t,v,index,diffuse_factor,to_light,r,g,b)
	for (int y = 0; y < rc.n_y; y++) {
		partindex = y*(rc.n_y * 4);
		for (x = 0; x < rc.n_y; x++) {
			index = partindex + x * 4; // index in char array computation (part 2)
			Ray ray3D = rc.getRayForPixel(x, y);
			Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, time_point, 0.0f);
			treeTraverser = Tree4DTraverserDifferentSides(tree, ray4D);
			// DO A BASIC BOUNDING BOX TEST FOR THE TEST FIRST
			while ((!treeTraverser.isTerminated())) {
				if (treeTraverser.getCurrentNode()->isLeaf()
					&& treeTraverser.getCurrentNode()->hasData()) {//&& t.stack.back().t0.max() >0){

					calculateAndStoreColorForThisPixel(rc, tree, texture_array, index, treeTraverser);
					break;
				}
				treeTraverser.step();
			}
		}
	}
}

SingleColorTree4DRenderer::~SingleColorTree4DRenderer(void)
{
}


void SingleColorTree4DRenderer::calculateAndStoreColorForThisPixel(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, int index, Tree4DTraverserDifferentSides &treeTraverser) const
{
	texture_array[index] = 255;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[0] * 255.0f);
		texture_array[index + 1] = 0;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
		texture_array[index + 2] = 0;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[2] * 255.0f);
	texture_array[index + 3] = (unsigned char)1;
}

#include "LODTree4DRenderer.h"
#include <omp.h>
#include "../Tree4DTraverserDifferentSides.h"
#include "../NodeHitChecker.h"

LODTree4DRenderer::LODTree4DRenderer(void) : Tree4DRenderer("LOD")
{
}

LODTree4DRenderer::~LODTree4DRenderer()
{
}

void LODTree4DRenderer::calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, Tree4DTraverserDifferentSides& treeTraverser) const
{
	texture_array[index] = (unsigned char)255;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[0] * 255.0f);
	texture_array[index + 1] = (unsigned char)0;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
	texture_array[index + 2] = (unsigned char)0;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[2] * 255.0f);
	texture_array[index + 3] = (unsigned char)1;
}

void LODTree4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const
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
		for (x = 0; x < rc.n_x; x++) {

			index_in_texture_array = partial_index_in_texture_array + x * 4; // index in char array computation (part 2)
			Ray ray3D = rc.getRayForPixel(x, y);
			Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, time_point, 0.0);
			treeTraverser = Tree4DTraverserDifferentSides(tree, ray4D);

			


			bool nodeCompletelyOverlapsesPixel = true;

			//zolang er nog ongecheckte nodes zijn
			//EN de vorige node overlapte de huidige pixel
			while (!treeTraverser.isTerminated() && nodeCompletelyOverlapsesPixel) {

				nodeCompletelyOverlapsesPixel
					= hasNodeBeenHit(x, y, rc,
						treeTraverser.getCurrentNodeInfo(), time_point, tree);

				if (!nodeCompletelyOverlapsesPixel &&
					( !treeTraverser.getCurrentNode()->isLeaf() 
					|| (treeTraverser.getCurrentNode()->isLeaf() 
						&& treeTraverser.getCurrentNode()->hasData()))) {

					calculateAndStoreColorForThisPixel(texture_array, index_in_texture_array, treeTraverser);
					



					nodeCompletelyOverlapsesPixel = false;
				}
				else
				{
					treeTraverser.step();
				}
			}






		}
	}
}

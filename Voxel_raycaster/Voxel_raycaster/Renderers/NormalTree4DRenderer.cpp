#include "NormalTree4DRenderer.h"
#include <omp.h>
#include "../Tree4DTraverserDifferentSides.h"

NormalTree4DRenderer::NormalTree4DRenderer() : Tree4DRenderer("normal")
{
}

NormalTree4DRenderer::~NormalTree4DRenderer()
{
}

void NormalTree4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const
{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index_in_texture_array, partial_index_in_texture_array;
	Tree4DTraverserDifferentSides treeTraverser;


#pragma omp parallel for private(x,t,v,index,factor,to_light)
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

//				if (treeTraverser.stack_TraversalInfo_about_Node4Ds.back().t0.max()> 0.0f) {
					if (treeTraverser.getCurrentNode()->isLeaf() &&
						treeTraverser.getCurrentNode()->hasData()) {

//							vec3_d normal = tree->data[treeTraverser.getCurrentNode()->data].normal;
							vec3_d normal = tree->data_ptrs[treeTraverser.getCurrentNode()->data]->normal;
							normal = normalize(normal);
							float r = 255 * ((normal[0] + 1.0f) / 2.0f);
							float g = 255 * ((normal[1] + 1.0f) / 2.0f);
							float b = 255 * ((normal[2] + 1.0f) / 2.0f);

							texture_array[index_in_texture_array] = (unsigned char)r;
							texture_array[index_in_texture_array + 1] = (unsigned char)g;
							texture_array[index_in_texture_array + 2] = (unsigned char)b;
							texture_array[index_in_texture_array + 3] = (unsigned char)1;
							colorFoundForThisPixel = true;
//						}
				}else
				{
					treeTraverser.step();
				}	
			}
		}
	}
}

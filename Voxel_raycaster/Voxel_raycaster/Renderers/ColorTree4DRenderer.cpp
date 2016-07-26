#include "ColorTree4DRenderer.h"
#include <omp.h>
#include "../Tree4DTraverser.h"
#include "../Tree4DTraverserDifferentSides.h"

ColorTree4DRenderer::ColorTree4DRenderer() : Tree4DRenderer("color")
{
}

ColorTree4DRenderer::~ColorTree4DRenderer()
{
}

void ColorTree4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const
{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index_in_texture_array, partial_index_in_texture_array;
	Tree4DTraverserDifferentSides treeTraverser;

#pragma omp parallel for private(x,t,v,index)
	for (int y = 0; y < rc.n_y; y++) {

		partial_index_in_texture_array = y*(rc.n_y * 4);
		for (x = 0; x < rc.n_x; x++) {

			index_in_texture_array = partial_index_in_texture_array + x * 4; // index in char array computation (part 2)
			Ray ray3D = rc.getRayForPixel(x, y);
			Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, time_point, 0.0f);

			//create a new tree traverser for each pixel
			treeTraverser = Tree4DTraverserDifferentSides(tree, ray4D);
			bool colorFoundForThisPixel = false;

			while (!treeTraverser.isTerminated() && !colorFoundForThisPixel) {
				if (treeTraverser.getCurrentNode()->isLeaf() && 
					treeTraverser.getCurrentNode()->hasData()) {
//						texture_array[index_in_texture_array] = (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[0] * 255.0f);
//						texture_array[index_in_texture_array + 1] = (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
//						texture_array[index_in_texture_array + 2] = (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[2] * 255.0f);

						size_t data = treeTraverser.getCurrentNode()->data;
						vec3_d color = tree->data_ptrs[data]->color;
//						cout << "color: " << color << endl;
						int red = color[0] * 255.0;
						int green = color[1] * 255.0;
						int blue = color[2] * 255.0;

//						cout << "red: " << red << ", green: " << green << " , blue: " << blue << endl;
						texture_array[index_in_texture_array] = (unsigned char) int(red);
						texture_array[index_in_texture_array + 1] = (unsigned char) int(green);
						texture_array[index_in_texture_array + 2] = (unsigned char) int(blue);


//						texture_array[index_in_texture_array] = (unsigned char) int(tree->data_ptrs[treeTraverser.getCurrentNode()->data]->color[0] * 255.0f);
//						texture_array[index_in_texture_array + 1] = (unsigned char) int(tree->data_ptrs[treeTraverser.getCurrentNode()->data]->color[1] * 255.0f);
//						texture_array[index_in_texture_array + 2] = (unsigned char) int(tree->data_ptrs[treeTraverser.getCurrentNode()->data]->color[2] * 255.0f);

						texture_array[index_in_texture_array + 3] = (unsigned char)1;
						colorFoundForThisPixel = true;
				}else
				{
					treeTraverser.step();
				}	
			}
		}
	}
}
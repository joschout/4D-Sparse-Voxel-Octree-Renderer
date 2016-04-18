#include "WorkTree4DRenderer.h"
#include <omp.h>
#include "../Tree4DTraverser.h"


WorkTree4DRenderer::WorkTree4DRenderer(void) : Tree4DRenderer("work")
{
}

WorkTree4DRenderer::~WorkTree4DRenderer()
{
}

void WorkTree4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, float time_point) const
{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index, partindex;
	Tree4DTraverser t;
	double size = tree->gridsize_T;

#pragma omp parallel for private(x,t,v,index,factor)
	for (int y = 0; y < rc.n_y; y++) {
		partindex = y*(rc.n_y * 4);
		for (x = 0; x < rc.n_y; x++) {
			index = partindex + x * 4; // index in char array computation (part 2)
			Ray ray3D = rc.getRayForPixel(x, y);
			Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, time_point, 0.0f);
			t = Tree4DTraverser(tree, ray4D);
			while ((!t.isTerminated())) {
				if (t.getCurrentNode()->isLeaf()) {
					if (t.getCurrentNode()->hasData()) {
						break;
					}
				}
				t.step();
			}
			texture_array[index] = (unsigned char) int((float(t.stepcount) / (log2(size)))*255.0);
			texture_array[index + 3] = (unsigned char)1;
		}
	}
}

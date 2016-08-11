#include "WorkTree4DRenderer.h"
#include <omp.h>
#include "../Tree4DTraverserDifferentSides.h"
#include "../Globals.h"
#include "../ColorMap.h"


size_t WorkTree4DRenderer::max_step_count = 0;


WorkTree4DRenderer::WorkTree4DRenderer(void) : Tree4DRenderer("work")
{
}

WorkTree4DRenderer::~WorkTree4DRenderer()
{
}

void WorkTree4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const
{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index_in_texture_array, partial_index_in_texture_array;
	Tree4DTraverserDifferentSides treeTraverser;
	double size = (tree->gridsize_T > tree->gridsize_S ? tree->gridsize_T : tree->gridsize_S);

	
#pragma omp parallel for private(x,t,v,index,factor)
	for (int y = 0; y < rc.n_y; y++) {

		partial_index_in_texture_array = y*(rc.n_y * 4);
		for (x = 0; x < rc.n_x; x++) {

			index_in_texture_array = partial_index_in_texture_array + x * 4; // index in char array computation (part 2)
			Ray ray3D = rc.getRayForPixel(x, y);
			Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, time_point, 0.0);
			treeTraverser = Tree4DTraverserDifferentSides(tree, ray4D);

			bool dataLeafFoundForThisPixel = false;
			while (!treeTraverser.isTerminated() && !dataLeafFoundForThisPixel) {
				if (treeTraverser.getCurrentNode()->isLeaf() &&
					treeTraverser.getCurrentNode()->hasData()) {
						dataLeafFoundForThisPixel = true;
				}else{
					treeTraverser.step();
				}	
			}
			if(treeTraverser.stepcount > max_step_count)
			{
				max_step_count = treeTraverser.stepcount;
			}
			calculateAndStoreColorForThisPixel(texture_array, index_in_texture_array, treeTraverser, size);
#ifdef showDebugTemp
//			tt_max = treeTraverser.getCurrentNodeInfo().max[3];
//			tt_min = treeTraverser.getCurrentNodeInfo().min[3];
#endif
		}
	}
}

void WorkTree4DRenderer::calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, Tree4DTraverserDifferentSides &treeTraverser, double size) const
{


/*
	double work = double(treeTraverser.stepcount) / max_step_count;
//	double work = double(treeTraverser.stepcount) / log2(size);
	int work_as_colour = (int)(work * 255.0);

//	if(treeTraverser.hasBeenHitByRay)
//	{
//		cout << "has been hit by a ray" << endl;
//		cout << "isTerminated: " << treeTraverser.isTerminated() << endl;
//
//		if (treeTraverser.stack_TraversalInfo_about_Node4Ds.size() > 0) {
//			bool isLeaf = treeTraverser.getCurrentNode()->isLeaf();
//			bool hasData = treeTraverser.getCurrentNode()->hasData();
//			bool dataLeafFoundForThisPixel = (isLeaf &&hasData);
//			cout << "dataLeafFoundForThisPixel: " << dataLeafFoundForThisPixel << endl;
//		}
//		else
//		{
//			cout << "stack is empty" << endl;
//		}
//
//		cout << "stepcount: " << treeTraverser.stepcount << endl;
//		cout << "work: " << work << endl;
//	}

	texture_array[index] = (unsigned char) int(work_as_colour);
	texture_array[index + 1] = (unsigned char)0;
	texture_array[index + 2] = (unsigned char)0;
	texture_array[index + 3] = (unsigned char)1;

*/


	
	double stepcount = (double)treeTraverser.stepcount;

//	double R = 0.0;
//	double G = 0.0;
//	double B = 1.0;

//	if (stepcount < (double)(max_step_count / 3.0)) {
//		R = 0.0;
//		G = 3.0 / (double)max_step_count*stepcount;
//		B = 1.0 - (double)(3.0 / (double)max_step_count)*stepcount;
//		 
//	}
//	//GREEN = in between
//	else if (stepcount < (double)(max_step_count * 2.0 / 3.0)) {
//		 R = (3.0 / (double)max_step_count)*(stepcount - (double)max_step_count / 3.0);
//		 G = 1.0 - (3.0 / (double)max_step_count)*(stepcount - (double)max_step_count / 3.0);
//		 B = 0.0;
//	}
//	else {
//		R = 1.0;
//		G = 0.0;
//		B = 0.0;
//	}



//	if (stepcount < (double)max_step_count / 2.0)
//	{
//		R = 0.0;
//		G = stepcount * 2.0 / (double)max_step_count;
//		B = 1.0 - stepcount * 2.0 / (double)max_step_count;
//	}
//	else
//	{
//		R = 2.0 / (double)max_step_count * stepcount - 1.0;
//		G = 2.0 - 2.0 * stepcount / (double)max_step_count;
//		B = 0.0;
//	}


//	double R, G, B;
//	color_map2(R, G, B, stepcount, static_cast<double>(max_step_count));


	MAP_COLOUR colour = GetColour(stepcount, 0.0, static_cast<double>(max_step_count));
	double& R = colour.r;
	double& G = colour.g;
	double& B = colour.b;

	texture_array[index] = (unsigned char) int(R * 255.0);
	texture_array[index + 1] = (unsigned char) int(G * 255.0);
	texture_array[index + 2] = (unsigned char) int(B * 255.0);
	texture_array[index + 3] = (unsigned char)1;

}
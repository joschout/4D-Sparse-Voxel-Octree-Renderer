#include "PixelLODRenderer.h"
#include <omp.h>
#include "../Tree4DTraverserDifferentSides.h"
#include "../NodeHitChecker.h"
#include "../Globals.h"

size_t PixelLODRenderer::max_step_count = 0;

int PixelLODRenderer::stack_size_node_too_small = -1;
PixelLODRenderer::PixelLODRenderer(void) : Tree4DRenderer("pixelLOD"), max_level(0)
{
}

PixelLODRenderer::~PixelLODRenderer()
{
}

void PixelLODRenderer::calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, Tree4DTraverserDifferentSides& treeTraverser) const
{
	texture_array[index] = (unsigned char)255;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[0] * 255.0f);
	texture_array[index + 1] = (unsigned char)0;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
	texture_array[index + 2] = (unsigned char)0;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[2] * 255.0f);
	texture_array[index + 3] = (unsigned char)1;
}

void PixelLODRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const
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

	double size = (tree->gridsize_T > tree->gridsize_S ? tree->gridsize_T : tree->gridsize_S);
	int max_level = log2(static_cast<int>(max(tree->gridsize_S, tree->gridsize_T))) + 1;


	int x = 536; //static_cast<int>(selected_pixel_x);
	int y = 540; //static_cast<int>(selected_pixel_y);

	for (int test_index = 0; test_index < 1; test_index++) {


		cout << "TEST CASE: " << to_string(test_index) << endl;
		cout << "pixel x: " << to_string(x) << ", y: " << to_string(y) << endl;
		cout << endl;


		index_in_texture_array = y*(rc.n_y * 4) + x * 4; // index in char array computation (part 2)

		double t_pixel;// dit is eigenlijk de t (staalparameter) tot het scherm
		Ray ray3D = rc.getRayForPixel_rayparam(x, y, t_pixel);
		Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, time_point, 0.0);
		treeTraverser = Tree4DTraverserDifferentSides(tree, ray4D);
		cout << "t_pixel: " << to_string(t_pixel) << endl;
		cout << endl;


		cout << "treeTraverser info: " << endl
			<< "  original ray: " << endl
			<< "      origin: " << treeTraverser.original_ray.origin << endl
			<< "      direction: " << treeTraverser.original_ray.direction << endl
			<< "  ray: " << endl
			<< "      origin: " << treeTraverser.ray.origin << endl
			<< "      direction: " << treeTraverser.ray.direction << endl;


		bool dataLeafNodeHasBeenFound = false;
		// DO A BASIC BOUNDING BOX TEST FOR THE TEST FIRST
		bool nodeIsToSmall = false;
		bool reachedMaxLevelToRender = false;
		while (!treeTraverser.isTerminated() && !dataLeafNodeHasBeenFound && !nodeIsToSmall && !reachedMaxLevelToRender) {


			cout << "--------" << endl;
			cout << "step count: " << treeTraverser.stepcount << endl;
			cout << "stack size: " << treeTraverser.stack_TraversalInfo_about_Node4Ds.size() << endl;

			cout << treeTraverser.stack_TraversalInfo_about_Node4Ds.back().to_string();
			cout << endl;




			if (treeTraverser.getCurrentNode()->isLeaf()
				&& treeTraverser.getCurrentNode()->hasData())
			{
				dataLeafNodeHasBeenFound = true;
				cout << "current node is leaf with data" << endl;
			}

			if (treeTraverser.stack_TraversalInfo_about_Node4Ds.size() >= max_level)
			{
				reachedMaxLevelToRender = true;
				cout << "max level in stach reached!" << endl
					<< "   max_level: " << to_string(max_level) << endl;
			}


			double projectedSizeOfCurrentNode = treeTraverser.getProjectedSizeOfCurrentNode(t_pixel);
			if (projectedSizeOfCurrentNode <= pixel_size)
			{
				nodeIsToSmall = true;
			}

			if (projectedSizeOfCurrentNode <= pixel_size)
			{
				cout << "NODE IS TO SMALL" << endl;
				nodeIsToSmall = true;
			}else
			{
				cout << "node is still large enough" << endl;
			}


			//============================================//

			if (dataLeafNodeHasBeenFound || reachedMaxLevelToRender || nodeIsToSmall) {


				if (dataLeafNodeHasBeenFound) {
					// RED
					texture_array[index_in_texture_array] = (unsigned char)255;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
				}

				if (reachedMaxLevelToRender) {
					// GREEN 
					texture_array[index_in_texture_array + 1] = (unsigned char)255;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
				}
				if (nodeIsToSmall) {
					// BLUE
					texture_array[index_in_texture_array + 2] = (unsigned char)255;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
				}
				texture_array[index_in_texture_array + 3] = (unsigned char)1;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);

			}

			else
			{
				treeTraverser.step();
			}
		}
		cout << "END OF TRAVERSAL" << endl
			<< "======================================" << endl;

	}
}

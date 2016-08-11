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
	double frustrum_height = rc.frustrum->top - rc.frustrum->bottom;
	double pixel_area = frustrum_width / rc.n_x * frustrum_height / rc.n_y;
	double size = (tree->gridsize_T > tree->gridsize_S ? tree->gridsize_T : tree->gridsize_S);
	int max_level = log2(static_cast<int>(max(tree->gridsize_S, tree->gridsize_T))) + 1;


	int x;// = static_cast<int>(selected_pixel_x);
	int y;// = static_cast<int>(selected_pixel_y);

	for (int test_index = 0; test_index < 1; test_index++) {


		if (test_index == 0)
		{
			x = 320;
			y = 321;
		}
//		else
//		{
//			x = 320;
//			y = 380;
//		}
		cout << "TEST CASE: " << to_string(test_index) << endl;
		cout << "pixel x: " << to_string(x) << ", y: " << to_string(y) << endl;
		cout << endl;


		index_in_texture_array = y*(rc.n_y * 4) + x * 4; // index in char array computation (part 2)


		vec3_d vector_to_current_pixel = rc.getPixelCoordinate(x, y) - rc.camera->eye;
		double t_pixel = abs(len(vector_to_current_pixel));
		// dit is eigenlijk de t (staalparameter) tot het scherm
		cout << "t_pixel: " << to_string(t_pixel) << endl;
		cout << endl;

		Ray ray3D = rc.getRayForPixel(x, y);
		Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, time_point, 0.0);
		treeTraverser = Tree4DTraverserDifferentSides(tree, ray4D);
		//			PixelNodeChecker pixel_node_checker = PixelNodeChecker(x, y, rc, time_point, tree);


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

			//===============================================//
			//				//NIEUW
			//				// tc_max = min(min(tx1, ty1), tz1)
			//				vec4_d& t1 = treeTraverser.stack_TraversalInfo_about_Node4Ds.back().t1;
			////				vec4_d& t0 = treeTraverser.stack_TraversalInfo_about_Node4Ds.back().t0;
			////				double t_min = max(max(t0[0], t0[1]), t0[2]);
			//				double t_max = min(min(t1[0], t1[1]), t1[2]);
			//				vec4_d& min = treeTraverser.stack_TraversalInfo_about_Node4Ds.back().min;
			//				vec4_d& max = treeTraverser.stack_TraversalInfo_about_Node4Ds.back().max;
			//
			//				double voxelFaceArea = abs(max[0] - min[0])* abs(max[1] - min[1]);
			//				double area_allowed_at_t_max = pixel_area * t_max / t_pixel;
			//				if(voxelFaceArea <= area_allowed_at_t_max)
			//				{
			//					nodeIsToSmall = true;
			//					//cout << "LOD: node is to small!" << endl;
			//				}


			//NIEUW
			// tc_max = min(min(tx1, ty1), tz1)
			//vec4_d& t0 = treeTraverser.stack_TraversalInfo_about_Node4Ds.back().t0;
			vec4_d& t1 = treeTraverser.stack_TraversalInfo_about_Node4Ds.back().t1;
			//vec4_d& t1 = treeTraverser.stack_TraversalInfo_about_Node4Ds.back().t1;
			//				vec4_d& t0 = treeTraverser.stack_TraversalInfo_about_Node4Ds.back().t0;
			//				double t_min = max(max(t0[0], t0[1]), t0[2]);
			double t_max = min(min(t1[0], t1[1]), t1[2]);
			vec4_d& min = treeTraverser.stack_TraversalInfo_about_Node4Ds.back().min;
			vec4_d& max = treeTraverser.stack_TraversalInfo_about_Node4Ds.back().max;




			double t_pixel_corrected = t_pixel;
			if (t_max < 0)
			{
				t_pixel_corrected = -t_pixel;
			}


			double voxelDiameter = abs(max[0] - min[0]);
			double voxelRadius = 0.5*voxelDiameter;

			double voxelSizeToProject = voxelRadius * voxelRadius; // * PI
			double projectedSize = voxelSizeToProject * t_pixel_corrected / t_max;

			double pixel_diameter = frustrum_width / rc.n_x;
			double pixel_radius = 0.5 * pixel_diameter;
			double pixel_size = pixel_radius * pixel_radius; // * PI


			
			cout
				<< "projection info: " << endl
				<< "   voxel diameter: " << to_string(voxelDiameter) << endl
				<< "   voxel radius: " << to_string(voxelRadius) << endl
				<< "   voxel projected size: " << projectedSize << endl
				<< "   pixel diameter: " << to_string(pixel_diameter) << endl
				<< "   pixel radius: " << to_string(pixel_radius) << endl
				<< "   pixel size: " << to_string(pixel_size) << endl;

			if (projectedSize <= pixel_size)
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

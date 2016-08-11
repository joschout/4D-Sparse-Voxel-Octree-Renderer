#include "LODTree4DRenderer.h"
#include <omp.h>
#include "../Tree4DTraverserDifferentSides.h"
#include "../NodeHitChecker.h"
#include "../ColorMap.h"


size_t LODTree4DRenderer::max_level = 0;

size_t LODTree4DRenderer::smallest_stack_size = 0;
size_t LODTree4DRenderer::largest_stack_size = 0;

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


	double frustrum_width = rc.frustrum->right - rc.frustrum->left;
	double frustrum_height = rc.frustrum->top - rc.frustrum->bottom;
	double pixel_area = frustrum_width / rc.n_x * frustrum_height / rc.n_y;
	size_t size = (tree->gridsize_T > tree->gridsize_S ? tree->gridsize_T : tree->gridsize_S);
	size_t max_stack_size = log(size) / log(2) + 1;

	smallest_stack_size = max_stack_size + 10;
	largest_stack_size = 0;

#pragma omp parallel for private(x,t,v,index,factor)
	for (int y = 0; y < rc.n_y; y++) {

		partial_index_in_texture_array = y*(rc.n_y * 4);
		for (x = 0; x < rc.n_x; x++) {

			index_in_texture_array = partial_index_in_texture_array + x * 4; // index in char array computation (part 2)
			if( x < 10 && y < 10)
			{
				texture_array[index_in_texture_array] = (unsigned char)255;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[0] * 255.0f);
				texture_array[index_in_texture_array + 1] = (unsigned char)255;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
				texture_array[index_in_texture_array + 2] = (unsigned char)255;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[2] * 255.0f);
				texture_array[index_in_texture_array + 3] = (unsigned char)1;
				
			}else if(x == 14 && y == 14)
			{
				texture_array[index_in_texture_array] = (unsigned char)255;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[0] * 255.0f);
				texture_array[index_in_texture_array + 1] = (unsigned char)255;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
				texture_array[index_in_texture_array + 2] = (unsigned char)255;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[2] * 255.0f);
				texture_array[index_in_texture_array + 3] = (unsigned char)1;
			}
			else {

				vec3_d vector_to_current_pixel = rc.getPixelCoordinate(x, y) - rc.camera->eye;
				double t_pixel = abs(len(vector_to_current_pixel));
				// dit is eigenlijk de t (staalparameter) tot het scherm


				
				Ray ray3D = rc.getRayForPixel(x, y);
				Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, time_point, 0.0);
				treeTraverser = Tree4DTraverserDifferentSides(tree, ray4D);
				//			PixelNodeChecker pixel_node_checker = PixelNodeChecker(x, y, rc, time_point, tree);

				bool dataLeafNodeHasBeenFound = false;
				// DO A BASIC BOUNDING BOX TEST FOR THE TEST FIRST
				bool nodeIsToSmall = false;
				bool reachedMaxLevelToRender = false;
				while (!treeTraverser.isTerminated() && !dataLeafNodeHasBeenFound && !nodeIsToSmall && !reachedMaxLevelToRender) {

					if (treeTraverser.getCurrentNode()->isLeaf()
						&& treeTraverser.getCurrentNode()->hasData())
					{
						dataLeafNodeHasBeenFound = true;
					}

					if (treeTraverser.stack_TraversalInfo_about_Node4Ds.size() >= max_level)
					{
						reachedMaxLevelToRender = true;
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


					double voxelDiameter =  abs(max[0] - min[0]);//len(max - min);
					double voxelRadius = 0.5*voxelDiameter;

					double voxelSizeToProject = voxelRadius * voxelRadius; // * PI
					double projectedSize = voxelSizeToProject * t_pixel_corrected / t_max;

					double pixel_diameter = frustrum_width / rc.n_x;
					double pixel_radius = 0.5 * pixel_diameter;
					double pixel_size = pixel_radius * pixel_radius; // * PI


					if (projectedSize <= pixel_size)
					{

						nodeIsToSmall = true;
					}


					//============================================//

					if (dataLeafNodeHasBeenFound || reachedMaxLevelToRender || nodeIsToSmall) {






						size_t current_stack_size = treeTraverser.stack_TraversalInfo_about_Node4Ds.size();


						if(current_stack_size < smallest_stack_size)
						{
							smallest_stack_size = current_stack_size;
						}
						if(current_stack_size > largest_stack_size)
						{
							largest_stack_size = current_stack_size;
						}


//						
//						MAP_COLOUR stack_colour = GetColour(static_cast<double>(current_stack_size), 0.0, static_cast<double>(max_stack_size));
//						double& R = stack_colour.r;
//						double& G = stack_colour.g;
//						double& B = stack_colour.b;
//						
//						
//						texture_array[index_in_texture_array] = (unsigned char) int(R * 255.0);
//						texture_array[index_in_texture_array + 1] = (unsigned char) int(G * 255.0);
//						texture_array[index_in_texture_array + 2] = (unsigned char) int(B * 255.0);
//						texture_array[index_in_texture_array + 3] = (unsigned char)1;

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
							texture_array[index_in_texture_array + 3] = (unsigned char)1;

//						if (dataLeafNodeHasBeenFound) {
//							// RED
//							texture_array[index_in_texture_array] = (unsigned char)255;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
//						}
//
//						if (reachedMaxLevelToRender) {
//							// GREEN 
//							texture_array[index_in_texture_array + 1] = (unsigned char)255;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
//						}
//						if (nodeIsToSmall) {
//							// BLUE
//							texture_array[index_in_texture_array + 2] = (unsigned char)255;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
//						}
//						texture_array[index_in_texture_array + 3] = (unsigned char)1;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);

					}













//
//						double factor = abs(treeTraverser.getCurrentPosition()[2]) / (abs(tree->max[2]) - abs(tree->min[2]));
//						double r = 255 - (255 * factor);
//						double g = 255 - (255 * factor);
//						double b = 255 - (255 * factor);
//
//
//						if (reachedMaxLevelToRender)
//						{
//							texture_array[index_in_texture_array] = (unsigned char)0;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[0] * 255.0f);
//							texture_array[index_in_texture_array + 1] = (unsigned char)255;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
//							texture_array[index_in_texture_array + 2] = (unsigned char)0;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[2] * 255.0f);
//							texture_array[index_in_texture_array + 3] = (unsigned char)1;
//						}
//						if (dataLeafNodeHasBeenFound && ! nodeIsToSmall)
//						{
////							size_t data = treeTraverser.getCurrentNode()->data;
////							vec3_d color = tree->data_ptrs[data]->color;
////							//						cout << "color: " << color << endl;
////							int red = color[0] * 255.0;
////							int green = color[1] * 255.0;
////							int blue = color[2] * 255.0;
//
//							//						cout << "red: " << red << ", green: " << green << " , blue: " << blue << endl;
////							texture_array[index_in_texture_array] = (unsigned char) int(red);
////							texture_array[index_in_texture_array + 1] = (unsigned char) int(green);
////							texture_array[index_in_texture_array + 2] = (unsigned char) int(blue);
////							texture_array[index_in_texture_array + 3] = (unsigned char)1;
//
//
//							texture_array[index_in_texture_array] = (unsigned char) 255;
//							texture_array[index_in_texture_array + 1] = (unsigned char) 0;
//							texture_array[index_in_texture_array + 2] = (unsigned char) 0;
//							texture_array[index_in_texture_array + 3] = (unsigned char)1;



//							texture_array[index_in_texture_array] = (unsigned char)r;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[0] * 255.0f);
//							texture_array[index_in_texture_array + 1] = (unsigned char)0;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
//							texture_array[index_in_texture_array + 2] = (unsigned char)0;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[2] * 255.0f);
//							texture_array[index_in_texture_array + 3] = (unsigned char)1;
//						}
//						if (nodeIsToSmall && ! dataLeafNodeHasBeenFound)
//						{
//							
//							size_t data = treeTraverser.getCurrentNode()->data;
//							vec3_d color = tree->data_ptrs[data]->color;
//							//						cout << "color: " << color << endl;
//							int red = color[0] * 255.0;
//							int green = color[1] * 255.0;
//							int blue = color[2] * 255.0;
//
//							//						cout << "red: " << red << ", green: " << green << " , blue: " << blue << endl;
//							texture_array[index_in_texture_array] = (unsigned char) int(red);
//							texture_array[index_in_texture_array + 1] = (unsigned char) int(green);
//							texture_array[index_in_texture_array + 2] = (unsigned char) int(blue);
//							texture_array[index_in_texture_array + 3] = (unsigned char)1;
//
//
//
////							texture_array[index_in_texture_array] = (unsigned char)r;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[0] * 255.0f);
////							texture_array[index_in_texture_array + 1] = (unsigned char)g;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
////							texture_array[index_in_texture_array + 2] = (unsigned char)b;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[2] * 255.0f);
////							texture_array[index_in_texture_array + 3] = (unsigned char)1;
//						}
//						if(nodeIsToSmall && dataLeafNodeHasBeenFound)
//						{
//							texture_array[index_in_texture_array] = (unsigned char) 1;
//							texture_array[index_in_texture_array + 1] = (unsigned char) 1;
//							texture_array[index_in_texture_array + 2] = (unsigned char) 1;
//							texture_array[index_in_texture_array + 3] = (unsigned char)1;
//						}
//
//
//
//						//calculateAndStoreColorForThisPixel(texture_array, index_in_texture_array, treeTraverser);
//					}
					else
					{
						treeTraverser.step();
					}

				}
			}


//			bool currentlevelIsCorrectLevel = false;
//			while (!treeTraverser.isTerminated() && !currentlevelIsCorrectLevel) {
//
//				double area_side_of_node =
//					abs(treeTraverser.getCurrentNodeInfo().max[0] - treeTraverser.getCurrentNodeInfo().min[0])
//					* abs(treeTraverser.getCurrentNodeInfo().max[1] - treeTraverser.getCurrentNodeInfo().min[1]);
//
//
//				vec3_d vector_to_current_node = (treeTraverser.getCurrentPosition() - rc.camera->eye);
//				double distance_to_current_node = abs(len(vector_to_current_node));
//
//
////				double minimal_area_of_node_at_this_distance =
////					pixel_area * distance_to_current_node / distance_to_current_pixel;
//
//
//				if (area_side_of_node < pixel_area//minimal_area_of_node_at_this_distance 
//					||
//					(treeTraverser.getCurrentNode()->isLeaf() &&
//					treeTraverser.getCurrentNode()->hasData())
//					) {
//
////					size_t data = treeTraverser.getCurrentNode()->data;
////					vec3_d color = tree->data_ptrs[data]->color;
////
////					int red = color[0] * 255.0;
////					int green = color[1] * 255.0;
////					int blue = color[2] * 255.0;
////
////					texture_array[index_in_texture_array] = (unsigned char) int(red);
////					texture_array[index_in_texture_array + 1] = (unsigned char) int(green);
////					texture_array[index_in_texture_array + 2] = (unsigned char) int(blue);				
////					texture_array[index_in_texture_array + 3] = (unsigned char)1;
//
//
//
//					//					cout << "treeTraverser.stack_TraversalInfo_about_Node4Ds.size(): " << treeTraverser.stack_TraversalInfo_about_Node4Ds.size() << endl;
//				//	calculateAndStoreColorForThisPixel(texture_array, index_in_texture_array, treeTraverser);
//
//
//
//					double stepcount = (double)treeTraverser.stepcount;
//					double R = 0.0;
//					double G = 0.0;
//					double B = 1.0;
//
//					if (stepcount < (double)(max_step_count / 3.0)) {
//						R = 0.0;
//						G = 3.0 / (double)max_step_count*stepcount;
//						B = 1.0 - (double)(3.0 / (double)max_step_count)*stepcount;
//
//					}
//					//GREEN = in between
//					else if (stepcount < (double)(max_step_count * 2.0 / 3.0)) {
//						R = (3.0 / (double)max_step_count)*(stepcount - (double)max_step_count / 3.0);
//						G = 1.0 - (3.0 / (double)max_step_count)*(stepcount - (double)max_step_count / 3.0);
//						B = 0.0;
//					}
//					else {
//						R = 1.0;
//						G = 0.0;
//						B = 0.0;
//					}
//
//
//
//					texture_array[index_in_texture_array] = (unsigned char) int(R * 255.0);
//					texture_array[index_in_texture_array + 1] = (unsigned char) int(G * 255.0);
//					texture_array[index_in_texture_array + 2] = (unsigned char) int(B * 255.0);
//					texture_array[index_in_texture_array + 3] = (unsigned char)1;
//
//
//
//					currentlevelIsCorrectLevel = true; // we stop here, break the while loop
//				}
//				else {
//					treeTraverser.step();
//				}
//
//			}
//
//
//			if (treeTraverser.stepcount > max_step_count)
//			{
//				max_step_count = treeTraverser.stepcount;
//			}





		}
	}
	cout << "smallest stack size: " << smallest_stack_size << endl
		<< "largest stack size: " << largest_stack_size << endl;
}

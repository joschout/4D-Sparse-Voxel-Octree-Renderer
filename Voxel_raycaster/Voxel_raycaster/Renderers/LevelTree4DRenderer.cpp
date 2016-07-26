#include "LevelTree4DRenderer.h"
#include <omp.h>

LevelTree4DRenderer::LevelTree4DRenderer(size_t max_level) : Tree4DRenderer("level"), max_level(max_level)
{
}

LevelTree4DRenderer::~LevelTree4DRenderer()
{
}

void LevelTree4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const
{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index_in_texture_array, partial_index_in_texture_array;
	Tree4DTraverserDifferentSides treeTraverser;
	vec3_d to_light;
	double diffuse_factor, r, g, b, distancecut;


#pragma omp parallel for private(x,t,v,index,diffuse_factor,to_light,r,g,b)
	for (int y = 0; y < rc.n_y; y++) {

		partial_index_in_texture_array = y*(rc.n_y * 4);
		for (x = 0; x < rc.n_x; x++) {

			index_in_texture_array = partial_index_in_texture_array + x * 4; // index in char array computation (part 2)
			Ray ray3D = rc.getRayForPixel(x, y);
			Ray4D ray4D = Ray4D::convertRayTo4D(ray3D, time_point, 0.0);
			treeTraverser = Tree4DTraverserDifferentSides(tree, ray4D);


			bool currentlevelIsCorrectLevel = false;
			while (!treeTraverser.isTerminated() && !currentlevelIsCorrectLevel) {
				if (treeTraverser.stack_TraversalInfo_about_Node4Ds.size() >= max_level) {

//					cout << "treeTraverser.stack_TraversalInfo_about_Node4Ds.size(): " << treeTraverser.stack_TraversalInfo_about_Node4Ds.size() << endl;
					calculateAndStoreColorForThisPixel(texture_array, index_in_texture_array, tree, treeTraverser);

					currentlevelIsCorrectLevel = true; // we stop here, break the while loop
				} else{
					treeTraverser.step();
				}
				
			}
		}
	}
}


void LevelTree4DRenderer::calculateAndStoreColorForThisPixel(unsigned char* texture_array, int index, Tree4D const* tree, Tree4DTraverserDifferentSides& treeTraverser) const
{

	//					VoxelData* data = tree->data;
	//
	//					r = 0.0f;
	//					g = 0.0f;
	//					b = 0.0f;
	//					for (int i = 0; i<rc.lights.size(); i++) {
	//						if (rc.lights[i].active) {
	//							to_light = rc.lights[i].position - t.getCurrentPosition();
	//							vec3_d s = to_light + (rc.camera->eye - t.getCurrentPosition());
	//							s = normalize(s);
	//
	//							distancecut = 1.0f / (
	//								rc.lights[i].CONSTANT_ATTENUATION +
	//								rc.lights[i].LINEAR_ATTENUATION*len(to_light) +
	//								rc.lights[i].QUADRATIC_ATTENUATION*len2(to_light));
	//
	//							to_light = normalize(to_light);
	//
	//							// Diffuse
	//							diffuse_factor = data[t.getCurrentNode()->data].normal DOT to_light;
	//							r += std::max(0.0, diffuse_factor) * distancecut * rc.lights[i].diffuse[0];
	//							g += std::max(0.0, diffuse_factor) * distancecut * rc.lights[i].diffuse[1];
	//							b += std::max(0.0, diffuse_factor) * distancecut * rc.lights[i].diffuse[2];
	//
	//							// Phong
	//							double phong_factor = pow(s DOT data[t.getCurrentNode()->data].normal, rc.lights[i].SHININESS);
	//							r += std::max(0.0, phong_factor) * distancecut * rc.lights[i].specular[0];
	//							g += std::max(0.0, phong_factor) * distancecut * rc.lights[i].specular[1];
	//							b += std::max(0.0, phong_factor) * distancecut * rc.lights[i].specular[2];
	//
	//						}
	//					}
	//					texture_array[index] = (unsigned char)clampf(255 * (r), 0, 255);
	//					texture_array[index + 1] = (unsigned char)clampf(255 * (g), 0, 255);
	//					texture_array[index + 2] = (unsigned char)clampf(255 * (b), 0, 255);
	//					texture_array[index + 3] = (unsigned char)1;



//	size_t data = treeTraverser.getCurrentNode()->data;
//	vec3_d color = tree->data_ptrs[data]->color;
//	//						cout << "color: " << color << endl;
//	int red = color[0] * 255.0;
//	int green = color[1] * 255.0;
//	int blue = color[2] * 255.0;
//
//	//						cout << "red: " << red << ", green: " << green << " , blue: " << blue << endl;
//	texture_array[index] = (unsigned char) int(red);
//	texture_array[index + 1] = (unsigned char) int(green);
//	texture_array[index + 2] = (unsigned char) int(blue);
//
//
//	//						texture_array[index_in_texture_array] = (unsigned char) int(tree->data_ptrs[treeTraverser.getCurrentNode()->data]->color[0] * 255.0f);
//	//						texture_array[index_in_texture_array + 1] = (unsigned char) int(tree->data_ptrs[treeTraverser.getCurrentNode()->data]->color[1] * 255.0f);
//	//						texture_array[index_in_texture_array + 2] = (unsigned char) int(tree->data_ptrs[treeTraverser.getCurrentNode()->data]->color[2] * 255.0f);
//
//	texture_array[index + 3] = (unsigned char)1;

//	texture_array[index] = 255;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[0] * 255.0f);
//	texture_array[index + 1] = 0;//(unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[1] * 255.0f);
//	texture_array[index + 2] = 0;// (unsigned char) int(tree->data[treeTraverser.getCurrentNode()->data].color[2] * 255.0f);
//	texture_array[index + 3] = (unsigned char)1;




// ORIGINAL 
	double factor = abs(treeTraverser.getCurrentPosition()[2]) / (abs(tree->max[2]) - abs(tree->min[2]));


	//const double& far = rc.frustrum->far;
	//const double& near = rc.frustrum->near;

	//double distance = len(currentPosition - rc.camera->eye);

	// z-value in [-1, 1]
	// see https://en.wikipedia.org/wiki/Z-buffering
	//double z_value = ((far + near) + 1.0f / distance * (-2.0f * far * near)) / (far - near);


	//double z_value = 2 * (distance - near) / (far - near);

	//double factor = (z_value + 1.0) / 2.0;
	//double factor = len(currentPosition - vec3_d(tree->min[0], tree->min[1], tree->min[2])) / len(vec3_d(tree->max[0], tree->max[1], tree->max[2])- vec3_d(tree->min[0], tree->min[1], tree->min[2]));

	double r = 255 - (255 * factor);
	double g = 255 - (255 * factor);
	double b = 255 - (255 * factor);


	/*	double r = 255;
	double g = 0;
	double b = 0;*/
	texture_array[index] = (unsigned char)r;
	texture_array[index + 1] = (unsigned char)g;
	texture_array[index + 2] = (unsigned char)b;
	texture_array[index + 3] = (unsigned char)1;

}
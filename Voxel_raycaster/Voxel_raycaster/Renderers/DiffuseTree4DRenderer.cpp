#include "DiffuseTree4DRenderer.h"
#include "../misc_math.h"
#include <omp.h>
#include "../Tree4DTraverserDifferentSides.h"

#include "../util.h"
#include "../Globals.h"
using namespace std;

DiffuseTree4DRenderer::DiffuseTree4DRenderer(void) : Tree4DRenderer("diffuse")
{
}

void DiffuseTree4DRenderer::Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const {
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
#ifdef showDebugTemp
					tt_max = treeTraverser.getCurrentNodeInfo().max[3];
					tt_min = treeTraverser.getCurrentNodeInfo().min[3];
#endif


					calculateAndStoreColorForThisPixel(rc, tree, texture_array, index, treeTraverser);
					break;
				}
				treeTraverser.step();
			}
		}
	}
}

DiffuseTree4DRenderer::~DiffuseTree4DRenderer(void)
{
}


void DiffuseTree4DRenderer::calculateAndStoreColorForThisPixel(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, int index, Tree4DTraverserDifferentSides &treeTraverser) const
{
	double r = 0.0f;
	double g = 0.0f;
	double b = 0.0f;
	for (int i = 0; i< rc.lights.size(); i++) {
		if (rc.lights[i].active) {

//			std::cout << "light: " << i << " of " << rc.lights.size() -1 << " is active"<< endl;

			vec3_d to_light = rc.lights[i].position - treeTraverser.getCurrentPosition();
			vec3_d s = to_light + (rc.camera->eye - treeTraverser.getCurrentPosition());
			s = normalize(s);

			double distancecut = 1.0 / (
				rc.lights[i].CONSTANT_ATTENUATION +
				rc.lights[i].LINEAR_ATTENUATION*len(to_light) +
				rc.lights[i].QUADRATIC_ATTENUATION*len2(to_light));

			to_light = normalize(to_light);

			// Diffuse
			size_t data = treeTraverser.getCurrentNode()->data;
//			double diffuse_factor = tree->data[data].normal DOT to_light;
			double diffuse_factor = tree->data_ptrs[data]->normal DOT to_light;

//			std::cout << "diffuse factor: " << diffuse_factor << endl;
			r += std::max(0.0, diffuse_factor) * distancecut * rc.lights[i].diffuse[0];
			g += std::max(0.0, diffuse_factor) * distancecut * rc.lights[i].diffuse[1];
			b += std::max(0.0, diffuse_factor) * distancecut * rc.lights[i].diffuse[2];

			// Phong
//			double phong_factor = pow(to_light DOT tree->data[treeTraverser.getCurrentNode()->data].normal, rc.lights[i].SHININESS);
			double phong_factor = pow(to_light DOT tree->data_ptrs[treeTraverser.getCurrentNode()->data]->normal, rc.lights[i].SHININESS);
//			std::cout << "phong factor: " << phong_factor << endl;
			r += std::max(0.0, phong_factor) * distancecut * rc.lights[i].specular[0];
			g += std::max(0.0, phong_factor) * distancecut * rc.lights[i].specular[1];
			b += std::max(0.0, phong_factor) * distancecut * rc.lights[i].specular[2];

		}
	}
//	texture_array[index] = (unsigned char)clampf(255 * (tree->data[treeTraverser.getCurrentNode()->data].color[0] * (r + 0.25)), 0, 255);
//	texture_array[index + 1] = (unsigned char)clampf(255 * (tree->data[treeTraverser.getCurrentNode()->data].color[1] * (g + 0.25)), 0, 255);
//	texture_array[index + 2] = (unsigned char)clampf(255 * (tree->data[treeTraverser.getCurrentNode()->data].color[2] * (b + 0.25)), 0, 255);
	texture_array[index] = (unsigned char)clampf(255 * (tree->data_ptrs[treeTraverser.getCurrentNode()->data]->color[0] * (r + 0.25)), 0, 255);
	texture_array[index + 1] = (unsigned char)clampf(255 * (tree->data_ptrs[treeTraverser.getCurrentNode()->data]->color[1] * (g + 0.25)), 0, 255);
	texture_array[index + 2] = (unsigned char)clampf(255 * (tree->data_ptrs[treeTraverser.getCurrentNode()->data]->color[2] * (b + 0.25)), 0, 255);
	texture_array[index + 3] = (unsigned char)1;
}

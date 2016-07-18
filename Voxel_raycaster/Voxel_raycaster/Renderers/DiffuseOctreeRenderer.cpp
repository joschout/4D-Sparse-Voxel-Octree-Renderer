#include "DiffuseOctreeRenderer.h"
#include "../TreeTraverser.h"
#include "../misc_math.h"

using namespace std;

DiffuseOctreeRenderer::DiffuseOctreeRenderer(void) : OctreeRenderer("diffuse")
{
}

void DiffuseOctreeRenderer::Render(const RenderContext& rc, const Octree const* tree, unsigned char* texture_array) const{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index, partindex;
	TreeTraverser treeTraverser;


#pragma omp parallel for private(x,t,v,index,diffuse_factor,to_light,r,g,b)
	for(int y = 0; y < rc.n_y; y++){
		partindex = y*(rc.n_y*4);
		for(x = 0; x < rc.n_y; x++) {
			index = partindex + x*4; // index in char array computation (part 2)
			Ray ray3D = rc.getRayForPixel(x, y);
			treeTraverser = TreeTraverser(tree, ray3D); // DO A BASIC BOUNDING BOX TEST FOR THE TEST FIRST
			while((!treeTraverser.isTerminated())){
				if(treeTraverser.getCurrentNode()->isLeaf()
					&& treeTraverser.getCurrentNode()->hasData()) {//&& t.stack.back().t0.max() >0){
					
					calculateAndStoreColorForThisPixel(rc, tree, texture_array, index, treeTraverser);
					break;
				}
				treeTraverser.step();
			}
		}
	}
}

DiffuseOctreeRenderer::~DiffuseOctreeRenderer(void)
{
}

void DiffuseOctreeRenderer::calculateAndStoreColorForThisPixel(RenderContext const& rc, Octree const* tree, unsigned char* texture_array, int index, TreeTraverser &treeTraverser) const
{
	double r = 0.0;
	double g = 0.0;
	double b = 0.0;
	for (int i = 0; i<rc.lights.size(); i++) {
		if (rc.lights[i].active) {
			vec3_d to_light = rc.lights[i].position - treeTraverser.getCurrentPosition();
			vec3_d s = to_light + (rc.camera->eye - treeTraverser.getCurrentPosition());
			s = normalize(s);

			double distancecut = 1.0f / (
				rc.lights[i].CONSTANT_ATTENUATION +
				rc.lights[i].LINEAR_ATTENUATION*len(to_light) +
				rc.lights[i].QUADRATIC_ATTENUATION*len2(to_light));

			to_light = normalize(to_light);

			// Diffuse
			double diffuse_factor = tree->data[treeTraverser.getCurrentNode()->data].normal DOT to_light;
			r += std::max(0.0, diffuse_factor) * distancecut * rc.lights[i].diffuse[0];
			g += std::max(0.0, diffuse_factor) * distancecut * rc.lights[i].diffuse[1];
			b += std::max(0.0, diffuse_factor) * distancecut * rc.lights[i].diffuse[2];

			// Phong
			double phong_factor = pow(to_light DOT tree->data[treeTraverser.getCurrentNode()->data].normal, rc.lights[i].SHININESS);
			r += std::max(0.0, phong_factor) * distancecut * rc.lights[i].specular[0];
			g += std::max(0.0, phong_factor) * distancecut * rc.lights[i].specular[1];
			b += std::max(0.0, phong_factor) * distancecut * rc.lights[i].specular[2];

		}
	}
	texture_array[index] = (unsigned char)clampf(255 * (tree->data[treeTraverser.getCurrentNode()->data].color[0] * (r + 0.25)), 0, 255);
	texture_array[index + 1] = (unsigned char)clampf(255 * (tree->data[treeTraverser.getCurrentNode()->data].color[1] * (g + 0.25)), 0, 255);
	texture_array[index + 2] = (unsigned char)clampf(255 * (tree->data[treeTraverser.getCurrentNode()->data].color[2] * (b + 0.25)), 0, 255);
	texture_array[index + 3] = (unsigned char)1;
}
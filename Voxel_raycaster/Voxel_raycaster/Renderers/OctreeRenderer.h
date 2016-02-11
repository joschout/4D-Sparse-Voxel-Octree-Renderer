#ifndef OCTREERENDERER_H_
#define OCTREERENDERER_H_

#include <string>
#include <iostream>
#include <vector>
#include <omp.h>
#include "time.h"
#include <TriMesh.h>
#include "../RenderContext.h"
#include "../DataPoint.h"
#include "../intersection.h"
#include "../Octree.h"
#include "../Renderer.h"

class OctreeRenderer 
{
public:
	virtual ~OctreeRenderer()
	{
	}

	std::string name;
	virtual void Render(RenderContext const& rc,  Octree const* tree, unsigned char* texture_array) const = 0;

//protected constructors
protected:
	OctreeRenderer();
	OctreeRenderer(std::string name);
};

inline OctreeRenderer::OctreeRenderer() : name(""){
};

inline OctreeRenderer::OctreeRenderer(std::string name) : name(name){
};

#endif /* OCTREERENDERER_H_ */
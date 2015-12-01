#ifndef MULTIPLEOBJECTSTRACER_H
#define MULTIPLEOBJECTSTRACER_H
#include "Ray.h"

namespace trimesh{
	class Color;
}

class RenderContext;


class MultipleObjectsTracer
{
public:
	static trimesh::Color traceRay(RenderContext rc, Ray ray);
};

#endif

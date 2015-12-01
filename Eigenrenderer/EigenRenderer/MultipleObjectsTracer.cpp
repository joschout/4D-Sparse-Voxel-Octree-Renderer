#include "MultipleObjectsTracer.h"
#include "ShadeRec.h"
#include "RenderContext.h"

trimesh::Color MultipleObjectsTracer::traceRay(RenderContext rc, Ray ray)
{

	ShadeRec sr = rc.hitObjects(ray);

	if (sr.hasHitAnObject) {
		//sr.ray = ray;
		//return sr.material.shade(sr);
		return sr.color;
	}
	else {
		return rc.background_color;
	}
}

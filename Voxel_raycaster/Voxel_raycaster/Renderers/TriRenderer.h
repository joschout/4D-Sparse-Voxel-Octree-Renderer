#ifndef TRIRENDERER_H
#define TRIRENDERER_H

#include "../RenderContext.h"
#include "TriReader2.h"

class ShadeRec;
class RenderContext;

class TriRenderer
{
public:
	TriReader2 reader;
	void setTriReader(const std::string &filename, size_t n_triangles, size_t buffersize);
	TriRenderer(void);
	void Render(RenderContext const& rc, unsigned char* texture_array) ;
	~TriRenderer(void);

	static bool hitTriangle(const Triangle &triangle, const Ray& ray, ShadeRec& sr);
};
#endif
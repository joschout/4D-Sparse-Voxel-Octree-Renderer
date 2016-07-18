#ifndef TREE4DRENDERER_H_
#define TREE4DRENDERER_H_

#include <string>
#include "../RenderContext.h"
#include "../Tree4D.h"
#include "../Renderer.h"

class Tree4DRenderer
{
public:
	virtual ~Tree4DRenderer();

	std::string name;
	virtual void Render(RenderContext const& rc, Tree4D const* tree, unsigned char* texture_array, double time_point) const = 0;

	//protected constructors
protected:
	Tree4DRenderer();
	Tree4DRenderer(std::string name);
};

inline Tree4DRenderer::~Tree4DRenderer()
{
}

inline Tree4DRenderer::Tree4DRenderer() : name("") {
};

inline Tree4DRenderer::Tree4DRenderer(std::string name) : name(name) {
};

#endif /* TREE4DRENDERER_H_ */
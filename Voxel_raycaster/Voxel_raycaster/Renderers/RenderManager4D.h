#ifndef RENDERERMANAGER4D_H_
#define RENDERERMANAGER4D_H_
#include "Tree4DRenderer.h"
#include <vector>

class RendererManager4D
{
public:
	RendererManager4D();
	void addRenderer(Tree4DRenderer* r);
	Tree4DRenderer* getCurrentRenderer();
	Tree4DRenderer* getRenderer(std::string name);
	void switchRenderer();
	void setCurrentRenderer(std::string name);
	void deleteAllRenderers();

private:
	std::vector<Tree4DRenderer*> renderers;
	int current_r;
};

inline RendererManager4D::RendererManager4D() : current_r(0) {

}

inline Tree4DRenderer* RendererManager4D::getCurrentRenderer() {
	return renderers.at(current_r);
}

inline void RendererManager4D::switchRenderer() {
	current_r = (current_r + 1) % renderers.size();
}

inline void RendererManager4D::setCurrentRenderer(std::string name)
{
	for (std::vector<Tree4DRenderer*>::iterator it = renderers.begin(); it != renderers.end(); ++it) {
		if ((*it)->name == name) {
			current_r = it - renderers.begin();
		}
	}
}

inline void RendererManager4D::deleteAllRenderers()
{
	for (std::vector<Tree4DRenderer*>::iterator it = renderers.begin(); it != renderers.end(); ++it) {
		delete *it;
	}
}

inline void RendererManager4D::addRenderer(Tree4DRenderer* r) {
	renderers.push_back(r);
}

inline Tree4DRenderer* RendererManager4D::getRenderer(std::string name) {
	for (std::vector<Tree4DRenderer*>::iterator it = renderers.begin(); it != renderers.end(); ++it) {
		if ((*it)->name == name) {
			return *it;
		}
	}
	return nullptr;
}



#endif
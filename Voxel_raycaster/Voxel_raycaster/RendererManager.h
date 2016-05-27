#ifndef RENDERERMANAGER_H_
#define RENDERERMANAGER_H_
#include "Renderers/OctreeRenderer.h"
#include <vector>

class RendererManager
{
public:
	RendererManager();
	void addRenderer(OctreeRenderer* r);
	OctreeRenderer* getCurrentRenderer();
	OctreeRenderer* getRenderer(std::string name);
	void switchRenderer();

private:
	std::vector<OctreeRenderer*> renderers;
	int current_r;
};

inline RendererManager::RendererManager() : current_r(0){

}

inline OctreeRenderer* RendererManager::getCurrentRenderer(){
	return renderers.at(current_r);
}

inline void RendererManager::switchRenderer(){
	current_r = (current_r+1) % renderers.size();
}

inline void RendererManager::addRenderer(OctreeRenderer* r){
	renderers.push_back(r);
}

inline OctreeRenderer* RendererManager::getRenderer(std::string name){
	for(std::vector<OctreeRenderer*>::iterator it = renderers.begin() ; it != renderers.end(); ++it){
		if((*it)->name == name){
			return *it;
		}
	}
	return NULL;
}

#endif
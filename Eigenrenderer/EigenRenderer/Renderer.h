#ifndef RENDERER_H_
#define RENDERER_H_

#include <string>
#include "RenderContext.h"


class Renderer{
	public:
	virtual ~Renderer()
	{
	}

	std::string name;
	
	virtual void Render(RenderContext const& rc,  unsigned char* texture_array) const = 0;

		//protected constructors
	protected:
		Renderer();
		Renderer(std::string name);
};

inline Renderer::Renderer() : name("") {
};

inline Renderer::Renderer(std::string name) : name(name) {
};

#endif /* RENDERER_H_ */
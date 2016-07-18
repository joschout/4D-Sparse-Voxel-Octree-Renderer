#ifndef LIGHT_H_
#define LIGHT_H_

#include "util.h"

using namespace trimesh;

class Light {
public:

	bool active;
	vec3_d position;
	vec3_d diffuse;
	vec3_d specular;
	
	// light constants
	double CONSTANT_ATTENUATION;
	double LINEAR_ATTENUATION;
	double QUADRATIC_ATTENUATION;

	// other light constants
	double SHININESS;

	Light(vec3_d position,vec3_d diffuse, vec3_d specular);
	virtual ~Light();

private:
	void initLight();
};

inline Light::Light(vec3_d position = vec3_d(0, 0, 0),vec3_d diffuse =vec3_d(1.0, 1.0, 1.0 ), vec3_d specular = vec3_d (1.0, 1.0, 1.0))
	: position(position), diffuse(diffuse), specular(specular) {
		initLight();
}

inline Light::~Light() {
	// TODO Auto-generated destructor stub
}

inline void Light::initLight(){
	active = true;
	this->CONSTANT_ATTENUATION = 1.0;
	this->LINEAR_ATTENUATION = 0.0;
	this->QUADRATIC_ATTENUATION = 0.0;
	this->SHININESS = 50.0;
}


#endif /* LIGHTOCTREERENDERER_H_ */
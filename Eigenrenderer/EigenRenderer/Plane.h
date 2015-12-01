#ifndef __PLANE__
#define __PLANE__

#include "GeometricObject.h"
#include <TriMesh.h>

using trimesh::vec3;
//-------------------------------------------------------------------- class Plane

class Plane : public GeometricObject {

public:

	Plane(void);   												// default constructor

	Plane(const trimesh::vec3 &point, const trimesh::vec3& normal);			// constructor	

	Plane(const Plane& plane); 									// copy constructor

	virtual Plane* 												// virtual copy constructor
		clone(void) const;

	Plane& 														// assignment operator
		operator= (const Plane& rhs);

	virtual														// destructor
		~Plane(void);

	virtual bool
		hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

private:

	vec3 	point;   				// point through which plane passes 
	vec3 	normal;					// normal to the plane

	static const double kEpsilon;   // for shadows and secondary rays
};

#endif

#ifndef RENDERCONTEXT_H_
#define RENDERCONTEXT_H_

#include "Frustrum.h"
#include "Camera.h"
#include "Ray.h"
#include "Light.h"
#include "Intersectable.h"
#include "MultipleObjectsTracer.h"
#include "Constants.h"

/*
	A raster image is indexed by the pair (i,j)
	indicating the column i and the row j of the pixel,
	counting from the bottom-left pixel.
	If an image has nx columns and ny rows of pixels,
	the bottom-left pixel is (0, 0)
	and the topright is pixel (nx ? 1, ny ? 1).
*/

class RenderContext {
public:
	// class attributes
	const Camera*  camera;
	const Frustrum*  frustrum;
	Color background_color;
	std::vector<Light> lights;
	std::vector<Intersectable*> intersectables;
	MultipleObjectsTracer* tracer;

	//image width: the image has n_x pixels in the horizontal direction
	int n_x;
	//image height: the imagge has n_y pixels in the vertical direction
	int n_y;

	// constuctors en destructors
	RenderContext();
	RenderContext(Camera const* c, Frustrum const* frustrum, int resolution_x, int resolution_y);
	virtual ~RenderContext();

	Ray getRayForPixel(int i, int j) const;
	Vector3D getPixelCoordinate(int i, int j) const;

	ShadeRec hitObjects(Ray ray);
};

inline RenderContext::RenderContext() : camera(nullptr), frustrum(nullptr), tracer(nullptr), n_x(0), n_y(0) {
}

/*
*/
inline RenderContext::RenderContext(Camera const* c, Frustrum const* f, int resolution_x, int resolution_y)
	: camera(c), frustrum(f), tracer(nullptr), n_x(resolution_x), n_y(resolution_y) {
}

/*
 Get the screenpoint s of the pixel on the frustrum (in world coordinates)\
 This is what a projection matrix normally does
 */
 inline Vector3D RenderContext::getPixelCoordinate(int i, int j) const {

	 //see also Fundametals of CG 3rd ed., page 75
	// compute u,v,w: the coordinates of the screenpoint s in the coordinates of the camera's basis
	float u_s = frustrum->left + (frustrum->right - frustrum->left)*((i + 0.5f) / n_x);
	float v_s = frustrum->bottom + (frustrum->top - frustrum->bottom)*((j + 0.5f) / n_y);
	float w_s = frustrum->neir;

	// get screenpoint s in world coordinates
	return camera->eye + ((u_s*camera->basis.u) + (v_s*camera->basis.v) + (w_s*camera->basis.w));
}

inline RenderContext::~RenderContext() {
	// TODO Auto-generated destructor stub
}

/**
* Returns the ray from the Rendercontext's camera to the given pixel
* @param int i 
*/
// Get the ray from the Rendercontext's camera to the given pixel
inline Ray RenderContext::getRayForPixel(int i, int j) const {
	//get the world coordinates of the point on the screen
	Vector3D s = this->getPixelCoordinate(i, j);
	s = s - camera->eye;
	s.normalize();
	//return Ray(camera->eye.toPoint3D(), (s - camera->eye));
	return Ray(camera->eye.toPoint3D(), s);
}

inline ShadeRec RenderContext::hitObjects(Ray ray) {

	//hb p 262
	ShadeRec sr = ShadeRec();

	double tmin = DOUBLE_MAX_VALUE;
	Normal normal = Normal();
	Point3D localHitPoint = Point3D();
	//Shape hittedShape = null;

	for (std::vector<Intersectable*>::iterator it = intersectables.begin(); it != intersectables.end(); ++it) {
		if ((*it)->intersect(ray, sr)) {
			if (sr.t < tmin) {
				//hittedShape = shape;
				sr.hasHitAnObject = true;
				//sr.ray = ray;
				sr.color = (*it)->color;
				/*if (intersectable.isInfinite()) {
					sr.material = ((Shape)intersectable).getMaterial();
				}*/

				sr.hitPoint = ray.origin + ray.direction *sr.t;

				//deze drie worden door de intersect functie van een shape aangepast
				// en moeten we tijdelijk lokaal opslaan
				tmin = sr.t;
				normal = sr.normal;
				localHitPoint = sr.localHitPoint;
			}
		}
	}
	if (sr.hasHitAnObject) {
		sr.t = tmin;
		sr.normal = normal;
		sr.localHitPoint = localHitPoint;
	}
	return sr;
}


#endif /* RENDERCONTEXT_H_ */


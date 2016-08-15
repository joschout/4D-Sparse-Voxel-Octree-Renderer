#ifndef RENDERCONTEXT_H_
#define RENDERCONTEXT_H_

#include "Frustrum.h"
#include "Camera.h"
#include "Ray.h"
#include "Light.h"


/*
A raster image is indexed by the pair (i,j)
indicating the column i and the row j of the pixel,
counting from the bottom-left pixel.
If an image has nx columns and ny rows of pixels,
the bottom-left pixel is (0, 0)
and the topright is pixel (nx - 1, ny - 1).
*/

class RenderContext {
public:
	// class attributes
	const Camera*  camera;
	const Frustrum*  frustrum;
	//Color background_color;
	std::vector<Light> lights;


	//image width: the image has n_x pixels in the horizontal direction
	int n_x;
	//image height: the imagge has n_y pixels in the vertical direction
	int n_y;

	// constuctors en destructors
	RenderContext();
	RenderContext(Camera const* c, Frustrum const* frustrum, int resolution_x, int resolution_y);
	virtual ~RenderContext();

	Ray getRayForPixel(int i, int j) const;
	Ray getRayForPixel_rayparam(int i, int j, double& t_pixel) const;
	vec3_d getPixelCoordinate(int i, int j) const;

	std::vector<Ray> getPixelCornerRays(int i, int j) const;

	double getRayParameterForPixel(int i, int j) const;

};

inline RenderContext::RenderContext() : camera(nullptr), frustrum(nullptr), n_x(0), n_y(0) {
}

/*
*/
inline RenderContext::RenderContext(Camera const* c, Frustrum const* f, int resolution_x, int resolution_y)
	: camera(c), frustrum(f), n_x(resolution_x), n_y(resolution_y) {
}

/*
Get the screenpoint s of the pixel on the frustrum (in world coordinates)\
This is what a projection matrix normally does
*/
inline vec3_d RenderContext::getPixelCoordinate(int i, int j) const {

	//see also Fundametals of CG 3rd ed., page 75
	// compute u,v,w: the coordinates of the screenpoint s in the coordinates of the camera's basis
	double u_s = frustrum->left + (frustrum->right - frustrum->left)*((i + 0.5f) / n_x);
	double v_s = frustrum->bottom + (frustrum->top - frustrum->bottom)*((j + 0.5f) / n_y);
	double w_s = frustrum->near;

	// get screenpoint s in world coordinates
	return camera->eye 
		+ ((u_s*camera->getU()) 
		+ (v_s*camera->getV()) 
		+ (w_s*camera->getW())); // W points away from the image plane
}

inline std::vector<Ray> RenderContext::getPixelCornerRays(int i, int j) const
{
	std::vector<Ray> ray_vector;


	double frustrum_width = frustrum->right - frustrum->left;
	double frustrum_height = frustrum->top - frustrum->bottom;


//	vec3_d previous_screenpoint_xys;
	for (size_t horizontal = 0; horizontal < 2; horizontal++)
	{
		for (size_t vertical = 0; vertical < 2; vertical++)
		{
			double offset_from_the_left = frustrum_width * ((i + vertical * 1.0f) / n_x);
			double u_screenpoint = frustrum->left + offset_from_the_left;
			double offset_from_the_bottom = frustrum_height * ((j + horizontal * 1.0f) / n_y);
			double v_screenpoint = frustrum->bottom + offset_from_the_bottom;
			double w_screenpoint = frustrum->near;

			vec3_d screenpoint_uvw = u_screenpoint*camera->getU() + v_screenpoint*camera->getV() + w_screenpoint*camera->getW();
			// get screenpoint s in world coordinates
			vec3_d screenpoint_xys = camera->eye + screenpoint_uvw;

//			if(screenpoint_xys == previous_screenpoint_xys)
//			{
//				std::cout << "something went wrong" << std::endl;
//
//			}
//			previous_screenpoint_xys = screenpoint_xys;

			vec3_d direction = screenpoint_xys - camera->eye;

//			if(isnan(direction[0]) || isnan(direction[1]) || isnan(direction[2]))
//			{
//				std::cout << "nanananana batman!" << std::endl;
//			}


			direction = normalize(direction);
			ray_vector.push_back(Ray(camera->eye, direction));
		}
	}

	return ray_vector;
}

inline double RenderContext::getRayParameterForPixel(int i, int j) const
{
	vec3_d vector_to_current_pixel = this->getPixelCoordinate(i, j) - this->camera->eye;
	double t_pixel = abs(len(vector_to_current_pixel));

	return t_pixel;
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
	vec3_d s = this->getPixelCoordinate(i, j);
	s = s - camera->eye;
	s = normalize(s);
	return Ray(camera->eye, s);
}

inline Ray RenderContext::getRayForPixel_rayparam(int i, int j, double& t_pixel) const {
	//get the world coordinates of the point on the screen
	vec3_d s = this->getPixelCoordinate(i, j);
	s = s - camera->eye;
	t_pixel = abs(len(s));
	s = normalize(s);
	return Ray(camera->eye, s);
}


#endif /* RENDERCONTEXT_H_ */


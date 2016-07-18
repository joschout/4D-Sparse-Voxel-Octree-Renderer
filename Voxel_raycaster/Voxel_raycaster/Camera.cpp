#include "Camera.h"

Camera::Camera(): eye(vec3_d(0, 0, 0)), gaze(vec3_d(0, 0, -1)), top(vec3_d(0, 1, 0))
{
	this->computeUVW();
}

Camera::Camera(vec3_d eye, vec3_d gaze, vec3_d top): eye(eye), gaze(gaze), top(top)
{
	std::cout << "constructing new camera" << std::endl;
	this->computeUVW();
}

void Camera::computeUVW()
{
	vec3_d w_ = normalize(gaze) * -1.0;
	vec3_d u_ = normalize(vec3_d(top CROSS w_));
	vec3_d v_ = (w_ CROSS u_);
	w = w_; u = u_; v = v_;
}

vec3_d Camera::getU() const
{
	return u;
}

vec3_d Camera::getV() const
{
	return v;
}

vec3_d Camera::getW() const
{
	return w;
}

vec3_d Camera::getGaze() const
{
	return gaze;
}

void Camera::setGaze(vec3_d gaze)
{
//	std::cout << "setting gaze vector:" << std::endl;
//	std::cout << "   old gaze:  "<< this->gaze << std::endl;

	this->gaze = gaze;

//	std::cout << "   new gaze:  " << this->gaze << std::endl;
//	std::cout << "------" << std::endl;
}

vec3_d Camera::getTop() const
{
	return top;
}

void Camera::setTop(vec3_d top)
{
	this->top = top;
}

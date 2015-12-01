#ifndef FRUSTRUM_H_
#define FRUSTRUM_H_
#include <cmath>
/*
The view frustrum defines the region of space that gets projected onto the screen

It also defines our image dimensions in coordinates relative to the camera's basis.
	left = the position of the left edge of the image (u-direction)
	right = the position of the right edge of the image (u-direction)


	bottom = the bottom edge of the image (v-direction)
	top = the top edge of the image (v-direction)

	neir = the position of the screen (w-direction)
	far = how far we can see (w-direction)

	usually: left < 0 , right
			bottom < 0 < top
*/
class Frustrum {
public:
	float left, right, bottom, top, neir, far;
	Frustrum()
		: left(-1), right(1), bottom(-1), top(1), neir(1), far(100) {
	}
	Frustrum(float left, float right, float bottom, float top, float neir, float far)
		: left(left), right(right), bottom(bottom), top(top), neir(neir), far(far) {
	}
	Frustrum(float fov, float aspectratio, float front, float back);
	//virtual ~Frustrum();
};

inline Frustrum::Frustrum(float fov, float aspectratio, float front, float back) {

	// using fov, determine height and width
	const float degree2radian = 3.14159265358979323846 / 180.0;
	float tang = tan(fov*degree2radian);
	float height = front * tang;
	float width = height*aspectratio;

	// put it in general frustrum coordinates
	left = -width;
	right = width;

	bottom = -height;
	top = height;

	neir = front;
	far = back;
}
#endif





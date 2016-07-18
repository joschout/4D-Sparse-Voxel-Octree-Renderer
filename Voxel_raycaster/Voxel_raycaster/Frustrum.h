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

near = the position of the screen (w-direction)
far = how far we can see (w-direction)

usually: left < 0 , right
bottom < 0 < top
*/
class Frustrum {
public:
	double left, right, bottom, top, near, far;
	Frustrum()
		: left(-1), right(1), bottom(-1), top(1), near(1), far(100) {
	}
	Frustrum(double left, double right, double bottom, double top, double near, double far)
		: left(left), right(right), bottom(bottom), top(top), near(near), far(far) {
	}
	Frustrum(double fov, double aspectratio, double front, double back);
	//virtual ~Frustrum();
};

inline Frustrum::Frustrum(double fov, double aspectratio, double front, double back) {

	// using fov, determine height and width
	const double degree2radian = 3.14159265358979323846 / 180.0;
	double tang = tan(fov*degree2radian);
	double height = front * tang;
	double width = height*aspectratio;

	// put it in general frustrum coordinates
	left = -width;
	right = width;

	bottom = -height;
	top = height;

	near = front;
	far = back;
}
#endif
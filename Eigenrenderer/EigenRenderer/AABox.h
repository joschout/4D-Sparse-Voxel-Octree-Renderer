#ifndef BOX_H_
#define BOX_H_

#include "Vector3D.h"


class AABox {
public:
	//variables
	Vector3D minPoint;
	Vector3D maxPoint;

	// functions
	AABox();
	AABox(Vector3D min, Vector3D max);
	virtual ~AABox();
};

inline AABox::AABox() : minPoint(Vector3D(0, 0, 0)), maxPoint(Vector3D(0.0, 0.0, 0.0)) {
	// TODO Auto-generated constructor stub
}

inline AABox::AABox(Vector3D min, Vector3D max) : minPoint(min), maxPoint(max) {
	// TODO Gooi exception wanneer verkeerd
	if (min.length() > max.length()) {
		this->maxPoint = min;
		this->minPoint = max;
	}
}

inline AABox::~AABox() {
	// TODO Auto-generated destructor stub
}

#endif /* BOX_H_ */

/*
Camera klasse, gebaseerd op mijn java-camera
*/

#include "Vector3D.h"


class OrthonormalBasis {
public:
	Vector3D u;
	Vector3D v;
	Vector3D w;

	/*
	* Creates an orthonormal basis from the two given vec3s.
	* The w vector  of this OrthonormalBasis basis will point in the same direction as the given vec3 a.
	* The constructor tries to force the v vector of this orthonormal basis to point roughly in the
	* same direction as the given b vector.

	@param a
	The first vector. The w vector of this orthonormal basis will point in the same
	direction as the given vector a.
	@param b
	The second vector. The v vector will point roughly in the same direction as the given b vector.
	*/
	OrthonormalBasis() {
		u = Vector3D(1, 0, 0);
		v = Vector3D(0, 1, 0);
		w = Vector3D(0, 0, 1);
	}

	OrthonormalBasis(Vector3D u, Vector3D v, Vector3D w) : u(u), v(v), w(w) {}

	OrthonormalBasis(Vector3D a, Vector3D b) {
		w = a.hat() * -1.0;
		u = (b ^ w).hat();
		v = (w ^ u);
	}

};

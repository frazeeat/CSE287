#include "Plane.h"

/**
* Constructor for the Plane.
*/
Plane::Plane( const vec3 & point, const vec3 & normal, const color & material )
	: Surface(material), a(point), n(normal)
{
}


Plane::~Plane(void)
{
}

/*
* Checks a ray for intersection with the surface. Finds the closest point of intersection
* if one exits. Returns a HitRecord with the t parmeter set to FLT_MAX if there is no
* intersection.
*/
HitRecord Plane::findClosestIntersection(const vec3 &rayOrigin, const vec3 &rayDirection)
{

	HitRecord hitRecord;

	float denominator = dot(rayDirection, n);

	if (denominator < 0) {

		float t = dot(a - rayOrigin, n) / denominator;

		// Set hit record information about the intersetion.
		hitRecord.interceptPoint = rayOrigin + t * rayDirection;

		hitRecord.surfaceNormal = n;

		hitRecord.material = material;
		hitRecord.t = t;

	}
	else {

		// Set parameter, t, in the hit record to indicate "no intersection."
		hitRecord.t = FLT_MAX;

	}

	return hitRecord;

} // end checkIntercepts


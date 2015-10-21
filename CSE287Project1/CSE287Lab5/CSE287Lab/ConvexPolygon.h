#pragma once

#include "BasicIncludesAndDefines.h"
#include "Plane.h"

/**
* Super class for all implicitly described surfaces in a scene. Support intersection testing
* with rays.
*/
class ConvexPolygon : public Plane
{
public:


	ConvexPolygon(const vector<vec3> & vec, const color & material);
	
	~ConvexPolygon(void);


	virtual HitRecord findClosestIntersection(const vec3 &rayOrigin, const vec3 &rayDirection);

	vector<vec3> vec;

};

#include "Elipsoid.h"

Elipsoid::Elipsoid(const vec3 & position, const color & material, const float x, const float y, const float z)
	:QuadricSurface(position,material){
	A = x;
	B = y;
	C = z;
}
Elipsoid::~Elipsoid(void)
{
}

HitRecord Elipsoid::findClosestIntersection(const vec3 &rayOrigin, const vec3 &rayDirection){
	HitRecord hit = QuadricSurface::findClosestIntersection(rayOrigin, rayDirection);
	return hit;
}
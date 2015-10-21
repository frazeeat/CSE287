#include "BasicIncludesAndDefines.h"
#include "QuadricSurface.h"

class Elipsoid : public QuadricSurface{
public:

	Elipsoid::Elipsoid(const vec3 & position, const color & material, const float x, const float y, const float z);
	virtual HitRecord findClosestIntersection(const vec3 &rayOrigin, const vec3 &rayDirection);
	vec3 postiion;
	~Elipsoid(void);
};
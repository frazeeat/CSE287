#pragma once
#include "surface.h"

/**
* Sub-class of Surface that represents inplicit description of a plane.
*/
class Plane : public Surface
{
	public:

	/**
	* Constructor for the plane. 
	* @param - point: specifies an xyz position that is on the plane
	* @param - normal: unit Vector that is perpendicular to the front face of the plane
	* @param - material: color of the plane.
	*/
	Plane( const vec3 & point, const vec3 & normal, const color & material);
	~Plane(void);

	/**
	* Checks a ray for intersection with the surface. Finds the closest point of intersection
	* if one exits. Returns a HitRecord with the t parmeter set to FLT_MAX if there is no 
	* intersection.
	* @param rayOrigin - Origin of the ray being check for intersetion
	* @param rayDirection - Unit vector represention the direction of the ray.
	* returns HitRecord containing intormation about the point of intersection.
	*/
	virtual HitRecord findClosestIntersection (const vec3 &rayOrigin, const vec3 &rayDirection);

	/** Point on the plane */
	vec3 a;

	/** Unit Vector that is perpendicular to the front face of the plane 
	* (surface normal */
	vec3 n;
};


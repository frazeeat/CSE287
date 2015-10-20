#include "Sphere.h"


Sphere::Sphere(const vec3 & position, float radius, const color & material)
	: Surface(material), center(position), radius(radius)
{
}


Sphere::~Sphere(void)
{
}

/*
* Checks a ray for intersection with the surface. Finds the closest point of intersection
* if one exits. Returns a HitRecord with the t parmeter set to FLT_MAX if there is no
* intersection.
*/
HitRecord Sphere::findClosestIntersection(const vec3 &rayOrigin, const vec3 &rayDirection)
{
	HitRecord hitRecord;

	// Page 77
	float discriminant = pow(dot(rayDirection, rayOrigin - center), 2) - dot(rayDirection, rayDirection)*(dot(rayOrigin - center, rayOrigin - center) - radius * radius);

	if (discriminant >= 0) {

		float t = FLT_MAX;

		if (discriminant > 0) {

			// Two intercepts. Find and return the closest one.
			float t1 = (dot(-rayDirection, rayOrigin - center) - sqrt(discriminant)) / dot(rayDirection, rayDirection);

			if (t1 > 0) {

				t = t1;
			}
			else {

				t = (dot(-rayDirection, rayOrigin - center) + sqrt(discriminant)) / dot(rayDirection, rayDirection);
			}
		}
		else {

			// One Intercept. Find and return the t for the single point of intersection.
			t = dot(-rayDirection, rayOrigin - center) / dot(rayDirection, rayDirection);
		}

		// Set hit record information about the intersetion.
		hitRecord.t = t;
		hitRecord.interceptPoint = rayOrigin + t * rayDirection;
		hitRecord.surfaceNormal = normalize(hitRecord.interceptPoint - center);
		hitRecord.material = material;
	}
	else {
		// Set parameter, t, in the hit record to indicate "no intersection."
		hitRecord.t = FLT_MAX;
	}

	return hitRecord;

} // end checkIntercept
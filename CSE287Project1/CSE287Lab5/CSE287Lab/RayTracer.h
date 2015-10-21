#pragma once

#include "BasicIncludesAndDefines.h"

#include "Surface.h"
#include "Sphere.h"
#include "Plane.h"
#include "ConvexPolygon.h"
#include "Elipsoid.h"

#include "Lights.h"

/**
* Class that supports simple ray tracing of a scene containing a number of object 
* (surfaces) and light sources.
*/
class RayTracer
{
	public:

	/**
	* Constructor.
	* @param color buffer to which the ray tracer will be render.
	* @param default color to which pixel will be set when there it no intersection
	* for a ray associated with a particular pixel.
	*/
	RayTracer(ColorBuffer & cBuffer, color defaultColor = color(0.0f, 0.0f, 0.0f, 1.0));

	~RayTracer(void);

	/**
	* Ray traces a scene containing a number of surfaces and light sources. Sets every
	* pixel in the rendering window. Pixels that are not associated with a ray/surface
	* intersection are set to a default color.
	* @param surfaces - list of the surfaces in the scene
	* @param lights - list of the light sources in the scene
	*/
	void raytraceScene(vector<Surface*> surfaces, vector<LightSource*> lights);

	/**
	* Sets the w, u, and v orthonormal basis vectors associated with the coordinate
	* frame that is tied to the viewing position and the eye data member of the
	* class.
	* @param viewPosition - xyz position of the view point
	* @param viewingDirection - vector that points in the viewing direction
	* @param up - approximation of the up vector (cannot be parallel to viewing direction)
	*/
	void setCameraFrame(vec3 viewPosition, vec3 viewingDirection, vec3 up);

	/**
	* Set the following members of the class: topLimit, bottomLimit, rightLimit,
	* leftLimit, distToPlane, nx, and ny based the distance form the middle of
	* the projection plane to the top of the projection plane and width of the
	* rendering window.
	* @param viewPlaneHeight - distance to the top of the projection plane
	*/
	void calculateOrthographicViewingParameters(float viewPlaneHeight = 10.0f);

	/**
	* Set the following members of the class: topLimit, bottomLimit, rightLimit,
	* leftLimit, distToPlane, nx, and ny based of the specified vertical field of
	* view and height and width of the rendering window.
	* @param verticalFieldOfViewDegrees - vertical field of view in degrees
	*/
	void calculatePerspectiveViewingParameters(float verticalFieldOfViewDegrees = 45.0f);

protected:

	/**
	* Checks the ray for intersection against every Surface in the scene. It does
	* this by repeatedly passing the ray origin (e) and the ray direction (d) to
	* findClosestIntersection methods of Surface class and its sub-classes. Once
	* the closest point of intersection is found an color is returned based on
	* calculated interactions between the intersected surface and the light
	* sources in the scene.
	*
	* Can be called recursively to trace rays associated with reflection and
	* refraction.
	* @param e - origin of the ray being traced
	* @param d - unit length vector representing the direction of the ray
	* @returns color for the point of intersection
	*/
	color traceIndividualRay(const vec3 &e, const vec3 &d,int rec);

	/**
	* Sets the rayOrigin and rayDirection data members of the class based on row and
	* column of a specified pixel.These two data members together represent a
	* parametric description of a ray to be checked for intersection with objects
	* in the scene. The ray is caluclated for a orthographic projection.
	* @param x column of a pixel in the rendering window
	* @param y row of a pixel in the rendering window
	*/
	void setOrthoRayOriginAndDirection( int x, int y);

	/**
	* Sets the rayOrigin and rayDirection data members of the class based on row and 
	* column of a specified pixel.These two data members together represent a 
	* parametric description of a ray to be checked for intersection with objects
	* in the scene. The ray is caluclated for a perspective projection.
	* @param x column of a pixel in the rendering window
	* @param y row of a pixel in the rendering window
	*/void setPerspectiveRayOriginAndDirection( int x, int y);

	/**
	* Finds the projection plane coordinates, u and v, for the pixel identified
	* by the input arguments.
	* @param x column of a pixel in the rendering window
	* @param y row of a pixel in the rendering window
	* @returns two dimensional vector containing the projection plane coordinates
	*/
	vec2 getProjectionPlaneCoordinates(int x, int y);

	// Alias for an object controls memory that stores a rgba color value f
	// or every pixel.
	ColorBuffer & colorBuffer;

	// Color to which a pixel is set if there is no intersection
	// for the traced pixel ray.
	color defaultColor;

	// View frame parameters
	vec3 eye; // position of the viewpoint
	vec3 u; // "right" relative to the viewing direction
	vec3 v; // perpendicular up vector
	vec3 w; // camera looks in the negative w direction

	// Projection plane parameters
	// Measured relative to u (right)
	float rightLimit;
	float leftLimit;
	// Measured reltive to v (up)
	float topLimit;
	float bottomLimit;

	// Rendering window parameters
	float nx; // Width in pixels
	float ny; // Height in pixel

	// Distance from the viewpoint to the projection plane
	float distToPlane;

	// Parametric ray being traced for a pixel
	vec3 rayDirection; // Direction of the ray
	vec3 rayOrigin; // Origin of the ray

	// List of the surfaces in the scene that is being ray traced
	vector<Surface*> surfacesInScene;

	// List of the light sources in the scene that is being ray traced
	vector<LightSource*> lightsInScene;

};


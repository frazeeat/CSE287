#include "RayTracer.h"


RayTracer::RayTracer(ColorBuffer & cBuffer, color defaultColor )
	:colorBuffer(cBuffer), defaultColor(defaultColor)
{
	
}


RayTracer::~RayTracer(void)
{
}

void RayTracer::setCameraFrame(vec3 viewPosition, vec3 viewingDirection, vec3 up)
{
	// TODOone
	

	//Squaring up a basis
	w = normalize(-viewingDirection);
	u = normalize(cross(up, w));
	v = normalize(cross(w, u));
	eye = viewPosition;

} // end setCameraFrame


void RayTracer::calculatePerspectiveViewingParameters(float verticalFieldOfViewDegrees)
{


	distToPlane = 1.0f / tan(radians(verticalFieldOfViewDegrees) / 2.0f);
	topLimit = distToPlane * tan(radians(verticalFieldOfViewDegrees) / 2.0f);
	rightLimit = topLimit * ((float)colorBuffer.getWindowWidth() / colorBuffer.getWindowHeight()); // Set r based on aspect ratio and height of plane
	leftLimit = -1*rightLimit;
	bottomLimit = -1*topLimit;

	nx = (float)colorBuffer.getWindowWidth();
	ny = (float)colorBuffer.getWindowHeight();
	
	
} // end calculatePerspectiveViewingParameters


void RayTracer::calculateOrthographicViewingParameters(float viewPlaneHeight)
{
	topLimit = fabs(viewPlaneHeight) / 2.0f;

	rightLimit = topLimit * ((float)colorBuffer.getWindowWidth()/colorBuffer.getWindowHeight()); // Set r based on aspect ratio and height of plane

	// Make view plane symetrical about the viewing direction
	leftLimit = -rightLimit; 
	bottomLimit = -topLimit;

	// Calculate the distance between pixels in the horizontal and vertical directions
	nx = (float)colorBuffer.getWindowWidth();
	ny = (float)colorBuffer.getWindowHeight();

	distToPlane = 0.0; // Rays start on the view plane
	
} // end calculateOrthographicViewingParameters


void RayTracer::raytraceScene(vector<Surface*> surfaces, vector<LightSource*> lights)
{
	this->surfacesInScene = surfaces;
	this->lightsInScene = lights; 

	calculatePerspectiveViewingParameters();

	// TODO
	for (int i = 0; i < colorBuffer.getWindowHeight(); i++){
		for (int j = 0; j < colorBuffer.getWindowWidth(); j++){
			setPerspectiveRayOriginAndDirection(j, i);
			colorBuffer.setPixel(j, i, traceIndividualRay(rayOrigin, rayDirection));
		}
	}

} // end raytraceScene


color RayTracer::traceIndividualRay(const vec3 &e, const vec3 &d)
{
	HitRecord hr, hr_temp;
	hr_temp.t=FLT_MAX;
	hr_temp.material = defaultColor;
	color totalLight;

	for (int i = 0; i < (int)surfacesInScene.size(); i++){
		hr = surfacesInScene[i]->findClosestIntersection(e, d);
		if (hr_temp.t > hr.t){
			hr_temp = hr;
		}		
	}
	for (int j = 0; j < (int)lightsInScene.size(); j++) {
		totalLight += lightsInScene[j]->illuminate(-d, hr_temp);
	}
	return hr_temp.material+totalLight;
	// TODO
	
	


} // end traceRay


void RayTracer::setOrthoRayOriginAndDirection( int x, int y)
{
	vec2 uv = getProjectionPlaneCoordinates(x, y);

	// Page 75
	rayDirection = normalize(-w);
	rayOrigin = eye + uv.x * u + uv.y * v;

} // end setOrthoOriginAndDirection


void RayTracer::setPerspectiveRayOriginAndDirection(int x, int y)
{
	// TODOne
	vec2 uv = getProjectionPlaneCoordinates(x, y);

	rayOrigin = eye;
	rayDirection = normalize((-distToPlane*w) + (uv.x *u) + (uv.y*v));


} // end setPerspectiveOriginAndDirection


vec2 RayTracer::getProjectionPlaneCoordinates(int x, int y)
{
	// Page 75
	

	// TODOne

	float u =leftLimit + (rightLimit - leftLimit)*((x + 0.5) / (float)colorBuffer.getWindowWidth());
	float v = bottomLimit + (topLimit - bottomLimit)*((y + 0.5) / (float)colorBuffer.getWindowHeight());
	vec2 s(u,v);
	return s;
}




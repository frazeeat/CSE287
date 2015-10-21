#pragma once
#define EPSILON 1.0E-2f

#include "BasicIncludesAndDefines.h"

/**
* Base struct for all types of lights. Supports only specification of the 
* color and intensity of light sources. Parent of sub-structs that simulate
* positional, directional, and spot lights.
*
* Instances of this struct an be used to simulate only ambient light. Ambient
* light is described as bounced light that has been scattered so much that it
* is impossible to tell the direction to its source. If using a LightSource 
* instantiation to simulate ambient light, the overall intensity of the light
* should be low in order to avoid washing out shadows as well as diffuse and 
* specular effects that can be achieved when using children of this struct.
*/
struct LightSource
{
	LightSource(const color & color) 
		: lightColor(color) 
	{}

	virtual color illuminate(const vec3 & eyeVector, HitRecord & closestHit, vector<Surface*> surfaces)
	{
		color return_color;
		return_color.r = closestHit.material.r *lightColor.r;
		return_color.g = closestHit.material.g *lightColor.g;
		return_color.b = closestHit.material.b *lightColor.b;
		return_color.a = closestHit.material.a *lightColor.a;
		return return_color;
	}

	/* 
	* Color and intensity of the light.
	*/
	color lightColor; 
};

/**
* Struct for simulating light sources that have an explicit position and 
* shine equally in all directions. Instantiations of the struct will have
* a position property and a color and intensity of the light given off
* by the light.
*/
struct PositionalLight : public LightSource
{
	PositionalLight(vec3 position, const color & color)
		: LightSource(color), position(position)
	{}

	virtual color illuminate(const vec3 & eyeVector, HitRecord & closestHit, vector<Surface*> surfaces)
	{
		int shiney = 255;
		color white = color(1.0f, 1.0f, 1.0f, 1.0f);
		vec3 n = closestHit.surfaceNormal;
		vec3 v = normalize(eyeVector - closestHit.interceptPoint);
		vec3 l = normalize(position - closestHit.interceptPoint);
		vec3 h = normalize(v + l);

		
		HitRecord hr_temp;
		hr_temp.t = FLT_MAX;
		vec3 point = closestHit.interceptPoint + (EPSILON * closestHit.surfaceNormal);
		for (int i = 0; i < surfaces.size(); i++){
			HitRecord hr = surfaces[i]->findClosestIntersection(point, -l);
			if (hr.t < hr_temp.t)
			{
				hr_temp = hr;
			}
		}

		float toLight = distance(point, position);
		float toCept = distance(point, hr_temp.interceptPoint);
		if (hr_temp.t < FLT_MAX  && hr_temp.t < 0 && toLight > toCept) {
			return color(0.0f, 0.0f, 0.0f, 1.0f);
		}
		
		color diffuse = max(0.0f, dot(n, l))*lightColor*closestHit.material;
		color specular = pow(max(0.0f, dot(n, h)), shiney)*white*white;
		return diffuse + specular;

	}
	float distance(vec3 a, vec3 b)
	{
		return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
	}

	/**
	* x, y, z position of the light source. 
	*/
	vec3 position; 
};

/**
* Struct for simulating light sources that do not have an explicit position.
* Such light sources have only a direction against which they are shinning.
* Instantiations of the struct will have this direction properties along with
* a color and intensity of the light given off by the light source.
*/
struct DirectionalLight : public LightSource
{
	DirectionalLight(vec3 direction, const color & color)
		: LightSource(color), direction(normalize(direction))
	{}

	virtual color illuminate(const vec3 & eyeVector, HitRecord & closestHit, vector<Surface*> surfaces)
	{
		int shiney = 255;
		color white = color(1.0f, 1.0f, 1.0f, 1.0f);
		vec3 n = closestHit.surfaceNormal;
		vec3 v = normalize(eyeVector - closestHit.interceptPoint);
		vec3 l = normalize(direction - closestHit.interceptPoint);
		vec3 h = normalize(v + l);

		
		for (int i = 0; i < surfaces.size(); i++){
			vec3 point = closestHit.interceptPoint + (EPSILON * closestHit.surfaceNormal);
			HitRecord hr = surfaces[i]->findClosestIntersection(point, l);
			if (hr.t < FLT_MAX)
			{
				if (hr.t < 0) {
					return color(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}
		}
		
		color diffuse = max(0.0f, dot(n, l))*lightColor*closestHit.material;
		color specular = pow(max(0.0f, dot(n, h)), shiney)*white*white;
		return diffuse + specular;

	}

	/**
	* Unit vector that points in the direction that is opposite 
	* the direction in which the light is shining.
	*/
	vec3 direction; 
};


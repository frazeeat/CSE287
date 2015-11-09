#pragma once

#include "lab9.h"

vector<glm::vec4> buildPyramid(float width = 1.0f, float height = 1.0f);

struct ReferencePlane
{
	ReferencePlane( float planeWidth = 8.0f, 
					color c1 = color(0.0f, 0.0f, 0.0f, 1.0f), 
					color c2 = color(1.0f, 1.0f, 1.0f, 1.0f));

	void draw();

	color color1;
	color color2;

	vector<glm::vec4> c1PlaneVertices;
	vector<glm::vec4> c2PlaneVertices;

};


struct Pyramid
{
	Pyramid(float width = 1.0f, float height = 1.0f);

	void draw(color pyramidColor);

	vector<glm::vec4> triangleVertices;
};


struct Sphere
{
	Sphere(float radius = 0.5f, int slices = 8, int stacks = 8);

	void draw(color sphereColor);

	vector<glm::vec4> triangleVertices;
};

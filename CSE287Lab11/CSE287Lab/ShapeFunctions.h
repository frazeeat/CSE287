#pragma once

#include "BasicIncludesAndDefines.h"

#include "lab.h"

vector<glm::vec4> buildPyramid(float width = 1.0f, float height = 1.0f);

struct ReferencePlane
{
	ReferencePlane( float planeWidth = 8.0f, 
					color c1 = color(0.0f, 0.0f, 0.0f, 1.0f), 
					color c2 = color(1.0f, 1.0f, 1.0f, 1.0f));

	void draw();

	color color1;
	color color2;

	vector<VertexData> c1PlaneVertices;
	vector<VertexData> c2PlaneVertices;

};


struct Pyramid
{
	Pyramid(float width = 1.0f, float height = 1.0f);

	void draw(color pyramidColor);

	vector<VertexData> triangleVertices;
};


struct Sphere
{
	Sphere(float radius = 0.5f, int slices = 8, int stacks = 8);

	void draw(color sphereColor);

	vector<VertexData> triangleVertices;
};



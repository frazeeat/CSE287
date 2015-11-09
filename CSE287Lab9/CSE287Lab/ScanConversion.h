#pragma once

#include "BasicIncludesAndDefines.h"

// Swaps two points
void swap(glm::vec3 &p1, glm::vec3 &p2);

// Draws a line segment if there are at least two vertices
void drawLine(vector<glm::vec3> vertices, color rgba);

// Draws a wire frame if there are at least three vertices
void drawWireFrameTriangle(vector<glm::vec3> vertices, color rgba);
void drawWireFrameTriangle(vector<glm::vec4> vertices, color rgba);

void drawManyWireFrameTriangles(vector<glm::vec4> vertices, color rgba);

// Draws a filled triangle if there are at least three vertices
void drawFilledTriangle(vector<glm::vec4> vertices, color rgba);

void drawManyFilledTriangles(vector<glm::vec4> vertices, color rgba);

// Draws a wire frame rectangle bounded by xMin, yMin, xMax and yMax
void drawWireRectangle(float xMin, float yMin, float xMax, float yMax, color rgba);

// Draws a filled rectangle bounded by xMin, yMin, xMax and yMax
void drawFilledRectangle(float xMin, float yMin, float xMax, float yMax, color rgba);

// Draws a filled in polygon if there are at least three vertices. 
// Polygon must be convex.
void drawFilledPolygon(vector<glm::vec3> vertices, color rgba);
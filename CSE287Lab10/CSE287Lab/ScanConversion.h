#pragma once

#include "BasicIncludesAndDefines.h"

// Swaps two points
void swap(glm::vec3 &p1, glm::vec3 &p2);

// Draws a line segment if there are at least two vertices
void drawLine(vector<glm::vec3> vertices, color rgba);

// Draws a wire frame if there are at least three vertices
void drawWireFrameTriangle(const vector<glm::vec3> & vertices, const color & rgba);
void drawWireFrameTriangle(const vector<glm::vec4> & vertices, const color & rgba);

void drawManyWireFrameTriangles(const vector<glm::vec4> & vertices, const color & rgba);

// Draws a filled triangle if there are at least three vertices
void drawFilledTriangle(const vector<glm::vec4> & vertices, const color & rgba);

void drawManyFilledTriangles(const vector<glm::vec4> & vertices, const color & rgba);

// Draws a wire frame rectangle bounded by xMin, yMin, xMax and yMax
void drawWireRectangle(const float xMin, const float yMin, const float xMax, const float yMax, const color & rgba);

// Draws a filled rectangle bounded by xMin, yMin, xMax and yMax
void drawFilledRectangle(const float & xMin, const float & yMin, const float & xMax, const float & yMax, const color & rgba);

// Draws a filled in polygon if there are at least three vertices. 
// Polygon must be convex.
void drawFilledPolygon(const vector<glm::vec3> & vertices, const color & rgba);
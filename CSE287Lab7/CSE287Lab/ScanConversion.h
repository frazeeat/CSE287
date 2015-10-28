#pragma once

#include "BasicIncludesAndDefines.h"




// Swaps two points
void swap(vecX &p1, vecX &p2);

// Draws a line segment if there are at least two vertices
void drawLine(vector<vecX> vertices, color rgba);

// Draws a wire frame if there are at least three vertices
void drawWireTriangle(vector<vecX> vertices, color rgba);

// Draws a filled triangle if there are at least three vertices
void drawFilledTriangle(vector<vecX> vertices, color rgba);

// Draws a wire frame rectangle bounded by xMin, yMin, xMax and yMax
void drawWireRectangle(float xMin, float yMin, float xMax, float yMax, color rgba);

// Draws a filled rectangle bounded by xMin, yMin, xMax and yMax
void drawFilledRectangle(float xMin, float yMin, float xMax, float yMax, color rgba);

// Draws a filled in polygon if there are at least three vertices. 
// Polygon must be convex.
void drawFilledPolygon(vector<vecX> vertices, color rgba);
#include "ScanConversion.h"

// ColorBuffer object that is global to all files
extern ColorBuffer colorBuffer;

void swap(glm::vec3 &p1, glm::vec3 &p2)
{
	glm::vec3 temp = p1;
	p1 = p2;
	p2 = temp;
}

void drawVerticalLine(glm::vec3 p1, glm::vec3 p2, color rgba)
{
	if (p2.y < p1.y) {

		swap(p1, p2);
	}

	for (float y = p1.y; y < p2.y; y++) {

		colorBuffer.setPixel(p1.x, y, rgba);
	}

} // end drawVerticalLine

void drawHorizontalLine(glm::vec3 p1, glm::vec3 p2, color rgba)
{
	if (p2.x < p1.x) {

		swap(p1, p2);
	}

	for (float x = p1.x; x < p2.x; x++) {

		colorBuffer.setPixel(x, p2.y, rgba);
	}

} // end drawVerticalLine


void drawBresenhamLine(glm::vec3 p1, glm::vec3 p2, color rgba) {

	int x0 = (int)p1.x, y0 = (int)p1.y;
	int x1 = (int)p2.x, y1 = (int)p2.y;

	int dx = std::abs(x1 - x0);
	int dy = std::abs(y1 - y0);

	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;

	int err = dx - dy;

	colorBuffer.setPixel(x0, y0, rgba);

	while (x0 != x1 && y0 != y1) {
		int e2 = err << 1;

		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}

		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}

		colorBuffer.setPixel(x0, y0, rgba);
	}
}

void drawLine(vector<glm::vec3> vertices, color rgba)
{
	if (vertices.size() > 1) {

		glm::vec3 v0 = vertices[0];
		glm::vec3 v1 = vertices[1];

		if (v0.x == v1.x) {
			drawVerticalLine(v0, v1, rgba);
		}
		else if (v0.y == v1.y) {
			drawHorizontalLine(v0, v1, rgba);
		}
		else {
			drawBresenhamLine(v0, v1, rgba);
		}
	}

}

void drawFilledRectangle(float xMin, float yMin, float xMax, float yMax, color rgba)
{
	for (int y = (int)yMin; y <= (int)yMax; y++){

		drawHorizontalLine(glm::vec3(xMin, y, 1), glm::vec3(xMax, y, 1), rgba);
	}
}


void drawManyWireFrameTriangles(vector<glm::vec4> vertices, color rgba)
{
	vector<glm::vec4> triVerts;

	for (unsigned int i = 0; (i + 2) < vertices.size(); i += 3) {

		triVerts.push_back(vertices[i]);
		triVerts.push_back(vertices[i + 1]);
		triVerts.push_back(vertices[i + 2]);

		drawWireFrameTriangle(triVerts, rgba);

		triVerts.clear();
	}

} // end drawManyWireFrameTriangles


void drawWireFrameTriangle(vector<glm::vec4> vertices, color rgba)
{
	if (vertices.size() > 2) {

		vector<glm::vec3> l0;
		l0.push_back(vertices[0].xyz);
		l0.push_back(vertices[1].xyz);
		vector<glm::vec3> l1;
		l1.push_back(vertices[1].xyz);
		l1.push_back(vertices[2].xyz);
		vector<glm::vec3> l2;
		l2.push_back(vertices[2].xyz);
		l2.push_back(vertices[0].xyz);

		drawLine(l0, rgba);
		drawLine(l1, rgba);
		drawLine(l2, rgba);
	}

} // end drawWireFrameTriangle


void drawWireFrameTriangle(vector<glm::vec3> vertices, color rgba)
{
	if (vertices.size() > 2) {

		vector<glm::vec3> l0;
		l0.push_back(vertices[0]);
		l0.push_back(vertices[1]);
		vector<glm::vec3> l1;
		l1.push_back(vertices[1]);
		l1.push_back(vertices[2]);
		vector<glm::vec3> l2;
		l2.push_back(vertices[2]);
		l2.push_back(vertices[0]);

		drawLine(l0, rgba);
		drawLine(l1, rgba);
		drawLine(l2, rgba);
	}

} // end drawWireFrameTriangle

struct edge
{
	float yMin;
	float yMax;
	float x;
	float m;
	float b;
	float oneOverSlope;

	glm::vec4 p1, p2;

	bool horizontal;
	bool vertical;

	edge(const glm::vec4 & v1, const glm::vec4 & v2)
	{
		if (v1.y > v2.y) {
			p1 = v2;
			p2 = v1;
		}
		else {
			p1 = v1;
			p2 = v2;
		}

		if (p1.y == p2.y) {
			horizontal = true;
			m = 0;
		}
		else {
			horizontal = false;
			m = (p2.y - p1.y) / (p2.x - p1.x);
			oneOverSlope = (p2.x - p1.x) / (p2.y - p1.y);
		}

		if (p1.x == p2.x) {
			vertical = true;
		}
		else {
			vertical = false;
		}

		yMin = p1.y;
		yMax = p2.y;
		x = p1.x;
		b = p1.y - m*p1.x;
	}
};

void sortEdges(vector<edge> &edgeList) {

	unsigned int i, j, iMin;

	// advance the position through the entire array
	for (j = 0; j < edgeList.size() - 1; j++) {

		// assume the min is the first element
		iMin = j;
		// test against elements after j to find the smallest
		for (i = j + 1; i < edgeList.size(); i++) {
			// if this element is less, then it is the new minimum
			if (edgeList[i].yMin < edgeList[iMin].yMin) {
				// found new minimum; remember its index
				iMin = i;
			}
		}

		if (iMin != j) {
			//swap
			edge temp = edgeList[j];
			edgeList[j] = edgeList[iMin];
			edgeList[iMin] = temp;
		}
	}
} // end sortEdges


void drawFilledPolygon(vector<glm::vec4> vertices, color rgba) {

	if (vertices.size() < 3) {
		return;
	}
	// vector holding all edges in polygon
	vector<edge> edgeList;

	// create edges from vector of vertices and add to edgeList
	for (unsigned int i = 0; i<vertices.size() - 1; i++) {
		if (edge(vertices[i], vertices[i + 1]).horizontal != true) edgeList.push_back(edge(vertices[i], vertices[i + 1]));
	}
	// connect edge from last point to first point
	if (edge(vertices[vertices.size() - 1], vertices[0]).horizontal != true) edgeList.push_back(edge(vertices[vertices.size() - 1], vertices[0]));

	// initially order the edges
	sortEdges(edgeList);

	// set the first scanline to the minimum y value
	float scanLine = edgeList[0].yMin;

	// for all the edges in the vector
	while (edgeList.size() > 1) {

		// while the scanLine hasn't exceeded max y value of either edge
		while ((scanLine <= edgeList[0].yMax) && (scanLine <= edgeList[1].yMax)) {

			// placeholders for x values
			float xLeft, xRight;

			// if the edge is not vertical, use slope to calculate x
			if (edgeList[0].p1.x != edgeList[0].p2.x) {
				xLeft = (scanLine - edgeList[0].b) / edgeList[0].m;
			}
			// if vertical, use the x value without slope
			else {
				xLeft = edgeList[0].p1.x;
			}

			// if the edge is not vertical, use slope to calculate x
			if (edgeList[1].p1.x != edgeList[1].p2.x) {
				xRight = (scanLine - edgeList[1].b) / edgeList[1].m;
			}
			// if vertical, use the x value without slope
			else {
				xRight = edgeList[1].p1.x;
			}

			// if left x is less than right x, draw line from left to right
			if (edgeList[0].x < edgeList[1].x) {
				drawHorizontalLine(glm::vec3(xLeft, scanLine, 1), glm::vec3(xRight, scanLine, 1), rgba);
			}
			// if left x is greater, draw scanline from right x to left x values
			else {
				drawHorizontalLine(glm::vec3(xRight, scanLine, 1), glm::vec3(xLeft, scanLine, 1), rgba);
			}

			// increment scanline to next y value
			scanLine++;
		}

		// when scanLine exceeds the max of either edge, erase it
		if (scanLine > edgeList[0].yMax) {
			edgeList.erase(edgeList.begin() + 0); // scanline exceeded first edge
		}
		else {
			edgeList.erase(edgeList.begin() + 1); // scanline exceeded second edge
		}

		// sort remaining edges
		sortEdges(edgeList);
	}

} // end drawFilledPolygon


bool allSameY(vector<glm::vec4> vertices)
{
	if (vertices[0].y == vertices[1].y  && vertices[1].y == vertices[2].y) {
		return true;
	}
	else {
		return false;
	}
}

void drawManyFilledTriangles(vector<glm::vec4> vertices, color rgba)
{
	vector<glm::vec4> triVerts;

	for (unsigned int i = 0; (i + 2) < vertices.size(); i += 3) {

		triVerts.push_back(vertices[i]);
		triVerts.push_back(vertices[i + 1]);
		triVerts.push_back(vertices[i + 2]);

		drawFilledTriangle(triVerts, rgba);
		drawWireFrameTriangle(triVerts, rgba);

		triVerts.clear();
	}

} // end drawManyFilledTriangles

void drawFilledTriangle(vector<glm::vec4> vertices, color rgba)
{
	vector<glm::vec4> v;

	for (glm::vec4 va : vertices) {
		v.push_back(va);
	}

	//setV0V1V2(vertices[0], vertices[1], vertices[2]);

	if (!allSameY(vertices)) {

		drawFilledPolygon(v, rgba);
	}
}

void drawWireRectangle(float xMin, float yMin, float xMax, float yMax, color rgba)
{
	glm::vec3 lowerLeft = glm::vec3(xMin, yMin, 1);
	glm::vec3 upperLeft = glm::vec3(xMax, yMax, 1);
	glm::vec3 lowerRight = glm::vec3(xMax, yMin, 1);
	glm::vec3 upperRight = glm::vec3(xMin, yMax, 1);

	drawHorizontalLine(lowerLeft, lowerRight, rgba);
	drawHorizontalLine(upperLeft, upperRight, rgba);
	drawVerticalLine(lowerLeft, upperLeft, rgba);
	drawVerticalLine(lowerRight, upperRight, rgba);

} // end drawWireRectangle
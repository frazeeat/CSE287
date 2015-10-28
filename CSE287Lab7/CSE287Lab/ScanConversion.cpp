#include "ScanConversion.h"

// ColorBuffer object that is global to all files
extern ColorBuffer colorBuffer;

void swap(vecX &p1, vecX &p2)
{
	vecX temp = p1;
	p1 = p2;
	p2 = temp;
}

void drawVerticalLine(vecX p1, vecX p2, color rgba)
{
		if (p2.y < p1.y) {

			swap(p1, p2);
		}

		for( float y = p1.y; y < p2.y; y++ ) {

			colorBuffer.setPixel(p1.x, y, rgba);
		}

} // end drawVerticalLine

void drawHorizontalLine(vecX p1, vecX p2, color rgba)
{
		if (p2.x < p1.x) {

			swap(p1, p2);
		}

		for( float x = p1.x; x < p2.x; x++ ) {

			colorBuffer.setPixel(x, p2.y, rgba);
		}

} // end drawVerticalLine


void drawBresenhamLine(glm::vecX p1, glm::vecX p2, color rgba) {

	int x0 = (int) p1.x, y0 = (int) p1.y;
	int x1 = (int) p2.x, y1 = (int) p2.y;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;

    int err = dx - dy;

	colorBuffer.setPixel(x0, y0, rgba);

    while(x0 != x1 && y0 != y1) {
        int e2 = err << 1;

        if(e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if(e2 < dx) {
            err += dx;
            y0 += sy;
        }

		colorBuffer.setPixel(x0, y0, rgba);
    }
}

void drawLine(vector<vecX> vertices, color rgba)
{
	if (vertices.size() > 1) {

		vecX v0 = vertices[0];
		vecX v1 = vertices[1];

		if(v0.x == v1.x) {
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

	for (int y = (int)yMin; y <= (int)yMax; y++ ){

		drawHorizontalLine(vecX(xMin, y, 1), vecX(xMax, y, 1), rgba);
	}
}



void drawWireTriangle(vector<vecX> vertices, color rgba)
{
	if (vertices.size() > 2) {

		mat3 trans(1.0f);
		trans[2][0] = colorBuffer.getWindowWidth() / 2.0f;
		trans[2][1] = colorBuffer.getWindowHeight() / 2.0f;
		trans[2][2] = 1.0f;

		vector<vecX> l0;
		l0.push_back(trans*vertices[0]);
		l0.push_back(trans*vertices[1]);
		vector<vecX> l1;
		l1.push_back(trans*vertices[1]);
		l1.push_back(trans*vertices[2]);
		vector<vecX> l2;
		l2.push_back(trans*vertices[2]);
		l2.push_back(trans*vertices[0]);

		drawLine(l0, rgba);
		drawLine(l1, rgba);
		drawLine(l2, rgba);
	}

} // end drawTriangle

struct edge
{
	float yMin;
	float yMax;
	float x;
	float m;
	float b;
	float oneOverSlope;
	
	vecX p1, p2;

	bool horizontal;
	bool vertical;

	edge( const vecX & v1, const vecX & v2 )
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
			m = (p2.y - p1.y)/(p2.x - p1.x);
			oneOverSlope = (p2.x - p1.x)/(p2.y - p1.y);
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
	for (j = 0; j < edgeList.size()-1; j++) {

		// assume the min is the first element
		iMin = j;
		// test against elements after j to find the smallest
		for (i = j+1; i < edgeList.size(); i++) {
			// if this element is less, then it is the new minimum
			if (edgeList[i].yMin < edgeList[iMin].yMin) {
				// found new minimum; remember its index
				iMin = i;
			}
		}

		if(iMin != j) {
			//swap
			edge temp = edgeList[j];
			edgeList[j] = edgeList[iMin];
			edgeList[iMin] = temp;
		}
	}
} // end sortEdges


void drawFilledPolygon(vector<vecX> vertices, color rgba) {

	if (vertices.size() < 3) {
		return;
	}
	// vector holding all edges in polygon
	vector<edge> edgeList;

	// create edges from vector of vertices and add to edgeList
	for(unsigned int i=0; i<vertices.size()-1; i++ ) {	
		if ( edge(vertices[i], vertices[i+1]).horizontal != true) edgeList.push_back(edge(vertices[i], vertices[i+1]));
	}
	// connect edge from last point to first point
	if (edge(vertices[vertices.size()-1], vertices[0]).horizontal != true) edgeList.push_back(edge(vertices[vertices.size()-1], vertices[0]));

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
			if(edgeList[0].p1.x != edgeList[0].p2.x) {
				xLeft = (scanLine - edgeList[0].b) / edgeList[0].m;
			} 
			// if vertical, use the x value without slope
			else {
				xLeft = edgeList[0].p1.x;
			}

			// if the edge is not vertical, use slope to calculate x
			if(edgeList[1].p1.x != edgeList[1].p2.x) {
				xRight = (scanLine - edgeList[1].b) / edgeList[1].m;
			} 
			// if vertical, use the x value without slope
			else {
				xRight = edgeList[1].p1.x;
			}

			// if left x is less than right x, draw line from left to right
			if ( edgeList[0].x < edgeList[1].x) {
				drawHorizontalLine(vecX(xLeft, scanLine,1), vecX(xRight, scanLine,1), rgba);
			} 
			// if left x is greater, draw scanline from right x to left x values
			else {
				drawHorizontalLine(vecX(xRight, scanLine,1), vecX(xLeft, scanLine,1),rgba);
			}

			// increment scanline to next y value
			scanLine++;
		} 
			
		// when scanLine exceeds the max of either edge, erase it
		if (scanLine > edgeList[0].yMax) {
			edgeList.erase(edgeList.begin()+0); // scanline exceeded first edge
		} else {
			edgeList.erase(edgeList.begin()+1); // scanline exceeded second edge
		}

		// sort remaining edges
		sortEdges(edgeList);
	}

} // end drawSolidTriangle


bool allSameY(vector<vecX> vertices)
{
	if (vertices[0].y == vertices[1].y  && vertices[1].y == vertices[2].y) {
		return true;
	}
	else {
		return false;
	}
}

void drawFilledTriangle(vector<vecX> vertices, color rgba)
{
	vector<vecX> v;

	mat3 trans(1.0f);
	trans[2][0] = colorBuffer.getWindowWidth() / 2.0f;
	trans[2][1] = colorBuffer.getWindowHeight() / 2.0f;
	trans[2][2] = 1.0f;

	for (vecX va : vertices) {
		v.push_back(trans * va);
	}

	//setV0V1V2(vertices[0],vertices[1],vertices[2]);

	if( !allSameY(vertices) ) {

		drawFilledPolygon(v, rgba);
	}
}

void drawWireRectangle(float xMin, float yMin, float xMax, float yMax, color rgba)
{
	vecX lowerLeft = vecX( xMin, yMin, 1);
	vecX upperLeft = vecX(xMax, yMax, 1 );
	vecX lowerRight = vecX(xMax, yMin, 1 );
	vecX upperRight = vecX(xMin, yMax, 1 );

	drawHorizontalLine( lowerLeft, lowerRight, rgba );
	drawHorizontalLine( upperLeft, upperRight, rgba );
	drawVerticalLine( lowerLeft, upperLeft, rgba );
	drawVerticalLine( lowerRight, upperRight, rgba );

} // end drawWireRectangle
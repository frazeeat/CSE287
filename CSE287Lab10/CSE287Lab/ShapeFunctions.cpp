#include "ShapeFunctions.h"

#define PI 3.1415926535897932384626433832795f


Pyramid::Pyramid(float width, float height)
{
	// Set vertex locations for one triangle
	triangleVertices.push_back(glm::vec4(0.0f, height / 2.0f, 0.0f, 1.0f));
	triangleVertices.push_back(glm::vec4(-width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));
	triangleVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));

	triangleVertices.push_back(glm::vec4(0.0f, height / 2.0f, 0.0f, 1.0f));
	triangleVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));
	triangleVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f));

	triangleVertices.push_back(glm::vec4(0.0f, height / 2.0f, 0.0f, 1.0f));
	triangleVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f));
	triangleVertices.push_back(glm::vec4(-width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f));

	triangleVertices.push_back(glm::vec4(0.0f, height / 2.0f, 0.0f, 1.0f));
	triangleVertices.push_back(glm::vec4(-width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f));
	triangleVertices.push_back(glm::vec4(-width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));

	triangleVertices.push_back(glm::vec4(-width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));
	triangleVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f));
	triangleVertices.push_back(glm::vec4(-width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f));

	triangleVertices.push_back(glm::vec4(-width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));
	triangleVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));
	triangleVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f));

}

void Pyramid::draw(color pyramidColor)
{

	vector<glm::vec4> transformedVertices = pipeline(triangleVertices);

	drawManyFilledTriangles(transformedVertices, pyramidColor);
	drawManyWireFrameTriangles(transformedVertices, pyramidColor);

}


ReferencePlane::ReferencePlane( float planeWidth, color c1, color c2 )
	: color1(c1), color2(c2)
{
	c1PlaneVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	c1PlaneVertices.push_back(glm::vec4(-planeWidth / 2.0f, 0.0f, -planeWidth / 2.0f, 1.0f));
	c1PlaneVertices.push_back(glm::vec4(-planeWidth / 2.0f, 0.0f, planeWidth / 2.0f, 1.0f));
	c1PlaneVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	c1PlaneVertices.push_back(glm::vec4(planeWidth / 2.0f, 0.0f, planeWidth / 2.0f, 1.0f));
	c1PlaneVertices.push_back(glm::vec4(planeWidth / 2.0f, 0.0f, -planeWidth / 2.0f, 1.0f));

	c2PlaneVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	c2PlaneVertices.push_back(glm::vec4(planeWidth / 2.0f, 0.0f, -planeWidth / 2.0f, 1.0f));
	c2PlaneVertices.push_back(glm::vec4(-planeWidth / 2.0f, 0.0f, -planeWidth / 2.0f, 1.0f));

	c2PlaneVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	c2PlaneVertices.push_back(glm::vec4(-planeWidth / 2.0f, 0.0f, planeWidth / 2.0, 1.0f));
	c2PlaneVertices.push_back(glm::vec4(planeWidth / 2.0f, 0.0f, planeWidth / 2.0f, 1.0f));
}

void ReferencePlane::draw()
{
	drawManyFilledTriangles(pipeline(c1PlaneVertices), color1 );
	drawManyFilledTriangles(pipeline(c2PlaneVertices), color2 );

} // end draw


glm::vec4 getSphericalCoordinate(float radius, float theta, float phi)
{
	glm::vec4 coord( radius * glm::cos(theta) * glm::sin(phi),
					 radius * glm::sin(theta) * glm::sin(phi),
					 radius * glm::cos(phi), 
					 1.0f );

	return coord;
}

Sphere::Sphere(float radius, int slices, int stacks)
{
	for (int t = 0; t < stacks; t++) // stacks are ELEVATION so they count theta
	{
		float theta1 = ((float)(t) / stacks)*PI;
		float theta2 = ((float)(t + 1) / stacks)*PI;

		for (int p = 0; p < slices; p++) // slices are ORANGE SLICES so the count azimuth
		{
			float phi1 = ((float)(p) / slices) * 2 * PI; // azimuth goes around 0 .. 2*PI
			float phi2 = ((float)(p + 1) / slices) * 2 * PI;

			//phi2   phi1
			// |      |
			// 2------1 -- theta1
			// |\ _   |
			// |    \ |
			// 3------4 -- theta2
			//

			glm::vec4 vertex1 = getSphericalCoordinate(radius, theta1, phi1);
			glm::vec4 vertex2 = getSphericalCoordinate(radius, theta1, phi2);
			glm::vec4 vertex3 = getSphericalCoordinate(radius, theta2, phi2);
			glm::vec4 vertex4 = getSphericalCoordinate(radius, theta2, phi1);

			// facing out
			if (t == 0) { // top cap
				//mesh->addTri(vertex1, vertex3, vertex4); //t1p1, t2p2, t2p1
				triangleVertices.push_back(vertex1);
				triangleVertices.push_back(vertex3);
				triangleVertices.push_back(vertex4);

			}
			else if (t + 1 == stacks) {//end cap
				// mesh->addTri(vertex3, vertex1, vertex2); //t2p2, t1p1, t1p2
				triangleVertices.push_back(vertex3);
				triangleVertices.push_back(vertex1);
				triangleVertices.push_back(vertex2);

			}
			else
			{
				// body, facing OUT:
				//mesh->addTri(vertex1, vertex2, vertex4);
				triangleVertices.push_back(vertex1);
				triangleVertices.push_back(vertex2);
				triangleVertices.push_back(vertex4);

				//mesh->addTri(vertex2, vertex3, vertex4);
				triangleVertices.push_back(vertex2);
				triangleVertices.push_back(vertex3);
				triangleVertices.push_back(vertex4);

			}
		}
	}
}

void Sphere::draw(color sphereColor)
{
	vector<glm::vec4> transformedVertices = pipeline(triangleVertices);

	drawManyFilledTriangles(transformedVertices, sphereColor);
	drawManyWireFrameTriangles(transformedVertices, sphereColor);

}

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
Cube::Cube(float width){

	//Cubes have 8 vertecies. 
	glm::vec4 A = glm::vec4(width / 2.0f, width / 2.0f, width / 2.0f, 1.0f);
	glm::vec4 B = glm::vec4(-width / 2.0f, width / 2.0f, width / 2.0f, 1.0f);
	glm::vec4 C = glm::vec4(-width / 2.0f, -width / 2.0f, width / 2.0f, 1.0f);
	glm::vec4 D = glm::vec4(width / 2.0f, -width / 2.0f, width / 2.0f, 1.0f);
	glm::vec4 E = glm::vec4(width / 2.0f, width / 2.0f, -width / 2.0f, 1.0f);
	glm::vec4 F = glm::vec4(-width / 2.0f, width / 2.0f, -width / 2.0f, 1.0f);
	glm::vec4 G = glm::vec4(width / 2.0f, -width / 2.0f, -width / 2.0f, 1.0f);
	glm::vec4 H = glm::vec4(-width / 2.0f, -width / 2.0f, -width / 2.0f, 1.0f);

	triangleVertices.push_back(A);
	triangleVertices.push_back(B);//A-B-C
	triangleVertices.push_back(C);

	triangleVertices.push_back(A);
	triangleVertices.push_back(C);//A-C-D
	triangleVertices.push_back(D);

	triangleVertices.push_back(E);
	triangleVertices.push_back(F);//E-F-B
	triangleVertices.push_back(B);

	triangleVertices.push_back(E);
	triangleVertices.push_back(B);//E-B-A
	triangleVertices.push_back(A);

	triangleVertices.push_back(E);
	triangleVertices.push_back(A);//E-A-D
	triangleVertices.push_back(D);

	triangleVertices.push_back(E);
	triangleVertices.push_back(D);//E-D-G
	triangleVertices.push_back(G);

	triangleVertices.push_back(B);
	triangleVertices.push_back(F);//B-F-H
	triangleVertices.push_back(H);

	triangleVertices.push_back(B);
	triangleVertices.push_back(H);//B-H-C
	triangleVertices.push_back(C);

	triangleVertices.push_back(F);
	triangleVertices.push_back(E);//F-E-G
	triangleVertices.push_back(G);

	triangleVertices.push_back(F);
	triangleVertices.push_back(G);//F-G-H
	triangleVertices.push_back(H);

	triangleVertices.push_back(H);
	triangleVertices.push_back(G);//H-G-D
	triangleVertices.push_back(D);

	triangleVertices.push_back(H);
	triangleVertices.push_back(D);//H-D-C
	triangleVertices.push_back(C);

}

void Cube::draw(color CubeColor)
{
	vector<glm::vec4> transformedVertices = pipeline(triangleVertices);

	drawManyFilledTriangles(transformedVertices, CubeColor);
	drawManyWireFrameTriangles(transformedVertices, CubeColor);

}
GameBoard::GameBoard(float gameWidth, color c1, color c2)
	: color1(c1), color2(c2)
{
	float x, z;
	int int_width = floor(gameWidth), skip = 0,r=0;
	int vert_per_row = floor(gameWidth) + 1;
	bool sw = false;
	std::vector<glm::vec4> vector;
	for (int i = 0; i <= int_width; i++){
		x = -gameWidth / 2.0f + i;
		for (int j = 0; j <= int_width; j++){
			z = -gameWidth / 2.0f + j;
			//std::cout << "x: " << x << " z: "<< z << std::endl;
			vector.push_back(glm::vec4(x, 0.0f, z, 1.0f));
		}
	}
	for (int k = 0; k < vector.size() - vert_per_row-1;k++){
		if (k == int_width + (vert_per_row*skip)){
			skip++;
			if (int_width%2 == 0){

				if (sw){
					sw = false;
				}
				else{
					sw = true;
				}
			}
		}
		else{
			glm::vec4 A = vector[k+1];
			glm::vec4 B = vector[k];
			glm::vec4 C = vector[k + vert_per_row];
			glm::vec4 D = vector[k + vert_per_row+1];
			if (sw){
				c1BoardVertices.push_back(A);
				c1BoardVertices.push_back(B);
				c1BoardVertices.push_back(C);
				c1BoardVertices.push_back(A);
				c1BoardVertices.push_back(C);
				c1BoardVertices.push_back(D);
				sw = false;
			}
			else{
				c2BoardVertices.push_back(A);
				c2BoardVertices.push_back(B);
				c2BoardVertices.push_back(C);
				c2BoardVertices.push_back(A);
				c2BoardVertices.push_back(C);
				c2BoardVertices.push_back(D);
				sw = true;
			}
		}
	}

}

void GameBoard::draw()
{
	drawManyFilledTriangles(pipeline(c1BoardVertices), color1);
	drawManyFilledTriangles(pipeline(c2BoardVertices), color2);

} // end draw

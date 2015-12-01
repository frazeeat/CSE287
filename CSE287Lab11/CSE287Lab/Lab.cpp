#include "Lab.h"

struct Plane {

	glm::vec3 a; // point on the plane
	glm::vec3 n; // vector that is normal to the front face of a plane

	Plane(){};

	Plane(glm::vec3 point, glm::vec3 normal)
	{
		a = point;
		n = glm::normalize(normal);
	}

	// Returns true is a point is to the "left" of a plane.
	bool insidePlane(const VertexData & point)
	{
		if (glm::dot((point.position.xyz - a), n) > 0.0) {
			return true;
		}
		else {
			return false;
		}

	} // end insidePlane

	// Finds the point of intersection for a line segment and a plane
	VertexData findIntersection(const VertexData &  p1, const VertexData & p2)
	{
		float d1 = glm::dot(p1.position.xyz - a, n);
		float d2 = glm::dot(p2.position.xyz - a, n);

		float t = d1 / (d1 - d2);

		VertexData I;
		// Find point of intersection
		I.position = p1.position + t * (p2.position - p1.position);

		// Find interpolated values for the vertex data
		I.material = p1.material + t * (p2.material - p1.material);
		I.normal = p1.normal + t * (p2.normal - p1.normal);

		return I;

	} // end findIntersection
};

/********************** GLOBALS ******************************/

glm::mat4 modelingTransformation;
glm::mat4 viewingTransformation;
glm::mat4 projectionTransformation;
glm::mat4 viewportTransformation;

// View port limits
float xViewportMin, yViewportMin, xViewportMax, yViewportMax;

// Normalized device coordinate limits
int xNdcMin = -1, yNdcMin = -1, xNdcMax = 1, yNdcMax = 1;

// Frame buffer that contains both the color and depth buffers
FrameBuffer frameBuffer(winWidth, winHeight);

// Object in the scene
ReferencePlane refPlane;
Pyramid pyramid;
Sphere sphere(0.5f, 16, 16);

// Globals that control view
float rotationX = 0;
float rotationY = 0;
float zTrans = -12;

float angle = glm::radians(45.0f);

// Planes describing the normalized device coordinates view volume
vector<Plane> ndcPlanes;

// Make the planes for a normalized view volume
void makeClippingPlanes()
{
	ndcPlanes.push_back(Plane(glm::vec3(0, 1, 0), glm::vec3(0, -1, 0)));
	ndcPlanes.push_back(Plane(glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0)));
	ndcPlanes.push_back(Plane(glm::vec3(0, 0, 1), glm::vec3(0, 0, -1)));
	ndcPlanes.push_back(Plane(glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0)));
	ndcPlanes.push_back(Plane(glm::vec3(0, -1, 0), glm::vec3(0, 1, 0)));
	ndcPlanes.push_back(Plane(glm::vec3(0, 0, -1), glm::vec3(0, 0, 1)));

} // end makeClippingPlanes


// Clips a convex polygon against a plane.
vector<VertexData> clipAgainstPlane(vector<VertexData> verts, Plane plane)
{
	vector<VertexData> output;
	VertexData p1;
	VertexData p2;
	for (int i = 0; i < verts.size();i++){
		//VertexData p1 = verts[(i-1)%3];
		//std::cout << (i + 1) % 3 << std::endl;
		p1 = verts[i];
		p2 = verts[(i+1)%3];
		if (plane.insidePlane(p1)&&plane.insidePlane(p2)){
			output.push_back(p2);
		}
		else if (plane.insidePlane(p1)&&!plane.insidePlane(p2)){
			output.push_back(plane.findIntersection(p1,p2));
		}
		else if (!plane.insidePlane(p1)&&plane.insidePlane(p2)){
			output.push_back(plane.findIntersection(p1, p2));
			output.push_back(p2);
		}
		else{

		}
	}


	// TODO

	return output;

} // end clipAgainstPlane




// Break general convex polygons into triangles.
vector<VertexData> triangulate(vector<VertexData> poly)
{
	vector<VertexData> triangles;
	/*
	for (int i = 0; i < poly.size() - 2;i++){
		triangles.push_back(poly[0]);
		triangles.push_back(poly[i+1]);
		triangles.push_back(poly[i+2]);

	}
	*/
	
	
	for (int i = 2; i < poly.size() - 1; i++) {
		triangles.push_back(poly[i]);
		triangles.push_back(poly[i + 1]);
		triangles.push_back(poly[i + 2]);
	}
	
	
	// TODO

	return triangles;

} // end triangulate


// Clip polygons against a a normalized view volume. Vertices should be in 
// clip coordinates.
vector<VertexData> clip(vector<VertexData> clipCoords)
{
	vector<VertexData> ndcCoords;

	if (clipCoords.size()>2){

		vector<VertexData> polygon;

		for (unsigned int i = 0; i<clipCoords.size() - 2; i += 3) {

			polygon.push_back(clipCoords[i]);
			polygon.push_back(clipCoords[i + 1]);
			polygon.push_back(clipCoords[i + 2]);

			for (Plane plane : ndcPlanes) {
				polygon = clipAgainstPlane(polygon, plane);
			}

			if (polygon.size() > 3) {
				polygon = triangulate(polygon);
			}

			for (VertexData v : polygon) {
				ndcCoords.push_back(v);
			}
			polygon.clear();

		}
	}

	return ndcCoords;

} // end clip

// Give three vertices on the face of a polygon in counter clockwise 
// order calculates a normal vector for the polygon.
glm::vec3 findUnitNormal(glm::vec3 pZero, glm::vec3 pOne, glm::vec3 pTwo)
{
	return glm::normalize(glm::cross(pOne - pZero, pTwo - pOne));

} // end findUnitNormal

// Remove all triangles that are not facing the view point.
// Should be performed in orthographic coordinates such as normalized device coordinates.
vector<VertexData> removeBackwardFacingTriangles(vector<VertexData> triangleVerts)
{
	vector<VertexData> fowardFacingTriangles;
	VertexData vd;
	glm::vec3 a, b, c, d,v=glm::vec3(0.0f,0.0f,-1.0f);
	for (int i = 0; i < triangleVerts.size()-2; i+=3){
		a = triangleVerts[i].position.xyz;
		b = triangleVerts[i+1].position.xyz;
		c = triangleVerts[i+2].position.xyz;
		d = findUnitNormal(a, b, c);
		if (glm::dot(v,d) <= 0.0f){
			fowardFacingTriangles.push_back(triangleVerts[i]);
			fowardFacingTriangles.push_back(triangleVerts[i+1]);
			fowardFacingTriangles.push_back(triangleVerts[i+2]);
		}
	}
	
	// TODO
	

	return fowardFacingTriangles;

} // end removeBackwardFacingTriangles

// Applies a transformation in the form of a matrix to a list of vertices.
vector<VertexData> transformVertices(const glm::mat4 & transMatrix, const vector<VertexData> & vertices)
{
	vector<VertexData> transformedVertices;

	for (VertexData v : vertices) {

		VertexData vt(transMatrix * v.position, v.material, transMatrix * v.normal);

		transformedVertices.push_back(vt);

	}

	return transformedVertices;

} // end transformVertices


// Tranforms vertices from world to view port coordinate via eye, clip, and normalized device coordinates. 
// Vertices are clipped and backfaces are culled. Lighting calculations are performed in eye coordinates.
vector<VertexData> pipeline(const vector<VertexData> & objectCoords)
{
	vector<VertexData> worldCoords = transformVertices(modelingTransformation, objectCoords);

	//vec4 eyePositionInWorldCoords = glm::inverse(viewingTransformation)[3];

	vector<VertexData> eyeCoords = transformVertices(viewingTransformation, worldCoords);

	vector<VertexData> projCoords = transformVertices(projectionTransformation, eyeCoords);

	vector<VertexData> clipCoords;

	// Perspective division
	for (VertexData v : projCoords) {

		v.position = v.position / v.position.w;
		clipCoords.push_back(v);
	}

	// Backface culling
	clipCoords = removeBackwardFacingTriangles(clipCoords);

	// Clipping
	vector<VertexData> ndcCoords = clipCoords;
	clip(clipCoords);

	vector<VertexData> windowCoords = transformVertices(viewportTransformation, ndcCoords);

	return windowCoords;

} // end pipeline


void renderObjects()
{
	// Set Modeling transformation for the reference plane
	modelingTransformation = glm::translate(glm::vec3(0.0f, -3.0f, 0.0f));
	refPlane.draw();

	// Set modeling transformation for the front left pyramid
	modelingTransformation = glm::translate(glm::vec3(-3.5f, -2.5f, 3.5f));
	pyramid.draw(color(0.0f, 0.0f, 1.0f, 1.0f));

	// Set modeling transformation for the back right pyramid
	modelingTransformation = glm::translate(glm::vec3(3.5f, -2.5f, -3.5f));
	pyramid.draw(color(0.0f, 0.0f, 1.0f, 1.0f));

	// Set modeling transformation for the center pyramid
	modelingTransformation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) * glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
	pyramid.draw(color(1.0f, 0.0f, 0.0f, 1.0f));

	// Set modeling transformation for the right hand pyramid
	modelingTransformation = glm::translate(glm::vec3(3.0f, 0.0f, 0.0f)) * glm::rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f));
	pyramid.draw(color(1.0f, 1.0f, 0.0f, 1.0f));

	// Set modeling transformation for the left hand pyramid
	modelingTransformation = glm::translate(glm::vec3(-3.0f, 0.0f, 0.0f)) * glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
	pyramid.draw(color(1.0f, 0.0f, 1.0f, 1.0f));

} // end renderObjects


/**
* Acts as the display function for the window.
*/
static void RenderSceneCB()
{
	// Clear the color buffer
	frameBuffer.clearColorAndDepthBuffers();

	angle += glm::radians(1.0f);

	renderObjects();

	// Display the color buffer
	frameBuffer.showColorBuffer();

} // end RenderSceneCB

// Reset viewport limits for full window rendering each time the window is resized.
// This function is called when the program starts up and each time the window is 
// resized.
static void ResizeCB(int width, int height)
{
	// Size the color buffer to match the window size.
	frameBuffer.setFrameBufferSize(width, height);

	// Set rendering window parameters for viewport transfomation
	xViewportMin = 0;
	yViewportMin = 0;
	xViewportMax = (float)width;
	yViewportMax = (float)height;

	// Create a perspective projection matrix. glm::perspective vertical field of view is specifed in degrees.
	projectionTransformation = glm::perspective(45.0f, ((float)xViewportMax - xViewportMin) / ((float)yViewportMax - yViewportMin), 0.1f, 20.0f);

	// Set viewport transformation
	viewportTransformation =
		glm::translate(glm::vec3(xViewportMin, yViewportMin, 0.0f)) *
		glm::scale(glm::vec3((float)(xViewportMax - xViewportMin) / (xNdcMax - xNdcMin), (float)(yViewportMax - yViewportMin) / (yNdcMax - yNdcMin), 1.0f)) *
		glm::translate(glm::vec3(-xNdcMin, -yNdcMin, 0.0f));

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end ResizeCB


// Responds to 'f' and escape keys. 'f' key allows 
// toggling full screen viewing. Escape key ends the
// program. Allows lights to be individually turned on and off.
static void KeyboardCB(unsigned char key, int x, int y)
{
	switch (key) {

	case('f') : case('F') : // 'f' key to toggle full screen
		glutFullScreenToggle();
		break;
	case(27) : // Escape key
		glutLeaveMainLoop();
		break;
	case('w') : // "forward" key

		zTrans += 0.05f;

		break;
	case('s') : // "back" key
		zTrans -= 0.05f;

		break;
	default:
		cout << key << " key pressed." << endl;
	}

	// Set the viewing transformation based on the values held in the global varaiables zTrans,
	// rotationX, and rotationY.
	glm::mat4 transView = glm::translate(glm::vec3(0.0f, 0.0f, zTrans));
	glm::mat4 rotateViewX = glm::rotate(glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotateViewY = glm::rotate(glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));

	viewingTransformation = transView * rotateViewX * rotateViewY;

	glutPostRedisplay();

} // end KeyboardCB

// Responds to presses of the arrow keys
static void SpecialKeysCB(int key, int x, int y)
{
	static const float rotateInc = glm::radians(20.0f);

	switch (key) {

	case(GLUT_KEY_RIGHT) :

		rotationY -= rotateInc;

		break;
	case(GLUT_KEY_LEFT) :

		rotationY += rotateInc;

		break;
	case(GLUT_KEY_UP) :

		rotationX += rotateInc;

		break;
	case(GLUT_KEY_DOWN) :

		rotationX -= rotateInc;

		break;

	default:
		cout << key << " key pressed." << endl;
	}

	// Set the viewing transformation based on the values held in the global varaiables zTrans,
	// rotationX, and rotationY.
	glm::mat4 transView = glm::translate(glm::vec3(0.0f, 0.0f, zTrans));
	glm::mat4 rotateViewX = glm::rotate(glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotateViewY = glm::rotate(glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));

	viewingTransformation = transView * rotateViewX * rotateViewY;

	glutPostRedisplay();

} // end SpecialKeysCB



// Register as the "idle" function to have the screen continously
// repainted. Due to software rendering, the frame rate will not
// be fast enough to support motion simulation
static void animate()
{
	glutPostRedisplay();

} // end animate


void viewMenu(int value)
{
	switch (value) {

	case(0) :

		// "Quit" selected on the menu
		glutLeaveMainLoop();
		break;

	default:
		cout << "Invalid view selection " << endl; 
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();

} // end viewMenu


// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.
int main(int argc, char** argv)
{
	// freeGlut and Window initialization ***********************

	// Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
	glutInit(&argc, argv);

	// Set the initial display mode.
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

	// Set the initial window size
	glutInitWindowSize(winWidth, winHeight);

	// Create a window using a string and make it the current window.
	GLuint world_Window = glutCreateWindow("Projection and Viewport Transformations");

	// Indicate to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Callback for window redisplay
	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);
	glutIdleFunc(animate);

	// Create polygon submenu
	int menu1id = glutCreateMenu(viewMenu);
	// Specify menu items and integer identifiers
	glutAddMenuEntry("Full Screen", 1);
	glutAddMenuEntry("Vertical Split", 2);
	glutAddMenuEntry("Horzontal Split", 3);

	glutAddMenuEntry("Quit", 0);

	// Attach menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Request that the window be made full screen
	//glutFullScreenToggle();

	color blue(0.784, 0.784, 1.0, 1.0);

	// Set red, green, blue, and alpha to which the color buffer is cleared.
	frameBuffer.setClearColor(blue);

	// Set the initial viewing tranformation for the scene
	viewingTransformation = glm::translate(glm::vec3(0.0f, 0.0f, zTrans));

	makeClippingPlanes();

	// Enter the GLUT main loop. Control will not return until the window is closed.
	glutMainLoop();

	return 0;

} // end main



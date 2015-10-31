#include "Lab8.h"

/********************** GLOBALS ******************************/

glm::mat4 modelingTransformation;
glm::mat4 viewingTransformation;
glm::mat4 projectionTransformation;
glm::mat4 viewportTransformation;

// View port limits
float xVpMin, yVpMin, xVpMax, yVpMax;

// Normalized device coordinate limits
int xNdcMin = -1, yNdcMin = -1, xNdcMax = 1, yNdcMax = 1;

// Color buffer to hold pixel color values
ColorBuffer colorBuffer(winWidth, winHeight);

vector<glm::vec4> pyramidVertices;
vector<glm::vec4> redPlaneVertices;
vector<glm::vec4> greeenPlaneVertices;

// Applies a transformation in the form of a matrix to a list of vertices.
vector<glm::vec4> transformVertices(const glm::mat4 & transMatrix, vector<glm::vec4>  vertices)
{
	vector<glm::vec4> transformedVertices;

	for (unsigned int i = 0; i < vertices.size(); i++) {

		glm::vec4 vt(transMatrix * vertices[i]);

		transformedVertices.push_back(vt);
	}

	return transformedVertices;

} // end transformVertices


// Tranforms vertices from world to window coordinates, via world, eye, clip, and normalized device coordinates.
vector<glm::vec4> pipeline(vector<glm::vec4> objectCoords)
{
	vector<glm::vec4> worldCoords = transformVertices(modelingTransformation, objectCoords);

	vector<glm::vec4> eyeCoords = transformVertices(viewingTransformation, worldCoords);

	vector<glm::vec4> projCoords = transformVertices(projectionTransformation, eyeCoords);

	vector<glm::vec4> clipCoords;

	// Perspective division
	for (glm::vec4 v : projCoords) {

		v = v / v.w;
		clipCoords.push_back(v);
	}

	// Clipping
	vector<glm::vec4> ndcCoords = clipCoords; // clip(clipCoords);

	vector<glm::vec4> windowCoords = transformVertices(viewportTransformation, ndcCoords);

	return windowCoords;

} // end pipeline



void drawBoard()
{
	drawManyFilledTriangles(pipeline(redPlaneVertices), color(1.0f, 0.0f, 0.0f, 1.0f));
	drawManyFilledTriangles(pipeline(greeenPlaneVertices), color(0.0f, 1.0f, 0.0f, 1.0f));

} // end drawBoard

/**
* Acts as the display function for the window.
*/
static void RenderSceneCB()
{
	vector<glm::vec4> transformedVertices;

	// Clear the color buffer
	colorBuffer.clearColorBuffer();

	static float angle = glm::radians(45.0f);
	
	angle += glm::radians(1.0f);

	static float angle1 = glm::radians(45.0f);

	angle1 -= glm::radians(1.0f);
	
	// Set modeling transformation for the center pyramid
	modelingTransformation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f))
		*glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
	transformedVertices = pipeline(pyramidVertices);
	drawManyFilledTriangles(transformedVertices, color(1.0f, 0.0f, 0.0f, 1.0f));

	// Set modeling transformation for the right pyramid
	modelingTransformation = glm::translate(glm::vec3(3.0f, 0.0f, 0.0f))
		*glm::rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f));
	transformedVertices = pipeline(pyramidVertices);
	drawManyFilledTriangles(transformedVertices, color(0.0f, 1.0f, 1.0f, 1.0f));

	// Set modeling transformation for the left pyramid
	modelingTransformation = glm::translate(glm::vec3(-3.0f, 0.0f, 0.0f))
		*glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f))
		*glm::scale(glm::vec3(2.0f,2.0f,1.0f));
	transformedVertices = pipeline(pyramidVertices);
	drawManyFilledTriangles(transformedVertices, color(0.824f, 0.412f, 0.118f,1.0f));

	// far out pyramid
	modelingTransformation =  glm::rotate(angle1, glm::vec3(0.0f, 1.0f, 0.0f))
		*glm::translate(glm::vec3(0.0f, 3.0f, -10.0f))
		*glm::rotate(angle1, glm::vec3(0.0f, 0.0f, 1.0f));
	transformedVertices = pipeline(pyramidVertices);
	drawManyFilledTriangles(transformedVertices, color(1.000f, 0.271f, 0.0f, 1.0f));
	
	// Set Modeling transformation for the reference plane
	modelingTransformation = glm::translate(glm::vec3(0.0f, -3.0f, 0.0f));
	drawBoard();

	//right back corner pyramid
	modelingTransformation = glm::translate(glm::vec3(2.85f, -2.0f, -0.5f));
	transformedVertices = pipeline(pyramidVertices);
	drawManyFilledTriangles(transformedVertices, color(0.118f, 0.565f, 1.000f, 1.0f));

	//left front corner pyramid
	modelingTransformation = glm::translate(glm::vec3(-2.85f, -2.0f, 4.75f));
	transformedVertices = pipeline(pyramidVertices);
	drawManyFilledTriangles(transformedVertices, color(0.118f, 0.565f, 1.000f, 1.0f));



	// Display the color buffer
	colorBuffer.showColorBuffer();

} // end RenderSceneCB

// Reset viewport limits for full window rendering each time the window is resized.
// This function is called when the program starts up and each time the window is 
// resized.
static void ResizeCB(int width, int height)
{
	// Size the color buffer to match the window size.
	colorBuffer.setColorBufferSize(width, height);

	// Set rendering window parameters for viewport transfomation
	xVpMin = 0;
	yVpMin = 0;
	xVpMax = (float)width;
	yVpMax = (float)height;

	// Create a perspective projection matrix. glm::perspective vertical field of view is specifed in degrees.
	projectionTransformation = glm::perspective(45.0f, ((float)xVpMax - xVpMin) / ((float)yVpMax - yVpMin), 0.1f, 100.0f);

	// Set viewport transformation
	viewportTransformation = 
		glm::translate(	glm::vec3(xVpMin, yVpMin, 0.0f)) * 
						glm::scale(glm::vec3((float)(xVpMax - xVpMin) / (xNdcMax - xNdcMin), (float)(yVpMax - yVpMin) / (yNdcMax - yNdcMin), 1.0f)) *
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
	default:
		cout << key << " key pressed." << endl;
	}
	glutPostRedisplay();

} // end KeyboardCB

// Responds to presses of the arrow keys
static void SpecialKeysCB(int key, int x, int y)
{
	switch (key) {

	case(GLUT_KEY_RIGHT) :

		break;
	case(GLUT_KEY_LEFT) :

		break;
	default:
		cout << key << " key pressed." << endl;
	}
	glutPostRedisplay();

} // end SpecialKeysCB

// Register as the "idle" function to have the screen continously
// repainted. Due to software rendering, the frame rate will not
// be fast enough to support motion simulation
static void animate()
{
	glutPostRedisplay();

} // end animate



int main(int argc, char** argv)
{
	// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.

	// ************* freeGlut and Window initialization ***********************

	// Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
	glutInit(&argc, argv);

	// Set the initial display mode.
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

	// Set the initial window size
	glutInitWindowSize(winWidth, winHeight);

	// Create a window using a string and make it the current window.
	GLuint world_Window = glutCreateWindow("Modeling Transformations");

	// Indicate to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Callback for window redisplay
	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);
	glutIdleFunc(animate);

	// Attach menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Request that the window be made full screen
	//glutFullScreenToggle();

	color aliceBlue(0.941f, 0.973f, 1.000f,1.0f);

	// Set red, green, blue, and alpha to which the color buffer is cleared.
	colorBuffer.setClearColor(aliceBlue);

	// ************* Object vertex coordinate initialization ***********************

	float height = 1.0f;
	float width = 1.0f;

	// Set vertex locations for a pyramid
	pyramidVertices.push_back( glm::vec4(0.0f, height / 2.0f, 0.0f, 1.0f) );
	pyramidVertices.push_back( glm::vec4(-width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));
	pyramidVertices.push_back (glm::vec4(width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));

	pyramidVertices.push_back(glm::vec4(0.0f, height / 2.0f, 0.0f, 1.0f));
	pyramidVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));
	pyramidVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f));

	pyramidVertices.push_back(glm::vec4(0.0f, height / 2.0f, 0.0f, 1.0f));
	pyramidVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f));
	pyramidVertices.push_back(glm::vec4(-width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f));

	pyramidVertices.push_back(glm::vec4(0.0f, height / 2.0f, 0.0f, 1.0f));
	pyramidVertices.push_back(glm::vec4(-width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));
	pyramidVertices.push_back(glm::vec4(-width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f));

	pyramidVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));
	pyramidVertices.push_back(glm::vec4(-width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));
	pyramidVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f));

	pyramidVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));
	pyramidVertices.push_back(glm::vec4(width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f));
	pyramidVertices.push_back(glm::vec4(-width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f));



	// Set vertex locations for a plane that is composed o four triangles.
	float planeWidth = 8.0f;

	// Red Triangles
	redPlaneVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	redPlaneVertices.push_back(glm::vec4(-planeWidth / 2.0f, 0.0f, -planeWidth / 2.0f, 1.0f));
	redPlaneVertices.push_back(glm::vec4(-planeWidth / 2.0f, 0.0f, planeWidth / 2.0f, 1.0f));
	redPlaneVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	redPlaneVertices.push_back(glm::vec4(planeWidth / 2.0f, 0.0f, planeWidth / 2.0f, 1.0f));
	redPlaneVertices.push_back(glm::vec4(planeWidth / 2.0f, 0.0f, -planeWidth / 2.0f, 1.0f));
	
	// Green Triangles
	greeenPlaneVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	greeenPlaneVertices.push_back(glm::vec4(planeWidth / 2.0f, 0.0f, -planeWidth / 2.0f, 1.0f));
	greeenPlaneVertices.push_back(glm::vec4(-planeWidth / 2.0f, 0.0f, -planeWidth / 2.0f, 1.0f));
	greeenPlaneVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	greeenPlaneVertices.push_back(glm::vec4(-planeWidth / 2.0f, 0.0f, planeWidth / 2.0, 1.0f));
	greeenPlaneVertices.push_back(glm::vec4(planeWidth / 2.0f, 0.0f, planeWidth / 2.0f, 1.0f));
	
	// Set the viewing tranformation for the scene
	viewingTransformation = glm::translate( glm::vec3(0.0f, 0.0f, -12.0) );

	// Enter the GLUT main loop. Control will not return until the window is closed.
	glutMainLoop();

	return 0;

} // end main
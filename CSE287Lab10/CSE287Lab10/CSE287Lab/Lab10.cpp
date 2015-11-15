#include "Lab10.h"

/**
* LAB INSTRUCTIONS:
*
* When you have completed all the problems, complete the following turning 
* instructions:
* 
*	1.	Copy the folder containing your solution to the desktop.
*	2.	Change the name of the folder to CSE287LabOne followed by your unique 
*		identifier. For instance “CSE287LabOneBachmaer.”
*	3.	Open the solution. Make sure it still runs.
*	4.	Clean the solution by selecting Build->Clean Solution. (The will delete 
*		all the intermediate temporary files that are part of your project and 
*		reduce the size of your submission.)
*	5.	Zip up the solution folder using the standard windows compression tool. 
*		(No 7zips, rars, etc.)
*	6.	Submit your zip archive of the solution through canvas.

*/

/********************** GLOBALS ******************************/

glm::mat4 modelingTransformation;
glm::mat4 viewingTransformation;
glm::mat4 projectionTransformation;
glm::mat4 viewportTransformation;

// View port limits
float xViewportMin, yViewportMin, xViewportMax, yViewportMax;

// Normalized device coordinate limits
int xNdcMin = -1, yNdcMin = -1, xNdcMax = 1, yNdcMax = 1;

ColorBuffer colorBuffer(winWidth, winHeight);

vector<glm::vec4> triangleVertices;

ReferencePlane refPlane;
Pyramid pyramid;
Sphere sphere(0.5f ,16, 16);

float rotationX = 0;
float rotationY = 0;
float zTrans = -12;


float angle = glm::radians(45.0f);

enum View_Type{FULL_SCREEN, VERTICAL_SPLIT, HORIZONTAL_SPLIT};

View_Type view = FULL_SCREEN;



// Applies a transformation in the form of a matrix to a list of vertices.
vector<glm::vec4> transformVertices(const glm::mat4 & transMatrix, const vector<glm::vec4>  & vertices)
{
	vector<glm::vec4> transformedVertices;

	for (unsigned int i = 0; i < vertices.size(); i++) {

		glm::vec4 vt(transMatrix * vertices[i]);

		transformedVertices.push_back(vt);
	}

	return transformedVertices;

} // end transformVertices


// Tranforms vertices from world to window coordinates, via world, eye, clip, and normalized device coordinates.
vector<glm::vec4> pipeline(const vector<glm::vec4> & objectCoords)
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
	sphere.draw(color(1.0f, 1.0f, 0.0f, 1.0f));

	// Set modeling transformation for the left hand pyramid
	modelingTransformation = glm::translate(glm::vec3(-3.0f, 0.0f, 0.0f)) * glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
	pyramid.draw(color(1.0f, 0.0f, 1.0f, 1.0f));

}

// Draws two views of the scene. One on the right and one on the left.
void twoViewsSplitVertically()
{

	float viewportWidth = (xViewportMax - xViewportMin)/2.0f;
	float viewportHeight = (yViewportMax - yViewportMin);

	projectionTransformation = glm::perspective(45.0f, ((float)viewportWidth) / ((float)viewportHeight), 0.1f, 100.0f);

	// Set viewport transformation
	viewportTransformation =
		glm::translate(glm::vec3(0, 0, 0.0f)) *
		glm::scale(glm::vec3((float)(viewportWidth) / (xNdcMax - xNdcMin), (float)(viewportHeight) / (yNdcMax - yNdcMin), 1.0f)) *
		glm::translate(glm::vec3(-xNdcMin, -yNdcMin, 0.0f));

	renderObjects();

	// TODO


} // end twoViewsSplitVertically

// Draws two views of the scene. One on the top and one on the bottom.
void twoViewsSplitHorizontally()
{

	// TODO

} // end twoViewsSplitHorizontally



/**
* Acts as the display function for the window.
*/
static void RenderSceneCB()
{
	vector<glm::vec4> transformedVertices;

	// Clear the color buffer
	colorBuffer.clearColorBuffer();

	angle += glm::radians(1.0f);

	switch (view) {

		case VERTICAL_SPLIT:

			twoViewsSplitVertically();
			break;

		case HORIZONTAL_SPLIT: 

			twoViewsSplitHorizontally();
			break;

		default:
			renderObjects();
	}

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
	xViewportMin = 0;
	yViewportMin = 0;
	xViewportMax = (float)width;
	yViewportMax = (float)height;

	// Create a perspective projection matrix. glm::perspective vertical field of view is specifed in degrees.
	projectionTransformation = glm::perspective(45.0f, ((float)xViewportMax - xViewportMin) / ((float)yViewportMax - yViewportMin), 0.1f, 100.0f);

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
	default:
		cout << key << " key pressed." << endl;
	}


	glutPostRedisplay();

} // end KeyboardCB



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
	case(1) :

		view = FULL_SCREEN;

		ResizeCB((int)xViewportMax, (int)yViewportMax);

		break;

	case(2) :

		view = VERTICAL_SPLIT;
		cout << "vertical" << endl;
		break;

	case(3) :

		view = HORIZONTAL_SPLIT;
		
		cout << "horizontal" << endl;

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
	colorBuffer.setClearColor(blue);
	
	// Set the initial viewing tranformation for the scene
	viewingTransformation = glm::translate( glm::vec3(0.0f, 0.0f, -20.0) );

	// Enter the GLUT main loop. Control will not return until the window is closed.
	glutMainLoop();

	return 0;

} // end main
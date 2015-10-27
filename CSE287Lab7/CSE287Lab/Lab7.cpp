#include "Lab7.h"

void print(const mat2 & m)
{
	std::cout << "\n";

	for (int row = 0; row < 2; row++) {
		std::cout << "|\t";
		for (int col = 0; col < 2; col++) {
			std::cout << m[col][row] << "\t";
		}
		std::cout << "|\n";
	}
	std::cout << "\n";

}

void print(const mat3 & m)
{
	std::cout << "\n";
	for (int row = 0; row < 3; row++) {
		{
			std::cout << "|\t";
			for (int col = 0; col < 3; col++) {
				std::cout << m[col][row] << "\t";
			}
		}
		std::cout << "|\n";
	}
	std::cout << "\n";
}

void print(const mat4 & m)
{
	std::cout << "\n";

	for (int row = 0; row < 3; row++) {
		std::cout << "|\t";
		for (int col = 0; col < 4; col++) {
			std::cout << m[col][row] << "\t";
		}
		std::cout << "|\n";
	}
	std::cout << "\n";

}

ColorBuffer colorBuffer(winWidth, winHeight);

vector<vec3> triangleVertices;

vector<vec3> verticalLineVertices;
vector<vec3> horizontalLineVertices;

/**
* LAB2 INSTRUCTIONS:
* Below are description of two dimensional transformation problems. With the 
* exception of problem one, each problem require you to tranform the vertices
* of a triangle and render it.
*
* When you have completed all the problems, complete the following turning 
* instructions:
* 
*	1.	Copy the folder containing your solution to the desktop.
*	2.	Change the name of the folder to CSE287LabSeven followed by your unique 
*		identifier. For instance “CSE287LabSevenBachmaer.”
*	3.	Open the solution. Make sure it still runs.
*	4.	Clean the solution by selecting Build->Clean Solution. (The will delete 
*		all the intermediate temporary files that are part of your project and 
*		reduce the size of your submission.)
*	5.	Zip up the solution folder using the standard windows compression tool. 
*		(No 7zips, rars, etc.)
*	6.	Submit your zip archive of the solution through canvas.

*/

// Index of the problem currently being rendered.
int displayedProblem = 1;

// Use the linear algebra functionality of GLM to solve the following system of 
// linear equations. Print the solution vector to the console. 
//
// 3x + 7y + 2z = 4
// 2x - 4y - 3z = -1
// 5x + 2y + z = 1
void problem1()
{
	mat3 a = mat3(vec3(3,7,2), vec3(2,-4,-3), vec3(5,2,1));
	a = inverse(a);
	vec3 b = vec3(4,-1,1);
	vec3 c = a*b;

	cout << "[" << c.x
		<< " " << c.y
		<< " " << c.z
		<< "]" << endl;


} // end Problem1


// Translate 300 250
void problem2()
{
	mat3 trans(1.0f);
	trans[2][0] = 300;
	trans[2][1] = 250;
	vector<vec3> transformedVert;

	for (int i = 0; i < (int)triangleVertices.size(); i++) {

		transformedVert.push_back( trans* triangleVertices[i]);
	}

	drawFilledTriangle(transformedVert, color(1.0f, 0.0f, 0.0f, 1.0f));

} // end Problem2

// Use a scale transformation to shrink the triangle to three quarters the 
// original size.
void problem3()
{
	mat3 matrix(1.0f);
	matrix[0][0] = 0.75;
	matrix[1][1] = 0.75;
	vector<vec3> transformedVert;

	for (int i = 0; i < (int)triangleVertices.size(); i++) {

		transformedVert.push_back(matrix* triangleVertices[i]);
	}

	drawFilledTriangle(transformedVert, color(1.0f, 0.0f, 0.0f, 1.0f));



} // end Problem3

// Use a scale transformation to reflect the triangle accross the x axis.
void problem4()
{

	mat3 matrix(1.0f);
	matrix[0][0] = 0.75;
	matrix[1][1] = -0.75;
	vector<vec3> transformedVert;

	for (int i = 0; i < (int)triangleVertices.size(); i++) {

		transformedVert.push_back(matrix* triangleVertices[i]);
	}

	drawFilledTriangle(transformedVert, color(1.0f, 0.0f, 0.0f, 1.0f));



} // end Problem4



// Shear the triangle by a factor of 3 in the horizontal direction
void problem5()
{

	mat3 matrix(1.0f);
	matrix[0][1] = 3;
	vector<vec3> transformedVert;

	for (int i = 0; i < (int)triangleVertices.size(); i++) {

		transformedVert.push_back(matrix* triangleVertices[i]);
	}

	drawFilledTriangle(transformedVert, color(1.0f, 0.0f, 0.0f, 1.0f));




} // end Problem5

// Rotate the triangle by negative 90 degrees.
void problem6()
{
	
	mat3 matrix(1.0f);

	cout << glm::cos(glm::degrees(90.0f)) << endl;
	cout << glm::sin(glm::degrees(90.0f)) << endl;
	cout << -glm::sin(glm::degrees(90.0f)) << endl;
	cout << glm::cos(glm::degrees(90.0f)) << endl;
	
	matrix[0][0] = glm::round(glm::cos(glm::degrees(90.0f)));
	matrix[0][1] = glm::round(-glm::sin(glm::degrees(90.0f)));
	matrix[1][0] = glm::round(glm::sin(glm::degrees(90.0f)));
	matrix[1][1] = glm::round(glm::cos(glm::degrees(90.0f)));
	
	vector<vec3> transformedVert;
	

	for (int i = 0; i < (int)triangleVertices.size(); i++) {

		transformedVert.push_back(matrix* triangleVertices[i]);
	}

	drawFilledTriangle(transformedVert, color(1.0f, 0.0f, 0.0f, 1.0f));

} // end Problem6


// Create a composite transformation that translates the triangle 200 in the
// horizontal direction 300 in the vertical direction and rotates it at that
// position by 180 degrees.
void problem7()
{



} // end Problem7

// Translate to the same position as the previous question. Instead of a 
// fixed rotation. Have the triangle continously rotate in place in a
// counter clockwise direction.
void problem8()
{
	static float angle = 0.0f;



} // end Problem8


// Implement and animation that makes the triangle continuously orbit in a clockwise
// direction around the origin at a distance of 400 pixels.
void problem9()
{


} // end Problem9



// Accomplish the same orbit as the previous question with the exception that
// the triagle always points at the top of the screen while it orbits.
void problem10() {



} // end Problem10


// Create and animation that causes the triangle to continously bounce from one 
// side of the screen to the other. You can get the width and/or height of the
// screen in pixels by using glutGet( GLUT_WINDOW_WIDTH ) and 
// glutGet( GLUT_WINDOW_HEIGHT ).
void problem11()
{



} // end Problem11


/**
* Acts as the display function for the window.
*/
static void RenderSceneCB()
{
	// Clear the color buffer
	colorBuffer.clearColorBuffer();

	// Drawn the reference lines
	drawLine(horizontalLineVertices, color(1.0f, 0.0f, 0.0f, 1.0f));
	drawLine(verticalLineVertices, color(0.0f, 1.0f, 0.0f, 1.0f));

	switch (displayedProblem) {

		case 1:
			problem1();
			break;
		case 2:
			problem2();
			break;
		case 3:
			problem3();
			break;
		case 4:
			problem4();
			break;
		case 5:
			problem5();
			break;
		case 6:
			problem6();
			break;
		case 7:
			problem7();
			break;
		case 8:
			problem8();
			break;
		case 9:
			problem9();
			break;
		case 10:
			problem10();
			break;
		case 11:
			problem11();
			break;
		default:
			cout << "Invalid displayProblem" << endl;
	}
	
	drawWireTriangle(triangleVertices, color(0.0f, 0.0f, 0.0f, 1.0f));

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

	verticalLineVertices.clear();

	verticalLineVertices.push_back(vec3(width / 2, 0.0f, 1.0f));
	verticalLineVertices.push_back(vec3(width / 2, height, 1.0f));

	horizontalLineVertices.clear();
	horizontalLineVertices.push_back(vec3(0.0f, height / 2, 1.0f));
	horizontalLineVertices.push_back(vec3(width, height / 2, 1.0f));

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

void problemMenu(int value)
{

	if (value != 0) {

		displayedProblem = value;
	}
	else {
		glutLeaveMainLoop();
	}
	

	// Signal GLUT to call display callback
	glutPostRedisplay();
} // end subMenu1


int main(int argc, char** argv)
{
	// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.

	// freeGlut and Window initialization ***********************

	// Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
	glutInit(&argc, argv);

	// Set the initial display mode.
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

	// Set the initial window size
	glutInitWindowSize(winWidth, winHeight);

	// Create a window using a string and make it the current window.
	GLuint world_Window = glutCreateWindow("Lab 7: Simple Transformations");

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
	int menu1id = glutCreateMenu(problemMenu);
	// Specify menu items and integer identifiers
	glutAddMenuEntry("Problem 1", 1);
	glutAddMenuEntry("Problem 2", 2);
	glutAddMenuEntry("Problem 3", 3);
	glutAddMenuEntry("Problem 4", 4);
	glutAddMenuEntry("Problem 5", 5);
	glutAddMenuEntry("Problem 6", 6);
	glutAddMenuEntry("Problem 7", 7);
	glutAddMenuEntry("Problem 8", 8);
	glutAddMenuEntry("Problem 9", 9);
	glutAddMenuEntry("Problem 10", 10);
	glutAddMenuEntry("Problem 11", 11);
	glutAddMenuEntry("Quit", 0);

	// Attach menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Request that the window be made full screen
	//glutFullScreenToggle();

	color blue(0.784, 0.784, 1.0, 1.0);

	// Set red, green, blue, and alpha to which the color buffer is cleared.
	colorBuffer.setClearColor(blue);

	// Set vertex locations for one triangle
	triangleVertices.push_back(vec3(-glm::cos(radians(45.0f)) * 100.0f, -glm::cos(radians(45.0f)) * 100.0f, 1.0f));
	triangleVertices.push_back(vec3(glm::cos(radians(45.0f)) * 100.0f, -glm::cos(radians(45.0f)) * 100.0f, 1.0f));
	triangleVertices.push_back(vec3(0.0f, 100.0f, 1.0f));

	// Enter the GLUT main loop. Control will not return until the window is closed.
	glutMainLoop();

	// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.

	return 0;

} // end main
#include "FrameBuffer.h"

/**
* Constructor. Allocates memory for storing pixel values.
*/
FrameBuffer::FrameBuffer( const int width, const int height)
{
	setFrameBufferSize(width, height);

} // end FrameBuffer constructor


/**
* Deallocates dynamically memory associated with the class.
*/
FrameBuffer::~FrameBuffer(void)
{
	// Free the memory associated with the color buffer
	delete [] colorBuffer;
	delete[] depthBuffer;

} // end FrameBuffer destructor


/**
* Sizes the color buffer to match the window size. Deallocates any
* memory that was previsouly allocated.
*/
void FrameBuffer::setFrameBufferSize( const int width, const int height) {

	// Save the dimensions of the window
	window.width = width;
    window.height = height;

	// Set pixel storage modes. 
	// (https://www.opengl.org/archives/resources/features/KilgardTechniques/oglpitfall/)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Free the memory previously associated with the color buffer
	delete [] colorBuffer;
	delete[] depthBuffer;

	// Allocate the color buffer to match the size of the window
	colorBuffer = new GLubyte[width*BYTES_PER_PIXEL*height];
	depthBuffer = new float[width*height];

} // end setFrameBufferSize


/**
* Sets the color to which the window will be cleared. Does NOT
* actually clear the window
*/
void FrameBuffer::setClearColor( const color & clear ) {

	clearColor[0] = (GLubyte)(clear.r * 255);
	clearColor[1] =	(GLubyte)(clear.g * 255);
	clearColor[2] =	(GLubyte)(clear.b * 255);
	clearColor[3] =	(GLubyte)(clear.a * 255);

} // end setClearColor


/**
* Clears the window to the clear color.
*/
void FrameBuffer::clearColorAndDepthBuffers() {

	for(int y = 0; y < window.height ; ++y) {
		for(int x = 0; x < window.width; ++x) {

			std::memcpy( colorBuffer + BYTES_PER_PIXEL * (x + y * window.width ), 
						 clearColor,  BYTES_PER_PIXEL );
			depthBuffer[y * window.width + x] = 1.0f;
		
		}
	}

} // end clearFrameBuffer


/**
* Copies memory into frame buffer and updates the window
* using an OpenGL command.
*/
void FrameBuffer::showColorBuffer()
{
	// Insure raster position is lower left hand corner of the window.
	glRasterPos2d(-1, -1);

	// Copy color buffer to raster (OpenGL command)
	glDrawPixels( window.width, window.height, 
				  GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer );
	
	// Flush all drawing commands and swapbuffers
	glutSwapBuffers();

} // end showFrameBuffer


/**
* Sets an individual pixel value in the color buffer. Origin (0,0)
* is the lower left hand corner of the window.
*/
void FrameBuffer::setPixel(const int x, const int y, const color & rgba) {

	GLubyte c[] = { (GLubyte)(rgba.r * 255),
		(GLubyte)(rgba.g * 255),
		(GLubyte)(rgba.b * 255),
		(GLubyte)(rgba.a * 255) };

	setPixel(x, y, c);

} // end setPixel

/**
* Sets an individual pixel value in the color buffer. Origin (0,0)
* is the lower left hand corner of the window.
*/
void FrameBuffer::setPixel(const float x, const float y, const color & rgba) {

	GLubyte c[] = { (GLubyte)(rgba.r * 255),
		(GLubyte)(rgba.g * 255),
		(GLubyte)(rgba.b * 255),
		(GLubyte)(rgba.a * 255) };

	setPixel((int)(x + 0.5f), (int)(y + 0.5f), rgba);

} // end setPixel

/**
* Sets an individual pixel value in the color buffer. Origin (0,0)
* is the lower left hand corner of the window.
*/
void FrameBuffer::setPixel(const int x, const int y, const GLubyte rgba[]) {

	if (x >= 0 && x < window.width && y >= 0 && y < window.height) {

		// TODO

		// Write to the color buffer
		// Depth Test Goes in here
		std::memcpy(colorBuffer + BYTES_PER_PIXEL * (x + y * window.width), rgba, BYTES_PER_PIXEL);

	}

} // end setPixel


void FrameBuffer::setV0V1V2(const VertexData & zero, const VertexData & one, const VertexData & two)
{
	// Store the vertex attributes for interpolation accross the face of the trianble
	v0 = zero;
	v1 = one;
	v2 = two;

	// Find and store a normal vector for the polygon
	glm::vec3 dif1 = v1.position.xyz - v0.position.xyz;
	glm::vec3 dif2 = v2.position.xyz - v0.position.xyz;

	triangleNormal = glm::normalize(glm::cross(dif1, dif2));

	// Find and stor the dot product of the 
	glm::vec3 a = v0.position.xyz;
	normalPointDot = -glm::dot(a, triangleNormal);

} // end setV1V2V3


// Finds the interpolated depth coordinate based on the depths of the corners of the triangle
float FrameBuffer::findInterpolatedDepthValue(float x, float y)
{
	// TODO on slides find z

	return 0; 

} // end findInterpolatedDepthValue


void FrameBuffer::setDepth(const float x, const float y, const float depth) {

	setDepth((int)(x + 0.5f), (int)(y + 0.5f), depth);
}

void FrameBuffer::setDepth(const int x, const int y, const float depth) {

	depthBuffer[y * window.width + x] = depth;
}

float FrameBuffer::getDepth(const int x, const int y) {

	return depthBuffer[y * window.width + x];
}


float FrameBuffer::getDepth(const float x, const float y) {

	return getDepth((int)(x + 0.5f), (int)(y + 0.5f));
}

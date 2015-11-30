#pragma once

#include "BasicIncludesAndDefines.h"

#include "VertexData.h"


/**
* Declarations for functions that acte as "call backs" for various
* events. FreeGLUT commands are used to register these functions
* as event handlers.
*/


/* 
* Preprocessor statement for text substitution
*/
#define winWidth 700
#define winHeight 500

/**
* Acts as the display function for the window. It is called every
* the window needs to be repainted.
*/
static void RenderSceneCB();

/**
* Resets the graphics viewport limits for full window rendering 
* each time the window is resized.
* 
* @param width of the window in pixels
* @param height of the window in pixels
*/
static void ResizeCB(int width, int height);

/**
* If registered as the "idle" function, this method will continously as the
* OS to repaint the window.
*/
static void animate();

// Tranforms vertices from world to window coordinates, via world, eye, clip, and normalized device coordinates.
vector<VertexData> pipeline(const vector<VertexData> & objectCoords);

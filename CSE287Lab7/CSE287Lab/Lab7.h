#pragma once

#include "BasicIncludesAndDefines.h"

/**
* Declarations for functions that acte as "call backs" for various
* events. FreeGLUT commands are used to register these functions
* as event handlers.
*/


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

// Responds to 'f' and escape keys. 'f' key allows 
// toggling full screen viewing. Escape key ends the
// program. Allows lights to be individually turned on and off.
static void KeyboardCB(unsigned char key, int x, int y);

// Responds to presses of the arrow keys
static void SpecialKeysCB(int key, int x, int y);

// Register as the "idle" function to have the screen continously
// repainted.
static void animate();

// Callback function for menu selections.
void problemMenu(int value);

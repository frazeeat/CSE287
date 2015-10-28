#pragma once

/*
* Preprocessor statement for text substitution
*/
#define winWidth 700
#define winHeight 500

#include <iostream>
#include <vector>

using namespace std;

#define GLM_SWIZZLE

#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/constants.hpp>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/constants.hpp>

using namespace glm;

#define color vec4
#define vecX vec3

#include "ColorBuffer.h"
#include "ScanConversion.h"

//#include "HitRecord.h"

#define EPSILON 1.0E-4f


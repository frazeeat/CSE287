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

#define GLM_FORCE_RADIANS

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

//using namespace glm;

#define color glm::vec4

#include "ColorBuffer.h"
#include "ScanConversion.h"
#include "ShapeFunctions.h"

//#include "HitRecord.h"

#define EPSILON 1.0E-4f

color getRandomColor();

void print(const glm::vec2 & v0);

void print(const glm::vec3 & v0);

void print(const glm::vec4 & v0);

void print(const glm::mat2 & m);

void print(const glm::mat3 & m);

void print(const glm::mat4 & m);


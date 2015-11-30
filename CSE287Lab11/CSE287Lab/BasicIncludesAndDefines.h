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

#define color glm::vec4

#define EPSILON 1.0E-4f

struct VertexData {

	glm::vec4 position;
	color material;
	glm::vec4 normal;

	VertexData(const glm::vec4 & pos = glm::vec4(0.0, 0.0, 0.0, 0.0),
		const color & col = color(0.75, 0.75, 0.75, 1.0),
		const glm::vec4 & norm = glm::vec4(0.0, 0.0, 1.0, 0.0))
	{
		position = pos;
		material = col;
		normal = glm::normalize(norm);
	}

};


color getRandomColor();

void print(const glm::vec2 & v0);

void print(const glm::vec3 & v0);

void print(const glm::vec4 & v0);

void print(const glm::mat2 & m);

void print(const glm::mat3 & m);

void print(const glm::mat4 & m);

#include "FrameBuffer.h"
#include "ScanConversion.h"
#include "ShapeFunctions.h"

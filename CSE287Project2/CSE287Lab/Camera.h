#pragma once
#include "BasicIncludesAndDefines.h"
struct Camera
{
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, -12.0f),
		glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	~Camera();
	void setPositionDirectionUp(glm::vec3 position,
		glm::vec3 direction,
		glm::vec3 up);
	glm::vec3 getWorldCoordinateViewPosition();
	glm::mat4 getViewingTransformation();
	glm::mat4 viewTrans;
}; 
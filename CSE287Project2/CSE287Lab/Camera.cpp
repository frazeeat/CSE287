#include "Camera.h"

Camera::Camera(glm::vec3 position,glm::vec3 direction,glm::vec3 up){
	viewTrans = glm::lookAt(position,direction , up);
}
Camera::~Camera(){

}
void Camera::setPositionDirectionUp(glm::vec3 position,
	glm::vec3 direction,
	glm::vec3 up){
	viewTrans = glm::lookAt(position, direction, up);

}
glm::vec3 Camera::getWorldCoordinateViewPosition(){
	return glm::vec3(0, 0, 0);
}
glm::mat4 Camera::getViewingTransformation(){
	return viewTrans;
}
/**
*	@file CameraController.cpp
*/
#include "CameraController.h"

void DefaultCamera::CalculateMatrix(){

	glm::vec3 forward(0, 0, 1);

	matView = glm::lookAt(position, rotation * forward, glm::vec3(0, 1, 0));
	matProj = glm::perspective(fov, aspect, near, far);
}

#include "Camera.h"
#include <gtx\transform.hpp>


Camera::Camera() : direction(0.0f, 0.0f, -1.0f)
{

}

void Camera::mouseUpdate(const glm::vec2 & mousePos)
{
	glm::vec2 delta = mousePos - oldMousePos;

	//sanity check
	if (glm::length(delta) > 50.0f)
	{
		oldMousePos = mousePos;
		return;
	}

	const float mouseSense = 0.005f;

	direction = glm::mat3(glm::rotate(-delta.x*mouseSense, glm::vec3(0.0f, 1.0f, 0.0f))) * direction;

	glm::vec3 RotateAround = glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f));
	direction = glm::mat3(glm::rotate(-delta.y*mouseSense, RotateAround)) * direction;

	oldMousePos = mousePos;
}

void Camera::moveUp()
{
	pos += speed * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::moveDown()
{
	pos += -speed * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::moveLeft()
{
	glm::vec3 strafe = glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f));
	pos += -speed * strafe;
}

void Camera::moveRight()
{
	glm::vec3 strafe = glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f));
	pos += speed * strafe;
}

void Camera::moveForward()
{
	pos += speed * direction;
}

void Camera::moveBackward()
{
	pos += -speed * direction;
}


glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(pos, pos + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}


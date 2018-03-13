#pragma once
#include <glm.hpp>


class Camera
{
private:
	glm::vec3 pos;
	glm::vec3 direction;
	glm::vec2 oldMousePos;
	const float speed = 0.2f;
public:
	Camera();

	void mouseUpdate(const glm::vec2& mousePos);

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void moveForward();
	void moveBackward();


	glm::mat4 getWorldToViewMatrix() const;
};


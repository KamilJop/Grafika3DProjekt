#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>



class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;

	bool isFlashlightOn = false;

	Camera(glm::vec3 Position, glm::vec3 Up, float yaw, float pitch, float speed, float sensitivity);

	glm::mat4 getViewMatrix();

	glm::vec3 getCameraPosition() { return Position; }
	glm::vec3 getCameraFront() { return Front; }
	glm::vec3 getCameraRight() { return Right; }
	glm::vec3 getCameraUp() { return Up; }

	bool getFlashlightState() { return isFlashlightOn; }
	void setFlashlightState(bool state) { isFlashlightOn = state; }

	void ProcessKeyboard(bool * keys, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	

private:
	void updateCameraVectors();
};


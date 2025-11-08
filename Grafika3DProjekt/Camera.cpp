#include "Camera.h"


// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float speed, float sensitivity)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	MovementSpeed = speed;
	MouseSensitivity = sensitivity;
	updateCameraVectors();
}

// Method calculating fron right and up camera vecotrs
void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

// Calculate and return the view matrix
glm::mat4 Camera::getViewMatrix()
{
	return	glm::lookAt(Position, Position + Front, Up);
}

// Change the position vector based on keyboard input
void Camera::ProcessKeyboard(bool* keys, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (keys[GLFW_KEY_W] == GLFW_PRESS)
		Position += Front * velocity;
	if (keys[GLFW_KEY_S] == GLFW_PRESS)
		Position -= Front * velocity;
	if (keys[GLFW_KEY_A] == GLFW_PRESS)
		Position -= Right * velocity;
	if (keys[GLFW_KEY_D] == GLFW_PRESS)
		Position += Right * velocity;
	if (keys[GLFW_KEY_F] == GLFW_PRESS)
	{
		isFlashlightOn = !isFlashlightOn;
		keys[GLFW_KEY_F] = GLFW_RELEASE; 
	}
		
	if (Position.y < 0.0f) Position.y = 0.0f;
	if (Position.y > 2.0f) Position.y = 2.0f;
	
}

// Change the yaw and pitch based on mouse movement
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;
	Yaw += xoffset;
	Pitch += yoffset;
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	updateCameraVectors();
}


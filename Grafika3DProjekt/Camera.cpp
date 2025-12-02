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


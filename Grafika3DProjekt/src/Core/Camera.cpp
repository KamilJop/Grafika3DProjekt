#include "Camera.h"

/**
 * @brief Constructor for the Camera class.
 * 
 * Initializes the camera with given position, up vector, yaw, pitch, movement speed,
 * and mouse sensitivity. Also calculates the initial camera vectors.
 * 
 * @param position Initial position of the camera in world space.
 * @param up World up vector (usually {0,1,0}).
 * @param yaw Initial yaw angle (rotation around Y-axis in degrees).
 * @param pitch Initial pitch angle (rotation around X-axis in degrees).
 * @param speed Movement speed of the camera.
 * @param sensitivity Mouse sensitivity for looking around.
 */
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

/**
 * @brief Updates the camera's Front, Right, and Up vectors based on the current yaw and pitch.
 * 
 * This is called internally whenever the camera orientation changes (e.g., after mouse movement).
 */
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

/**
 * @brief Calculates and returns the view matrix for the camera.
 * 
 * Uses glm::lookAt with the camera's position, front vector, and up vector.
 * 
 * @return glm::mat4 The view matrix.
 */
glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

/**
 * @brief Processes mouse movement to adjust the camera's yaw and pitch.
 * 
 * Updates the camera orientation based on x and y mouse offsets. Optionally constrains
 * the pitch to prevent the camera from flipping.
 * 
 * @param xoffset Mouse movement in the X direction.
 * @param yoffset Mouse movement in the Y direction.
 * @param constrainPitch Whether to limit pitch to [-89, 89] degrees.
 */
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

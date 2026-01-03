#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

/**
 * @brief Represents a 3D camera for use in OpenGL.
 * 
 * Handles camera position, orientation (yaw/pitch), movement, and view matrix calculation.
 * Supports mouse-controlled rotation and movement speed adjustment.
 */
class Camera
{
public:
    /**
     * @brief Constructs a Camera object.
     * 
     * Initializes the camera's position, orientation, speed, and mouse sensitivity.
     * Automatically calculates initial Front, Right, and Up vectors.
     * 
     * @param Position Initial position in world space.
     * @param Up World up vector (usually {0,1,0}).
     * @param yaw Initial yaw angle (rotation around Y-axis in degrees).
     * @param pitch Initial pitch angle (rotation around X-axis in degrees).
     * @param speed Movement speed of the camera.
     * @param sensitivity Mouse sensitivity for rotation.
     */
    Camera(glm::vec3 Position, glm::vec3 Up, float yaw, float pitch, float speed, float sensitivity);

    /**
     * @brief Returns the view matrix calculated using glm::lookAt.
     * 
     * @return glm::mat4 The view matrix representing the camera's position and orientation.
     */
    glm::mat4 getViewMatrix();

    /**
     * @brief Returns the camera's world position.
     */
    glm::vec3 getCameraPosition() { return Position; }

    /**
     * @brief Returns the camera's front (looking) direction.
     */
    glm::vec3 getCameraFront() { return Front; }

    /**
     * @brief Returns the camera's right vector.
     */
    glm::vec3 getCameraRight() { return Right; }

    /**
     * @brief Returns the camera's up vector.
     */
    glm::vec3 getCameraUp() { return Up; }

    /**
     * @brief Processes mouse movement to update yaw and pitch.
     * 
     * @param xoffset Mouse movement along the X-axis.
     * @param yoffset Mouse movement along the Y-axis.
     * @param constrainPitch If true, constrains pitch to [-89, 89] degrees to avoid gimbal lock.
     */
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    /**
     * @brief Returns the current pitch angle of the camera.
     */
    float getPitch() { return Pitch; }

    /**
     * @brief Returns the current yaw angle of the camera.
     */
    float getYaw() { return Yaw; }

    /**
     * @brief Returns the movement speed of the camera.
     */
    float getMovementSpeed() { return MovementSpeed; }

    /**
     * @brief Sets the camera's world position.
     * 
     * Automatically updates camera vectors after changing position.
     * 
     * @param pos New position vector.
     */
    void setPosition(glm::vec3 pos) { Position = pos; updateCameraVectors(); }

    /**
     * @brief Sets the camera's pitch angle.
     * 
     * Automatically updates camera vectors after changing pitch.
     * 
     * @param pitch New pitch angle in degrees.
     */
    void setPitch(float pitch) { Pitch = pitch; updateCameraVectors(); }

    /**
     * @brief Sets the camera's yaw angle.
     * 
     * Automatically updates camera vectors after changing yaw.
     * 
     * @param yaw New yaw angle in degrees.
     */
    void setYaw(float yaw) { Yaw = yaw; updateCameraVectors(); }

    /**
     * @brief Updates the camera's Front, Right, and Up vectors based on current yaw and pitch.
     * 
     * This should be called whenever the camera orientation changes.
     */
    void updateCameraVectors();

private:
    /** @brief Camera position in world space. */
    glm::vec3 Position;

    /** @brief Camera front vector (direction it's looking at). */
    glm::vec3 Front;

    /** @brief Camera up vector. */
    glm::vec3 Up;

    /** @brief Camera right vector. */
    glm::vec3 Right;

    /** @brief World up vector. */
    glm::vec3 WorldUp;

    /** @brief Yaw angle (rotation around Y-axis). */
    float Yaw;

    /** @brief Pitch angle (rotation around X-axis). */
    float Pitch;

    /** @brief Movement speed of the camera. */
    float MovementSpeed;

    /** @brief Mouse sensitivity for camera rotation. */
    float MouseSensitivity;
};

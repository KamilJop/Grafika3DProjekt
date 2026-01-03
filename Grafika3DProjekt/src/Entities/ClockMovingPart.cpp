#include "ClockMovingPart.h"

/**
 * @brief Updates the rotation of the clock’s moving part.
 *
 * This function oscillates the rotation of the clock component between
 * a minimum and maximum angle, creating a ticking or swinging motion.
 *
 * Behavior:
 * - If `direction` is true, the part rotates forward.
 * - If the rotation exceeds `maximumAngle`, direction reverses.
 * - If `direction` is false, the part rotates backward.
 * - If the rotation goes below `minimumAngle`, direction reverses.
 *
 * @param deltaTime Time elapsed since last frame.
 */
void ClockMovingPart::Update(float deltaTime)
{
    if (direction)
    {
        // Rotate forward
        setRotation(getRotation() + glm::vec3(0.0f, 0.0f, rotationSpeed * deltaTime));

        // Reverse direction at maximum angle
        if (getRotation().z >= maximumAngle)
        {
            direction = false;
        }
    }
    else
    {
        // Rotate backward
        setRotation(getRotation() - glm::vec3(0.0f, 0.0f, rotationSpeed * deltaTime));

        // Reverse direction at minimum angle
        if (getRotation().z <= minimumAngle)
        {
            direction = true;
        }
    }
}

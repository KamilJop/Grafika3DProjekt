#pragma once
#include "Entity.h"

/**
 * @class ClockMovingPart
 * @brief Represents an oscillating moving part of a clock (e.g., pendulum or hand).
 *
 * The ClockMovingPart class extends Entity and provides:
 * - Automatic back‑and‑forth rotation between two angles
 * - Adjustable rotation speed and limits
 * - Simple oscillation logic for decorative or puzzle elements
 */
class ClockMovingPart : public Entity
{
public:

    /**
     * @brief Constructs a new ClockMovingPart entity.
     *
     * @param model Pointer to the 3D model.
     * @param pos World position.
     * @param rot Initial rotation vector.
     * @param scal Scale vector.
     * @param interaction Whether the part is interactable (usually false).
     */
    ClockMovingPart(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
                    bool interaction = false)
        : Entity(model, pos, rot, scal, interaction)
    {
    }

    /**
     * @brief Updates the clock part each frame.
     *
     * Oscillates the rotation between @ref minimumAngle and @ref maximumAngle
     * using @ref rotationSpeed.  
     * Creates a smooth ticking or swinging motion.
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override;

private:

    /** @brief Current rotation direction (true = forward, false = backward). */
    bool direction = true;

    /** @brief Speed of rotation in degrees per second. */
    float rotationSpeed = 25.0f;

    /** @brief Minimum rotation angle (degrees). */
    float minimumAngle = -10.0f;

    /** @brief Maximum rotation angle (degrees). */
    float maximumAngle = 30.0f;
};

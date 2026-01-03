#pragma once
#include "Entity.h"
#include "../Light/PointLight.h"

/**
 * @class Lamp
 * @brief Represents an interactable lamp that toggles a PointLight on and off.
 *
 * A Lamp controls a PointLight component and allows the player to switch it
 * between an illuminated and unlit state. The lamp stores the original light
 * intensities and color so it can restore them when turned back on.
 */
class Lamp : public Entity
{
public:

    /**
     * @brief Constructs a new Lamp entity.
     *
     * Initializes:
     * - Base Entity properties
     * - Stores the target ambient/diffuse intensities and color
     * - Turns the light off initially
     * - Positions the light above the lamp model
     *
     * @param model Pointer to the lamp model.
     * @param position World position of the lamp.
     * @param rotation Rotation vector.
     * @param scale Scale vector.
     * @param lightSource Pointer to the PointLight controlled by the lamp.
     * @param interaction Whether the lamp is interactable.
     */
    Lamp(Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
         PointLight* lightSource, bool interaction);

    /**
     * @brief Toggles the lamp on or off.
     *
     * Behavior:
     * - If the lamp is on: sets all light intensities to zero (turns off)
     * - If the lamp is off: restores original intensities and color (turns on)
     *
     * @param playerInventory Pointer to the player's inventory (unused).
     */
    void Interact(Inventory* playerInventory) override;

    /**
     * @brief Updates the lamp each frame.
     *
     * Currently unused — lamp logic is handled entirely in Interact().
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override {};

private:

    /** @brief Pointer to the PointLight controlled by the lamp. */
    PointLight* light;

    /** @brief Whether the lamp is currently turned on. */
    bool isOn = false;

    /** @brief Stored ambient intensity used when turning the lamp back on. */
    float targetAmbient;

    /** @brief Stored diffuse intensity used when turning the lamp back on. */
    float targetDiffuse;

    /** @brief Stored light color used when turning the lamp back on. */
    glm::vec3 targetColor;
};

#include "Lamp.h"

/**
 * @brief Constructs a new Lamp entity.
 *
 * A Lamp is an interactable light source that can be toggled on and off.
 * This constructor:
 * - Stores the target light intensities and color
 * - Turns the light off initially
 * - Positions the light above the lamp model
 * - Automatically toggles the lamp once to initialize its state
 *
 * @param model Pointer to the lamp model.
 * @param position World position of the lamp.
 * @param rotation Rotation vector.
 * @param scale Scale vector.
 * @param lightSource Pointer to the PointLight controlled by the lamp.
 * @param interaction Whether the lamp is interactable.
 */
Lamp::Lamp(Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
           PointLight* lightSource, bool interaction)
    : Entity(model, position, rotation, scale, interaction), light(lightSource)
{
    actionText = "Toggle Lamp";

    // Store original light values
    targetAmbient = light->getAmbientIntensity();
    targetDiffuse = light->getDiffuseIntensity();
    targetColor   = light->getColor();

    // Turn light off initially
    light->setAmbientIntensity(0.0f);
    light->setDiffuseIntensity(0.0f);
    light->setColor(glm::vec3(0.0f));

    // Position the light above the lamp
    light->setPosition(position + glm::vec3(0.0f, 3.0f, 0.0f));

    // Initialize lamp state
    Interact(nullptr);
}

/**
 * @brief Toggles the lamp on or off.
 *
 * Behavior:
 * - If the lamp is on: sets all light intensities to zero (turns off)
 * - If the lamp is off: restores original intensities and color (turns on)
 *
 * @param playerInventory Pointer to the player's inventory (unused).
 */
void Lamp::Interact(Inventory* playerInventory)
{
    if (isOn)
    {
        // Turn off
        light->setAmbientIntensity(0.0f);
        light->setDiffuseIntensity(0.0f);
        light->setColor(glm::vec3(0.0f));
        isOn = false;
    }
    else
    {
        // Turn on
        light->setAmbientIntensity(targetAmbient);
        light->setDiffuseIntensity(targetDiffuse);
        light->setColor(targetColor);
        isOn = true;
    }
}

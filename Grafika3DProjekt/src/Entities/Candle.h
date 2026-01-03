#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include "../Light/PointLight.h"
#include <string>
#include <vector>

/**
 * @class Candle
 * @brief Represents an interactable candle that can be lit or extinguished.
 *
 * The Candle class extends Entity and provides:
 * - Light source control (ambient, diffuse, color)
 * - Interaction logic requiring a lighter
 * - Sound effects for lighting and extinguishing
 * - Utility functions for scripted lighting/blowing out
 */
class Candle : public Entity
{
public:

    /**
     * @brief Constructs a new Candle entity.
     *
     * Initializes:
     * - Base Entity properties
     * - Light source reference and its offset
     * - Stores original light intensities and color
     * - Turns the candle light off initially
     * - Loads lighting and extinguish sound effects
     *
     * @param model Pointer to the 3D model.
     * @param pos World position.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param lightSource Pointer to the PointLight representing the candle flame.
     * @param interaction Whether the candle is interactable.
     */
    Candle(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
           PointLight* lightSource, bool interaction = true);

    /**
     * @brief Handles player interaction with the candle.
     *
     * If the candle is unlit:
     * - Requires the player to hold a lighter
     * - Plays lighting sound
     * - Restores original light intensities and color
     *
     * @param playerInventory Pointer to the player's inventory.
     */
    void Interact(Inventory* playerInventory) override;

    /**
     * @brief Updates the candle each frame.
     *
     * (Currently unused — candle logic is handled in Interact, blowOut, and lightUp.)
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override {};

    /**
     * @brief Checks whether the candle is currently lit.
     *
     * @return True if the candle is lit.
     */
    bool getIsLit() { return isLit; }

    /**
     * @brief Extinguishes the candle.
     *
     * Plays extinguish sound and sets light intensities to zero.
     */
    void blowOut();

    /**
     * @brief Lights the candle without requiring interaction checks.
     *
     * Used for scripted events or puzzle logic.
     */
    void lightUp();

    /**
     * @brief Gets the action text displayed when targeting the candle.
     *
     * @return "Examine" if lit, otherwise "Light".
     */
    std::string GetActionText() override { return isLit ? "Examine" : "Light"; }

    /**
     * @brief Sets the world position of the candle's light source.
     *
     * @param newPos New light position.
     */
    void setLightPosition(glm::vec3 newPos) { candleLight->setPosition(newPos); }

    /**
     * @brief Gets the world position of the candle's light source.
     *
     * @return Light position vector.
     */
    glm::vec3 getLightPosition() { return candleLight->getPosition(); }

    /**
     * @brief Candle destructor.
     */
    ~Candle();

private:

    /** @brief Light source representing the candle flame. */
    PointLight* candleLight;

    /** @brief Whether the candle is currently lit. */
    bool isLit = false;

    /** @brief Whether the player attempted to light the candle without a lighter. */
    bool triedToLight = false;

    /** @brief Original ambient light intensity before extinguishing. */
    float lightAmbientIntensity;

    /** @brief Original diffuse light intensity before extinguishing. */
    float lightDiffuseIntensity;

    /** @brief Original light color before extinguishing. */
    glm::vec3 lightColor;

    /** @brief Path to the candle lighting sound effect. */
    static std::string lightingSoundPath;

    /** @brief Path to the candle extinguish sound effect. */
    static std::string extinguishSoundPath;
};

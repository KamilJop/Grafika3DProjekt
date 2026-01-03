#include "Candle.h"

/**
 * @brief Path to the candle lighting sound effect.
 */
std::string Candle::lightingSoundPath = "Audio/candle_lighting.mp3";

/**
 * @brief Path to the candle extinguish sound effect.
 */
std::string Candle::extinguishSoundPath = "Audio/candle_blow.mp3";

/**
 * @brief Constructs a new Candle entity.
 *
 * Initializes:
 * - Base Entity properties
 * - Light source reference and its initial offset
 * - Stores original light intensities and color
 * - Turns the candle light off initially
 * - Loads lighting and extinguish sound effects
 *
 * @param model Pointer to the 3D model.
 * @param pos World position.
 * @param rot Rotation vector.
 * @param scal Scale vector.
 * @param LightSource Pointer to the PointLight representing the candle flame.
 * @param interaction Whether the candle is interactable.
 */
Candle::Candle(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
               PointLight* LightSource, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    candleLight = LightSource;

    // Position the light slightly above the candle
    candleLight->setPosition(pos + glm::vec3(-0.1f, 0.5f, -0.1f));

    // Store original intensities and color
    lightAmbientIntensity = candleLight->getAmbientIntensity();
    lightDiffuseIntensity = candleLight->getDiffuseIntensity();
    lightColor = candleLight->getColor();

    // Turn off the candle light initially
    candleLight->setAmbientIntensity(0.0f);
    candleLight->setDiffuseIntensity(0.0f);
    candleLight->setColor(glm::vec3(0.0f));

    // Load sound effects
    AudioManager::GetInstance().Load3DSoundEffect("candle_blow", Candle::extinguishSoundPath);
    AudioManager::GetInstance().Load3DSoundEffect("candle_lighting", Candle::lightingSoundPath);
}

/**
 * @brief Candle destructor.
 */
Candle::~Candle()
{
}

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
void Candle::Interact(Inventory* playerInventory)
{
    if (!isLit)
    {
        // Player must have a lighter
        if (playerInventory->GetCurrentItem()->tag != "Lighter")
        {
            UI::SetSubtitle("I need something to light it with.", 3.0f);
            return;
        }

        // Light the candle
        AudioManager::GetInstance().Play3DSoundEffect(
            "candle_lighting", position, Config::getInstance().sfxVolume * 2.0f);

        candleLight->setAmbientIntensity(lightAmbientIntensity);
        candleLight->setDiffuseIntensity(lightDiffuseIntensity);
        candleLight->setColor(lightColor);

        isLit = true;
    }
}

/**
 * @brief Extinguishes the candle.
 *
 * Plays extinguish sound and sets light intensities to zero.
 */
void Candle::blowOut()
{
    AudioManager::GetInstance().Play3DSoundEffect("candle_blow", position);

    candleLight->setAmbientIntensity(0.0f);
    candleLight->setDiffuseIntensity(0.0f);
    candleLight->setColor(glm::vec3(0.0f));

    isLit = false;
}

/**
 * @brief Lights the candle without requiring interaction checks.
 *
 * Used for scripted events or puzzle logic.
 */
void Candle::lightUp()
{
    if (isLit)
        return;

    AudioManager::GetInstance().Play3DSoundEffect(
        "candle_lighting", position, Config::getInstance().sfxVolume * 2.0f);

    candleLight->setAmbientIntensity(lightAmbientIntensity);
    candleLight->setDiffuseIntensity(lightDiffuseIntensity);
    candleLight->setColor(lightColor);

    isLit = true;
}

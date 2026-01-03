#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Texture.h"

/**
 * @enum RadioState
 * @brief Represents the different operational states of the radio.
 *
 * - BROKEN: No batteries inserted, radio cannot function.
 * - ONE_BATTERY: One battery inserted, still insufficient to power the radio.
 * - WORKING: Fully powered and capable of playing Morse code.
 */
enum RadioState
{
    BROKEN,
    ONE_BATTERY,
    WORKING
};

/**
 * @class Radio
 * @brief Represents an interactable radio that changes behavior based on inserted batteries.
 *
 * The Radio progresses through three states:
 * - **BROKEN**: No batteries; plays broken sound and hints to find batteries.
 * - **ONE_BATTERY**: One battery inserted; still not functional, occasionally emits static.
 * - **WORKING**: Fully powered; plays Morse code periodically.
 *
 * The player interacts with the radio by inserting batteries from their inventory.
 */
class Radio : public Entity
{
public:

    /**
     * @brief Constructs a new Radio entity.
     *
     * Initializes:
     * - Base Entity properties
     * - Sets the radio to the BROKEN state
     * - Loads all sound effects used during interaction
     *
     * @param model Pointer to the radio model.
     * @param pos World position.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param interaction Whether the radio is interactable.
     */
    Radio(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction = true);

    /**
     * @brief Handles interaction with the radio.
     *
     * Behavior depends on the current state:
     * - **BROKEN**: Requires first battery.
     * - **ONE_BATTERY**: Requires second battery.
     * - **WORKING**: Plays Morse code and displays a hint.
     *
     * @param playerInventory Pointer to the player's inventory.
     */
    void Interact(Inventory* playerInventory) override;

    /**
     * @brief Updates the radio each frame.
     *
     * Handles countdown timers for sound playback.
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Gets the action text displayed when targeting the radio.
     *
     * @return "Turn On" when BROKEN or ONE_BATTERY, "Listen" when WORKING.
     */
    std::string GetActionText() override;

    /**
     * @brief Radio destructor.
     */
    ~Radio();

private:

    /** @brief Current operational state of the radio. */
    int currentState = BROKEN;

    /** @brief Timer used to control sound playback intervals. */
    float soundTimer = 0.0f;

    /** @brief Sound played when interacting with a broken radio. */
    std::string brokenRadioSound = "Audio/broken_radio.mp3";

    /** @brief Sound played when inserting the first battery. */
    std::string firstInsert = "Audio/insert_first_battery.mp3";

    /** @brief Sound played when inserting the second battery. */
    std::string secondInsert = "Audio/insert_second_battery.mp3";

    /** @brief Static noise sound played when the radio has one battery. */
    std::string radioNoises = "Audio/radio_noises.mp3";

    /** @brief Morse code sound played when the radio is fully powered. */
    std::string morseCodeSound = "Audio/morse.mp3";
};

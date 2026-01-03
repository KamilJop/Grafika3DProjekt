#include "Radio.h"

/**
 * @brief Constructs a new Radio entity.
 *
 * A Radio has three possible states:
 * - BROKEN: No batteries inserted
 * - ONE_BATTERY: One battery inserted, still not functional
 * - WORKING: Fully powered and capable of playing Morse code
 *
 * This constructor:
 * - Initializes the radio in the BROKEN state
 * - Loads all sound effects used during interaction
 *
 * @param model Pointer to the radio model.
 * @param pos World position.
 * @param rot Rotation vector.
 * @param scal Scale vector.
 * @param interaction Whether the radio is interactable.
 */
Radio::Radio(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    currentState = BROKEN;

    AudioManager::GetInstance().Load3DSoundEffect(brokenRadioSound, brokenRadioSound);
    AudioManager::GetInstance().Load3DSoundEffect(firstInsert, firstInsert);
    AudioManager::GetInstance().Load3DSoundEffect(secondInsert, secondInsert);
    AudioManager::GetInstance().Load3DSoundEffect(radioNoises, radioNoises);
    AudioManager::GetInstance().Load3DSoundEffect(morseCodeSound, morseCodeSound);
}

/**
 * @brief Radio destructor.
 */
Radio::~Radio()
{
}

/**
 * @brief Handles interaction with the radio.
 *
 * Behavior depends on the radio's current state:
 *
 * **BROKEN**
 * - If the player has a battery: inserts it and transitions to ONE_BATTERY
 * - Otherwise: plays broken sound and displays hint
 *
 * **ONE_BATTERY**
 * - If the player has another battery: inserts it and transitions to WORKING
 * - Otherwise: plays static noise occasionally and displays hint
 *
 * **WORKING**
 * - Plays Morse code periodically
 * - Displays a mysterious subtitle hint
 *
 * @param playerInventory Pointer to the player's inventory.
 */
void Radio::Interact(Inventory* playerInventory)
{
    switch (currentState)
    {
    case BROKEN:
        if (playerInventory->GetCurrentItem()->tag == "battery")
        {
            currentState = ONE_BATTERY;
            playerInventory->RemoveItem("battery");
            AudioManager::GetInstance().Play3DSoundEffect(firstInsert, getPosition());
            UI::SetSubtitle("I inserted a battery into the radio. Maybe it will work now.", 3.0f);
        }
        else
        {
            AudioManager::GetInstance().Play3DSoundEffect(brokenRadioSound, getPosition());
            UI::SetSubtitle("It's dead. I need to find some batteries.", 3.0f);
        }
        break;

    case ONE_BATTERY:
        if (playerInventory->GetCurrentItem()->tag == "battery")
        {
            currentState = WORKING;
            playerInventory->RemoveItem("battery");
            AudioManager::GetInstance().Play3DSoundEffect(secondInsert, getPosition());
            UI::SetSubtitle("I inserted another battery into the radio. It's working now!", 3.0f);
        }
        else
        {
            UI::SetSubtitle("It has one battery, but it still won't turn on. I need another one.", 3.0f);

            if (soundTimer <= 0.0f)
            {
                soundTimer = 5.5f;
                AudioManager::GetInstance().Play3DSoundEffect(radioNoises, getPosition());
            }
        }
        break;

    case WORKING:
        if (soundTimer <= 0.0f)
        {
            soundTimer = 10.0f;
            AudioManager::GetInstance().Play3DSoundEffect(morseCodeSound, getPosition());
        }

        UI::SetSubtitle("Beeps and pauses... Is something trying to communicate through the noise?", 4.0f);
        break;
    }
}

/**
 * @brief Gets the action text displayed when targeting the radio.
 *
 * @return "Turn On" when BROKEN or ONE_BATTERY, "Listen" when WORKING.
 */
std::string Radio::GetActionText()
{
    switch (currentState)
    {
    case BROKEN:
        return "Turn On";
    case ONE_BATTERY:
        return "Turn On";
    case WORKING:
        return "Listen";
    default:
        return "Examine";
    }
}

/**
 * @brief Updates the radio each frame.
 *
 * Handles countdown for sound playback timers.
 *
 * @param deltaTime Time elapsed since last frame.
 */
void Radio::Update(float deltaTime)
{
    if (soundTimer > 0.0f)
    {
        soundTimer -= deltaTime;
    }
    else
    {
        soundTimer = 0.0f;
    }
}

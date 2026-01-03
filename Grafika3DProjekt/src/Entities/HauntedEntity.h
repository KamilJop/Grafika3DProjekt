#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Model.h"
#include "../Systems/CandlePuzzle.h"

/**
 * @class HauntedEntity
 * @brief Represents an entity that visually transforms after a puzzle is solved.
 *
 * A HauntedEntity begins with a default model and switches to an alternate model
 * once a specific puzzle condition is met (in this case, when CandlePuzzle::isSolved becomes true).
 *
 * This allows for environmental changes, supernatural effects, or story-driven
 * transformations triggered by gameplay events.
 */
class HauntedEntity : public Entity
{
public:

    /**
     * @brief Constructs a new HauntedEntity.
     *
     * Initializes:
     * - Base Entity properties
     * - Stores the alternate model used after transformation
     *
     * @param model The default model shown before the puzzle is solved.
     * @param secondModel The alternate model shown after the puzzle is solved.
     * @param pos World position.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param interaction Whether the entity is interactable.
     */
    HauntedEntity(Model* model, Model* secondModel,
                  glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
                  bool interaction = true);

    /**
     * @brief Updates the haunted entity each frame.
     *
     * Behavior:
     * - Resets examination state every frame
     * - If the entity has already switched models, no further updates occur
     * - If the CandlePuzzle is solved, replaces the current model with the alternate one
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Gets the action text displayed when targeting the entity.
     *
     * Haunted entities always return "Examine".
     *
     * @return "Examine"
     */
    std::string GetActionText() override { return "Examine"; }

    /**
     * @brief Destructor for HauntedEntity.
     */
    ~HauntedEntity();

private:

    /** @brief The model used after the puzzle is solved. */
    Model* alternateModel;

    /** @brief Whether the entity has already switched to the alternate model. */
    bool hasSwitched = false;
};

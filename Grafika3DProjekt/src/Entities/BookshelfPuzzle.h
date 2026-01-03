#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Texture.h"
#include "Book.h"

/**
 * @class BookshelfPuzzle
 * @brief Represents the movable bookshelf that reacts to the book puzzle solution.
 *
 * The BookshelfPuzzle class:
 * - Slides the bookshelf when interacted with
 * - Automatically moves when the Book puzzle is solved
 * - Updates its collision box during movement
 * - Marks itself as finished once fully moved
 */
class BookshelfPuzzle : public Entity
{
public:

    /**
     * @brief Constructs a new BookshelfPuzzle entity.
     *
     * Initializes:
     * - Base Entity properties
     * - Marks the object as non‑pickable
     * - Stores original and solved positions
     *
     * @param model Pointer to the 3D model.
     * @param pos Initial world position.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param interaction Whether the puzzle object is interactable.
     */
    BookshelfPuzzle(Model* model, glm::vec3 pos, glm::vec3 rot,
                    glm::vec3 scal, bool interaction = false);

    /**
     * @brief Handles player interaction with the bookshelf.
     *
     * If the bookshelf is not already moving, triggers the movement animation.
     *
     * @param playerInventory Pointer to the player's inventory (unused).
     */
    void Interact(Inventory* playerInventory) override;

    /**
     * @brief Updates the bookshelf each frame.
     *
     * Handles:
     * - Waiting for the Book puzzle to be solved
     * - Sliding the bookshelf to its solved position
     * - Updating collision box during movement
     * - Marking the puzzle as finished once movement completes
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Destructor for BookshelfPuzzle.
     */
    ~BookshelfPuzzle();

private:

    /** @brief Whether the bookshelf is currently animating. */
    bool isMoving = false;

    /** @brief Whether the bookshelf has finished its solved movement. */
    bool ended = false;

    /** @brief Original resting position of the bookshelf. */
    glm::vec3 originalPosition;

    /** @brief Final position the bookshelf moves to when puzzle is solved. */
    glm::vec3 solvedPosition;
};

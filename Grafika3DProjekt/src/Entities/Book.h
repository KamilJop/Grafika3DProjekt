#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Texture.h"

/**
 * @class Book
 * @brief Represents a movable book used in a bookshelf puzzle.
 *
 * The Book class extends Entity and provides:
 * - Pulling/pushing animation
 * - Puzzle logic (tracking correct book order)
 * - Sound effects for movement and puzzle completion
 * - Automatic movement to a solved position when puzzle is completed
 */
class Book : public Entity
{
public:

    /**
     * @brief Constructs a new Book entity.
     *
     * Initializes:
     * - Base Entity properties
     * - Book tag and movement states
     * - Original, moved, and solved positions
     * - Loads movement and puzzle‑solved sound effects
     *
     * @param model Pointer to the 3D model.
     * @param pos Initial world position.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param tag Identifier tag for the book.
     * @param interaction Whether the book is interactable.
     */
    Book(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
         std::string tag, bool interaction = true);

    /**
     * @brief Handles player interaction with the book.
     *
     * Toggles the book between moved and original positions,
     * plays a movement sound, and updates puzzle state.
     *
     * @param playerInventory Pointer to the player's inventory (unused).
     */
    void Interact(Inventory* playerInventory) override;

    /**
     * @brief Updates the book each frame.
     *
     * Handles:
     * - Movement animation (pulling out / pushing in)
     * - Transition to solved position when puzzle is completed
     * - Collision box updates
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Destructor for Book.
     */
    ~Book();

    /**
     * @brief List of tags for books currently pulled out.
     *
     * Used to evaluate puzzle state.
     */
    static std::vector<std::string> bookTags;

    /**
     * @brief Global puzzle state indicating whether the bookshelf puzzle is solved.
     */
    static bool solved;

    /**
     * @brief Gets the action text displayed when targeting the book.
     *
     * @return "Examine" if puzzle solved, otherwise "Pull out" or "Put back".
     */
    std::string GetActionText() override;

private:

    /** @brief Whether the book is currently pulled out. */
    bool isMoved = false;

    /** @brief Whether the book is currently animating. */
    bool isMoving = false;

    /** @brief Whether the book is animating toward the solved position. */
    bool isSolvedMoving = false;

    /**
     * @brief Checks whether the bookshelf puzzle is solved.
     *
     * Compares the list of moved books with the required solution order.
     */
    void CheckSolved();

    /** @brief Path to the bookshelf puzzle solved sound. */
    std::string bookshelfSolvedSoundPath = "Audio/bookshelf_solved.mp3";

    /** @brief Sound effects for book movement. */
    std::vector<std::string> movingSoundPaths = {
        "Audio/book_move1.mp3",
        "Audio/book_move2.mp3",
        "Audio/book_move3.mp3",
        "Audio/book_move4.mp3"
    };

    /** @brief Original resting position of the book. */
    glm::vec3 originalPosition;

    /** @brief Position when the book is pulled out. */
    glm::vec3 movedPosition;

    /** @brief Position the book moves to when the puzzle is solved. */
    glm::vec3 solvedPosition;

    /** @brief Number of books required to solve the puzzle. */
    int solveSize = 4;

    /** @brief Correct order of book tags required to solve the puzzle. */
    std::string correctBooks[4] = { "redBook", "yellowBook", "orangeBook", "purpleBook" };
};

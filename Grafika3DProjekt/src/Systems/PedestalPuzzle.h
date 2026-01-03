#pragma once
#include "../Entities/Pedestal.h"
#include "../Entities/Door.h"

/**
 * @class PedestalPuzzle
 * @brief Checks whether all pedestals have the correct item and unlocks doors when solved.
 */
class PedestalPuzzle
{
public:
    /**
     * @brief Creates the pedestal puzzle.
     *
     * @param pedestal List of pedestals involved in the puzzle.
     * @param door1 First door to unlock when solved.
     * @param door2 Second door to unlock when solved.
     */
    PedestalPuzzle(std::vector<Pedestal*> pedestal, Door* door1, Door* door2);

    /** @brief Returns true if the puzzle has been solved. */
    bool getSolved() { return puzzleSolved; }

    /** @brief Updates the puzzle state each frame. */
    void Update();

private:
    std::vector<Pedestal*> pedestals; ///< Pedestals to check.
    Door* doorToUnlock1;              ///< First door unlocked when solved.
    Door* doorToUnlock2;              ///< Second door unlocked when solved.

    bool puzzleSolved = false;        ///< True once all pedestals are correct.
    bool puzzleSolvedPlayed = false;  ///< Ensures solved sound plays once.

    std::string puzzleSolvedSoundPath = "Audio/demon_death.mp3"; ///< Sound played on solve.
};

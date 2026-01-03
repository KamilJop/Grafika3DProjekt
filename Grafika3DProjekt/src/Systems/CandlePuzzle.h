#pragma once
#include "../Entities/Candle.h"
#include "../Entities/Door.h"
#include "../Entities/Pedestal.h"
#include <vector>
#include "../Core/Config.h"
#include <string>

/**
 * @class CandlePuzzle
 * @brief Handles the candle‑lighting puzzle logic.
 *
 * Tracks candle order, checks the solution, and triggers
 * events such as lighting secondary candles and locking the door.
 */
class CandlePuzzle
{
public:

    /**
     * @brief Creates the candle puzzle.
     *
     * @param candles First set of candles (player interacts with these).
     * @param candles2 Second set of candles (light up after solving).
     * @param ped Pedestals revealed after solving.
     * @param doors Door that becomes locked when puzzle completes.
     */
    CandlePuzzle(std::vector<Candle*> candles,
                 std::vector<Candle*> candles2,
                 std::vector<Pedestal*> ped,
                 Door* doors);

    /**
     * @brief Updates puzzle logic each frame.
     *
     * @param dt Delta time.
     */
    void Update(float dt);

    /** @brief True when the puzzle has been solved. */
    static bool isSolved;

private:
    std::vector<Candle*> candles;        ///< Player‑lit candles.
    std::vector<Candle*> secondCandles;  ///< Auto‑lit candles after solving.
    std::vector<std::string> solution;   ///< Player's input order.
    std::vector<Pedestal*> pedestals;    ///< Revealed after solving.

    std::vector<std::string> correctOrder = { "B", "A", "E", "L", "" }; ///< Required order.

    Door* doorToLock;                    ///< Door affected by puzzle.
    float timer = 0.0f;                  ///< Sequence timer.

    std::string demonicWhisperSoundPath = "Audio/demonic_whisper.mp3";
    std::string doorSlamSoundPath = "Audio/door_slam.mp3";
    std::string whooshSoundPath = "Audio/whoosh.mp3";

    bool doorSoundPlayed = false;
    bool whooshSoundPlayed[4] = { false, false, false, false };
};

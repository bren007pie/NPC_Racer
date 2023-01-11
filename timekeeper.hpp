/**
 * @file timekeeper.hpp
 * @author Brendan Fallon (fallonbr@mcmaster.ca) (https://github.com/bren007pie)
 * @brief This module contains chrono based class for timing and doing NPC race statistics.
 * @version 0.4
 * @date January 10, 2022
 * @copyright Copyright (c) 2022 Brendan Fallon
 */

//// Preprocessor Directives ////
#include <chrono> // std::chrono::steady_clock, std::chrono::time_point, std::chrono::duration

namespace NPC_Racer
{
    // originally adapted  from
    //// ----------------------- ////
    //// Timekeeper Class ////
    //// ----------------------- ////
    /**
     * @brief Starts timers and displays statistics
     * @cite Adapted from https://baraksh.com/CSE701/notes.php#interlude-measuring-performance-with-chrono
     */
    class timekeeper
    {
    public:
        /**
         * @brief Records the start time of the race when called.
         */
        void start()
        {
            start_time = std::chrono::steady_clock::now();
        }

        /**
         * @brief Records the start end of the race when called.
         */
        void end()
        {
            elapsed_time = std::chrono::steady_clock::now() - start_time;
        }

        /**
         * @brief Gives the time of the race in seconds.
         * @return The elapsed time of the race as a double.
         */
        double race_time() const
        {
            return elapsed_time.count();
        }

    private:
        /**
         * @param start_time The start time point for the race.
         * @note When NPC_Racer::timekeeper object is initialized the start time is recorded but can be overridden with NPC_Racer::timekeeper::start()
         */
        std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();

        /**
         * @param elapsed_time The elapsed time duration as a double in seconds.
         */
        std::chrono::duration<double> elapsed_time = std::chrono::duration<double>::zero();
    };
}
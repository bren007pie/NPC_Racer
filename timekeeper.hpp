/**
 * @file timekeeper.hpp
 * @author Brendan Fallon (fallonbr@mcmaster.ca) (https://github.com/bren007pie)
 * @brief This module contains chrono based class for timing and NPC race statistics functions.
 * @version 1.0
 * @date January 10, 2022
 * @copyright Copyright (c) 2022 Brendan Fallon
 */

//// Preprocessor Directives ////
#pragma once
#include <chrono>  // std::chrono::steady_clock, std::chrono::time_point, std::chrono::duration
#include <vector>  // std::vector
#include <numeric> // std::accumulate
#include <cmath>

namespace NPC_Racer
{
    //// --------- ////
    //// Functions ////
    //// --------- ////
    /**
     * @brief Takes the average of a series of pathfinding trial times.
     *
     * @param trial_times A collection of all the run times from each trial.
     *
     * @return Returns the population mean or average.
     */
    double run_average(const std::vector<double> &trial_times)
    {
        return (std::accumulate(trial_times.begin(), trial_times.end(), 0.0) / (double)trial_times.size());
    }

    /**
     * @brief Takes the population standard deviation of a series of pathfinding trial times.
     *
     * @param trial_times A collection of all the run times from each trial.
     *
     * @return Returns the population standard deviation of the series of trials.
     */
    double run_standard_deviation(const std::vector<double> &trial_times)
    {
        // inspired by: https://www.programiz.com/cpp-programming/examples/standard-deviation
        // recalculating the mean to avoid user error of having to re-input
        const double mean = std::accumulate(trial_times.begin(), trial_times.end(), 0.0) / (double)trial_times.size();
        double standard_deviation = 0.0;

        for (size_t i = 0; i < trial_times.size(); i++)
        {
            standard_deviation += std::pow(trial_times[i] - mean, 2); // taking the some of the squares
        }

        return std::sqrt(standard_deviation / (double)trial_times.size());
    }

    /**
     * @brief Takes the percentage difference between run averages.
     *
     * @param bigger The longer of the two trials.
     * @param smaller The shorter of the two trials.
     *
     * @return Returns the percentage difference as a percentage (i.e. decimal x 100).
     * @note Percentage difference definition: https://www.calculatorsoup.com/calculators/algebra/percent-difference-calculator.php
     */
    double run_percentage_difference(const double &bigger, const double &smaller)
    {
        double mean = (bigger + smaller) / 2;
        double percentage_difference = std::abs(bigger - smaller) / mean; // abs just in case user passed wrong input
        return (percentage_difference * 100);
    }

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

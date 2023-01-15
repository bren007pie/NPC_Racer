/**
 * @file main.cpp
 * @author Brendan Fallon (fallonbr@mcmaster.ca) (https://github.com/bren007pie)
 * @brief The main entry point reads in the inputs and runs all the NPC races.
 * @version 1.0
 * @date Sunday December 18, 2022
 * @copyright Copyright (c) 2022 Brendan Fallon
 */

//// Preprocessor Directives ////
#include <iostream>       // std::cout
#include <vector>         // std::vector
#include <algorithm>      // std::min_element, std::distance
#include "mazes.hpp"      // NPC_Racer::maze
#include "agents.hpp"     // NPC_Racer::depth_first_agent, NPC_Racer::dijkstra_agent
#include "timekeeper.hpp" // NPC_Racer::timekeeper, NPC_Racer::run_average, NPC_Racer::run_percentage_difference

/**
 * @brief Runs the races of the mazes passed to the program as arguments.
 */
int main(int argc, char *argv[])
{
    // Argument Parsing //
    // for now the program can only run one race at a time.
    if (argc == 1)
    {
        std::cout << "### Welcome to NPC Racer! ###\n"
                  << "- The only argument is the filename of the maze.\n"
                  << "- Make sure the maze file is in the same directory as the executable.\n"
                  << "- For example on Windows Powershell type: \n\t .\\NPCRacer.exe 10_10_test_maze.txt \n";
        exit(EXIT_SUCCESS);
    }
    else if (argc != 2)
    {
        std::cout << "ERROR: More than 1 maze argument, only 1 should be given.\n"
                  << (argc - 1) << " arguments were given.\n";
        exit(EXIT_FAILURE);
    }

    //// Race variables ////
    size_t trials_per_run = 11; // The number of times to loop each pathfinding algorithm.

    // Timers Setup //
    NPC_Racer::timekeeper total_program_timer;
    NPC_Racer::timekeeper maze_parsing_timer;
    NPC_Racer::timekeeper depth_first_timer;
    NPC_Racer::timekeeper dijkstra_timer;

    total_program_timer.start(); // starting measurement of the program

    // Statistics //
    std::vector<double> depth_first_trials; // For only 11 trials don't need to reserve space
    std::vector<double> dijkstra_trials;
    std::vector<double> run_averages; // the average of each run stored
    double depth_first_average;       // averages, standard deviations, and percentage differences
    double dijkstra_average;
    double depth_first_standard_deviation;
    double dijkstra_standard_deviation;
    double depth_first_percentage_difference;
    double dijkstra_percentage_difference;

    // Agent Setup //
    NPC_Racer::depth_first_agent depth_first_racer;
    NPC_Racer::dijkstra_agent dijkstra_racer;

    // Parsing Maze //
    maze_parsing_timer.start();
    NPC_Racer::maze race_maze(argv[1]); // parsing the maze based on the argument given.
    maze_parsing_timer.end();
    std::cout << race_maze; // printing it out

    //// Runs ////
    // Depth-first Pathfinding //
    std::cout << "Running depth-first search pathfinding.\n";
    for (size_t i = 0; i < trials_per_run; i++)
    {
        depth_first_timer.start();
        depth_first_racer.pathfind(race_maze); // doing the pathfinding
        depth_first_timer.end();
        depth_first_trials.push_back(depth_first_timer.race_time()); // adding to trial container
    }

    if (depth_first_racer.path.size() > 1) // If maze didn't encounter an error
    {
        std::cout << "Depth-first pathfinding complete, path was successfully found!\n";
    }

    race_maze.print_path_on_maze(depth_first_racer.path);
    std::cout << "\n"; // space for a bit of separation

    // Dijkstra Agent Pathfinding //
    std::cout << "Running Dijkstra's algorithm pathfinding.\n";

    for (size_t i = 0; i < trials_per_run; i++)
    {
        dijkstra_timer.start();
        dijkstra_racer.pathfind(race_maze); // doing the pathfinding
        dijkstra_timer.end();
        dijkstra_trials.push_back(dijkstra_timer.race_time()); // adding to trial container
    }

    if (dijkstra_racer.path.size() > 1) // If maze didn't encounter an error
    {
        std::cout << "Dijkstra's algorithm pathfinding complete, path was successfully found!\n";
    }

    race_maze.print_path_on_maze(dijkstra_racer.path);
    std::cout << "\n"; // space for a bit of separation

    // race statistics //
    depth_first_average = NPC_Racer::run_average(depth_first_trials);
    depth_first_standard_deviation = NPC_Racer::run_standard_deviation(depth_first_trials);

    dijkstra_average = NPC_Racer::run_average(dijkstra_trials);
    dijkstra_standard_deviation = NPC_Racer::run_standard_deviation(dijkstra_trials);

    // finding the shortest of all the runs
    run_averages = {depth_first_average, dijkstra_average}; // make sure to add all the algorithm averages here
    // finding index of the minimum, used also for printing, based on https://en.cppreference.com/w/cpp/algorithm/min_element
    uint64_t minimum_index = (uint64_t)std::distance(run_averages.begin(), std::min_element(run_averages.begin(), run_averages.end()));

    double minimum_average = run_averages[minimum_index];

    // percentage difference
    depth_first_percentage_difference = NPC_Racer::run_percentage_difference(depth_first_average, minimum_average);
    dijkstra_percentage_difference = NPC_Racer::run_percentage_difference(dijkstra_average, minimum_average);

    //// Race Results ////
    std::cout << "\n## RACE RESULTS ##.\n";
    std::cout << "Maze Parsing time: " << maze_parsing_timer.race_time() << "\n";

    std::cout << "\n# Depth-first pathfinding #\n";
    std::cout << "Average depth-first pathfinding time: " << depth_first_average << " seconds\n";
    std::cout << "Depth-first pathfinding standard deviation: " << depth_first_standard_deviation << " seconds\n";

    std::cout << "\n# Dijkstra's algorithm pathfinding #\n";
    std::cout << "Average Dijkstra pathfinding time: " << dijkstra_average << " seconds\n";
    std::cout << "Dijkstra pathfinding standard deviation: " << dijkstra_standard_deviation << " seconds\n";

    std::cout << "\n# Summary Table #\n";
    std::cout << "Race on maze: '" << race_maze.file_name << "'\n";
    std::cout << "\t| Depth-first\t| Dijkstra's\t| Algorithm\t\n";
    std::cout << "--------|---------------|---------------|\n";
    std::cout << "Winner\t| " << (minimum_index == 0 ? "WINNER" : "\t") << "\t| " << (minimum_index == 1 ? "WINNER" : "\t") << "\t| \n";
    std::cout << "Average\t| " << depth_first_average << "\t| " << dijkstra_average << "\t| \n";
    std::cout << "% diff.\t| " << (depth_first_percentage_difference == 0 ? "0\t" : std::to_string(depth_first_percentage_difference)) << " %\t| " << (dijkstra_percentage_difference == 0 ? "0\t" : std::to_string(dijkstra_percentage_difference)) << " %\t| \n";
    std::cout << "STDDEVP.| " << depth_first_standard_deviation << "\t| " << dijkstra_standard_deviation << "\t| \n";
    std::cout << "--------|---------------|---------------|\n";
    std::cout << "Value\n";
    std::cout << "^average and standard deviation time is in seconds\n";

    total_program_timer.end(); // end of the program
    std::cout << "\nTotal program time: " << total_program_timer.race_time() << " seconds\n";
}

/**
 * @file main.cpp
 * @author Brendan Fallon (fallonbr@mcmaster.ca) (https://github.com/bren007pie)
 * @brief The main entry point reads in the inputs and runs all the NPC races.
 * @version 0.3
 * @date Sunday December 18, 2022
 * @copyright Copyright (c) 2022 Brendan Fallon
 */

//// Preprocessor Directives ////
#include <iostream>       // std::cout
#include "mazes.hpp"      // NPC_Racer::maze, NPC_Racer::make_empty_maze_file
#include "timekeeper.hpp" // NPC_Racer::timekeeper

/**
 * @brief Runs the races of the mazes passed to the program as arguments.
 */
int main(int argc, char *argv[])
{
    // for now the program can only run one race at a time.
    if (argc != 2)
    {
        std::cout << "ERROR: 1 an only one input maze argument should be given.\n"
                  << (argc - 1) << " arguments were given.\n";
        exit(EXIT_FAILURE);
    }

    NPC_Racer::timekeeper timer;
    timer.start();
    NPC_Racer::maze race_maze(argv[1]); // parsing the maze based on the argument given.
    std::cout << race_maze;             // printing it out
    timer.end();

    std::cout << timer.race_time();
}

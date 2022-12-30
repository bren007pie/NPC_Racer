/**
 * @file main.cpp
 * @author Brendan Fallon (fallonbr@mcmaster.ca) (https://github.com/bren007pie)
 * @brief The main entry point reads in the inputs and runs all the NPC races.
 * @version 0.1
 * @date Sunday December 18, 2022
 * @copyright Copyright (c) 2022 Brendan Fallon
 */

//// Preprocessor Directives ////
#include <iostream>  // std::cout
#include "mazes.hpp" // NPC_Racer::maze

int main() // int argc, char const *argv[]
{
    std::cout << "Hello NPCs!\n";
    // NPC_Racer::make_empty_maze_file(10, 10, false);

    NPC_Racer::maze test_maze("10_10_test_maze.csv");
    //   NPC_Racer::maze test_maze("10_10_test_maze.csv);

    std::cout << test_maze;
    return 0;

    // okay let's play around with the maze file here
}

/**
 * @file main.cpp
 * @author Brendan Fallon (fallonbr@mcmaster.ca) (https://github.com/bren007pie)
 * @brief The main entry point reads in the inputs and runs all the NPC races.
 * @version 0.3
 * @date Sunday December 18, 2022
 * @copyright Copyright (c) 2022 Brendan Fallon
 */

//// Preprocessor Directives ////
#include <iostream>  // std::cout
#include "mazes.hpp" // NPC_Racer::maze, NPC_Racer::make_empty_maze_file

int main()
{
    NPC_Racer::maze test_maze_1("10_10_test_maze.txt"); // parsing first maze
    std::cout << test_maze_1;                           // printing it out
    NPC_Racer::maze test_maze_2("10_10_test_maze.csv"); // parsing second maze
    std::cout << test_maze_2;                           // printing that one out
}

/**
 * @file main.cpp
 * @author Brendan Fallon (fallonbr@mcmaster.ca) (https://github.com/bren007pie)
 * @brief The main entry point reads in the inputs and runs all the NPC races.
 * @version 0.1
 * @date Sunday December 18, 2022
 * @copyright Copyright (c) 2022 Brendan Fallon
 */

//// Preprocessor Directives ////
#include <iostream>
#include "mazes.hpp"

int main() // int argc, char const *argv[]
{
    std::cout << "Hello NPCs!\n";
    NPC_Racer::make_empty_maze_file(100, 100, true);
    return 0;
}

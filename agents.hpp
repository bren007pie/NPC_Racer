/**
 * @file agents.hpp
 * @author Brendan Fallon (fallonbr@mcmaster.ca) (https://github.com/bren007pie)
 * @brief This module contains the base agent class and derived agent classes with different pathfinding algorithms.
 * @version 0.3
 * @date Sunday December 30, 2022
 * @copyright Copyright (c) 2022 Brendan Fallon
 */

//// Preprocessor Directives ////
#pragma once
#include <iostream>  // std::cout
#include <list>      // std::list
#include "mazes.hpp" // NPC_Racer::maz

////// ========= //////
////// Interface //////
////// ========= //////

namespace NPC_Racer
{
    //// --------- ////
    //// Functions ////
    //// --------- ////

    //// ----------------------- ////
    //// Brute Force Agent Class ////
    //// ----------------------- ////
    /**
     * @brief Agent class that does a brute force search to find the shortest path in a maze.
     */
    class brute_force_agent // deriving from virtual class agent
    {
    public:
        /**
         * @brief Constructs an agent object than can perform brute force search pathfinding on mazes.
         *
         * @result Initializes the agent object.
         */
        brute_force_agent()
        {
            // no initialization needed
        }

        /**
         * @brief Runs a brute force search algorithm to determine the shortest path from the start point to the destination point on the specified maze.
         *
         * @return A doubly linked list of index positions denoting the shortest path between the start point and destination point.
         */
        std::list<uint64_t> pathfind(const NPC_Racer::maze &race_maze);
    };

}

////// ============== //////
////// Implementation //////
////// ============== //////

std::list<uint64_t> NPC_Racer::brute_force_agent::pathfind(const NPC_Racer::maze &race_maze)
{
    race_maze.print_maze();
    std::list<uint64_t> test_list = {1, 2};
    test_list.push_back(3);
    test_list.push_front(0);
    return test_list;
}
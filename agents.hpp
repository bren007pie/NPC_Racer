/**
 * @file agents.hpp
 * @author Brendan Fallon (fallonbr@mcmaster.ca) (https://github.com/bren007pie)
 * @brief This module contains the base agent class and derived agent classes with different pathfinding algorithms.
 * @version 1.0
 * @date Sunday December 30, 2022
 * @copyright Copyright (c) 2022 Brendan Fallon
 */

//// Preprocessor Directives ////
#pragma once
#include <iostream>  // std::cout
#include <algorithm> // std::min_element, std::distance
#include <deque>     // std::deque
#include <vector>    // std::vector
#include <limits>    // std::numeric_limits
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
    //// Depth-first Agent Class ////
    //// ----------------------- ////
    /**
     * @brief Agent class that does a depth-first search to find a path in a maze.
     */
    class depth_first_agent
    {
    public:
        /**
         * @brief Constructs an agent object than can perform depth-first search pathfinding on mazes.
         *
         * @result Initializes the agent object.
         */
        depth_first_agent()
        {
            // nothing to initialize on construction
        }

        /**
         * @brief Runs a depth-first search algorithm to determine a path from the start point to the destination point on the specified maze.
         *
         * @param race_maze The maze to do the pathfinding on.
         *
         * @return A deque of index positions denoting a path between the start point and destination point.
         * @cite inspired and translated from: https://github.com/mikepound/mazesolving
         */
        std::deque<uint64_t> &pathfind(const NPC_Racer::maze &race_maze);

        //// Data Members ////
        // Public data members that could be used for purposes other than pathfinding are included here

        /**
         * @param current_position The current position of the agent during pathfinding.
         */
        uint64_t current_position;

        /**
         * @param pathfinding_completed If the pathfinding algorithm is completed or not.
         */
        bool pathfinding_completed = false;

        /**
         * @param path The path found from pathfinding composed of indicies.
         * @note deque used for quick insertion
         */
        std::deque<uint64_t> path = {};

        /**
         * @param nodes_explored count of nodes explored in pathfinding
         */
        uint64_t nodes_explored = 0;
    };

    //// -------------------------------- ////
    //// Dijkstra's Algorithm Agent Class ////
    //// -------------------------------- ////
    /**
     * @brief Agent class that uses Dijkstra's algorithm to find the shortest path in a maze.
     */
    class dijkstra_agent
    {
    public:
        /**
         * @brief Constructs an agent object than can perform Dijkstra's algorithm search pathfinding on mazes.
         *
         * @result Initializes the agent object.
         */
        dijkstra_agent()
        {
            // nothing to initialize on construction
        }

        /**
         * @brief Runs a Dijkstra's algorithm search to determine the shortest path from the start point to the destination point on the specified maze.
         *
         * @return A deque of index positions denoting a path between the start point and destination point.
         * @cite inspired and translated from: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm#Pseudocode
         * @note Follows the pseudocode closely so best to read that to understand first
         */
        std::deque<uint64_t> &pathfind(const NPC_Racer::maze &race_maze);

        //// Data Members ////
        // Public data members that could be used for purposes other than pathfinding are included here

        /**
         * @param current_position The current position of the agent during pathfinding.
         */
        uint64_t current_position;

        /**
         * @param pathfinding_completed If the pathfinding algorithm is completed or not.
         */
        bool pathfinding_completed = false;

        /**
         * @param path The path found from pathfinding composed of indicies.
         * @note deque used for quick insertion
         */
        std::deque<uint64_t> path = {};

        /**
         * @param nodes_explored count of nodes explored in pathfinding
         */
        uint64_t nodes_explored = 0;
    };

}

////// ============== //////
////// Implementation //////
////// ============== //////

std::deque<uint64_t> &NPC_Racer::depth_first_agent::pathfind(const NPC_Racer::maze &race_maze)
{
    // variables needed for pathfinding
    std::vector<int64_t> previous_nodes;                               // map containing indicies of previous nodes
    std::vector<bool> visited_nodes;                                   // bitmap of visited nodes
    std::deque<uint64_t> path_stack = {race_maze.start_position};      // stack of indicies to explore
    uint64_t next_position;                                            // the index of the next position to be explored
    int64_t previous_position;                                         // index of the previous position to back-track and record the path
    size_t number_directions = race_maze.connected_paths.at(0).size(); // number of neighbors, for 4-way grid should be 4

    // initializing variables
    previous_nodes.resize(race_maze.character_maze.size(), -1);   // all -1 initially because none visited
    visited_nodes.resize(race_maze.character_maze.size(), false); // all false initially

    // looping through and exploring all the paths depth-first (i.e. exploring up branch first)
    while (!path_stack.empty()) // once path stack is empty we've explored as much as we can
    {
        nodes_explored++;                     // increment count
        current_position = path_stack.back(); // pop the back of the stack
        path_stack.pop_back();

        if (current_position == race_maze.destination_position) // guard for when made it to the end
        {
            pathfinding_completed = true;
            break;
        }

        visited_nodes[current_position] = true; // marking node as visited

        // remember the 4 directions in the inner array of connected_paths:
        // 0  1    2    3
        // up down left right
        for (size_t i = 0; i < number_directions; i++) // checking all the edges at the node
        {
            if (race_maze.connected_paths[current_position][i] != -1) // if edge not empty
            {
                // next position is in that direction
                next_position = (uint64_t)race_maze.connected_paths[current_position][i];
                if (visited_nodes[next_position] == false) // if not explored
                {
                    // adds the next position to be explored on the stack
                    path_stack.push_back(next_position);
                    // adds current position to previous position tracker to know where we came from
                    previous_nodes[next_position] = (int64_t)current_position;
                }
            }
        }
    }

    if (current_position != race_maze.destination_position) // if for some reason didn't end on the destination
    {
        // for now a warning will just throw and we'll return a size 1 path (which means no path found)
        std::cout << "WARNING: Path to exit was not found!";
        path = {race_maze.start_position};
        return path;
    }

    previous_position = (int64_t)current_position; // setting to end position to backtrack
    // backtracking from the end position to generate the path
    while (previous_position != -1) // while not at an empty previous path i.e. still backtracking
    {
        path.push_front((uint64_t)previous_position);                // adding node to the path
        if ((uint64_t)previous_position == race_maze.start_position) // if reached the start then end
            break;
        previous_position = previous_nodes[(size_t)previous_position]; // backtracking
    }

    return path;
}

std::deque<uint64_t> &NPC_Racer::dijkstra_agent::pathfind(const NPC_Racer::maze &race_maze)
{
    // variables needed for pathfinding
    std::vector<uint64_t> node_distances;                              // vector containing the distances from the source to each node
    std::vector<uint64_t> queue;                                       // tracks distance if nodes and if nodes have been visited so far (0- (max_int64-1) is in Q, max_int64 is not in Q)
    std::vector<int64_t> previous_nodes;                               // map containing indicies of previous nodes
    std::vector<bool> visited_nodes;                                   // bitmap of visited nodes
    uint64_t next_position;                                            // the index of the next position to be explored
    int64_t previous_position;                                         // index of the previous position to back-track and record the path
    size_t number_directions = race_maze.connected_paths.at(0).size(); // number of neighbors, for 4-way grid should be 4
    uint64_t max_uint64 = std::numeric_limits<uint64_t>::max();        // max size uint64_t, infinite distance or an empty element in the queue
    uint64_t max_uint64_minus_one = max_uint64 - 1;                    // max size uint64_t, infinite distance in the queue
    size_t queue_counter;                                              // tracks non-infinite elements in queue, if is 0 it's empty
    uint64_t distance_through_current;                                 // the distance of next_position through current one

    // initializing variables
    // initially all distances, besides start node, start at infinity (max int)
    node_distances.resize(race_maze.character_maze.size(), max_uint64);
    queue.resize(race_maze.character_maze.size(), max_uint64_minus_one);
    previous_nodes.resize(race_maze.character_maze.size(), -1);   // all -1 initially because none visited
    visited_nodes.resize(race_maze.character_maze.size(), false); // all false initially

    node_distances[race_maze.start_position] = 0; // setting distance of start position
    queue[race_maze.start_position] = 0;          // any time we update distance we also need to update the distance tracker
    queue_counter = race_maze.character_maze.size();

    current_position = race_maze.start_position; // starting at the start node

    // Performing Dijkstra algorithm
    while (queue_counter != 0)
    {
        nodes_explored++; // increment count
        // minimum element of queue, from https://en.cppreference.com/w/cpp/algorithm/min_element
        std::vector<uint64_t>::iterator min_iterator = std::min_element(queue.begin(), queue.end()); // iterator to min element
        current_position = (uint64_t)std::distance(queue.begin(), min_iterator);

        queue[current_position] = max_uint64; // removing element from Q
        queue_counter--;
        visited_nodes[current_position] = true; // marking as explored

        if (current_position == race_maze.destination_position) // guard for when made it to the end
        {
            pathfinding_completed = true;
            break;
        }

        // remember the 4 directions in the inner array of connected_paths:
        // 0  1    2    3
        // up down left right
        for (size_t i = 0; i < number_directions; i++) // checking all the edges at the node
        {

            if (race_maze.connected_paths[current_position][i] != -1) // if edge not empty
            {
                // next position is in that direction
                next_position = (uint64_t)race_maze.connected_paths[current_position][i];
                if (queue[next_position] != max_uint64) // if still in queue
                {
                    distance_through_current = node_distances[current_position] + 1; // the distance of the neighbour from source through current node (all weights are 1 for grid)
                    if (distance_through_current < node_distances[next_position])    // if new distance less than old one
                    {
                        node_distances[next_position] = distance_through_current; // update distance
                        queue[next_position] = distance_through_current;          // update distance in queue
                        // adds current position to previous position tracker to know where we came from
                        previous_nodes[next_position] = (int64_t)current_position;
                    }
                }
            }
        }
    }

    if (current_position != race_maze.destination_position) // if for some reason didn't end on the destination
    {
        // for now a warning will just throw and we'll return a size 1 path (which means no path found)
        std::cout << "WARNING: Path to exit was not found!";
        path = {race_maze.start_position};
        return path;
    }

    previous_position = (int64_t)current_position; // setting to end position to backtrack

    if ((previous_nodes[(uint64_t)previous_position] == -1) or ((uint64_t)previous_position == race_maze.start_position)) // guard for if vertex is non-reachable (no exit from start or end position)
    {
        // for now a warning will just throw and we'll return a size 1 path (which means no path found)
        std::cout << "WARNING: End Vertex is non-reachable";
        path = {race_maze.start_position};
        return path;
    }

    // backtracking from the end position to generate the path
    while (previous_position != -1) // while not at an empty previous path i.e. still backtracking
    {
        path.push_front((uint64_t)previous_position);                // adding node to the path
        if ((uint64_t)previous_position == race_maze.start_position) // if reached the start then end
            break;
        previous_position = previous_nodes[(size_t)previous_position]; // backtracking
    }

    return path; // returns the path
}
[![DOI:10.5281/zenodo.4742687](https://zenodo.org/badge/DOI/10.5281/zenodo.4742687.svg)](https://doi.org/10.5281/zenodo.4742687)
[![arXiv:2105.00613](https://img.shields.io/badge/arXiv-2105.00613-b31b1b.svg)](https://arxiv.org/abs/2105.00613)
[![License: MIT](https://img.shields.io/github/license/bshoshany/thread-pool)](https://github.com/bshoshany/thread-pool/blob/master/LICENSE.txt)
![Language: C++17](https://img.shields.io/badge/Language-C%2B%2B17-yellow)
![File size in bytes](https://img.shields.io/github/size/bshoshany/thread-pool/BS_thread_pool.hpp)
![GitHub last commit](https://img.shields.io/github/last-commit/bshoshany/thread-pool)
[![GitHub repo stars](https://img.shields.io/github/stars/bshoshany/thread-pool?style=social)](https://github.com/bshoshany/thread-pool)
[![Twitter @BarakShoshany](https://img.shields.io/twitter/follow/BarakShoshany?style=social)](https://twitter.com/BarakShoshany)
[![Open in Visual Studio Code](https://img.shields.io/badge/-Open%20in%20Visual%20Studio%20Code-007acc)](https://vscode.dev/github/bshoshany/thread-pool)

# `NPC_Racer`: A Performance Analysis of Game Agent Pathfinding Algorithms


```C++
McMaster University 
CSE 701: Foundations of Modern Scientific Programming
Final Project

```



By Brendan Fallon<br />
Email: [fallonbr@mcmaster.ca](mailto:fallonbr@mcmaster.ca)<br />
GitHub: [https://github.com/bren007pie](https://github.com/bren007pie)<br />

This is the complete documentation for v0.1 of the library, released on Friday December 31, 2022. [TODO: Update before release]

* [Introduction](#introduction)
    * [Motivation](#motivation)
    * [Overview of features](#overview-of-features)
    * [Compiling and compatibility](#compiling-and-compatibility)
* [Getting started](#getting-started)
    * [Including the library](#including-the-library)
    * [Constructors](#constructors)
	* [Visual Studio Code IDE](#visual-studio-code-ide)

* [Submitting and waiting for tasks](#submitting-and-waiting-for-tasks)
    * [Submitting tasks to the queue with futures](#submitting-tasks-to-the-queue-with-futures)
    * [Submitting tasks to the queue without futures](#submitting-tasks-to-the-queue-without-futures)
    * [Manually waiting for all tasks to complete](#manually-waiting-for-all-tasks-to-complete)
    * [Submitting class member functions to the queue](#submitting-class-member-functions-to-the-queue)
    * [Parallelizing loops](#parallelizing-loops)
    * [Loops with return values](#loops-with-return-values)
    * [Parallelizing loops without futures](#parallelizing-loops-without-futures)
* [Helper classes](#helper-classes)
    * [Synchronizing printing to an output stream](#synchronizing-printing-to-an-output-stream)
    * [Handling multiple futures at once](#handling-multiple-futures-at-once)
    * [Measuring execution time](#measuring-execution-time)
* [Other features](#other-features)
    * [Monitoring the tasks](#monitoring-the-tasks)
    * [Pausing the workers](#pausing-the-workers)
    * [Exception handling](#exception-handling)
* [Testing the package](#testing-the-package)
    * [Automated tests](#automated-tests)
    * [Performance tests](#performance-tests)
* [The light version of the package](#the-light-version-of-the-package)
* [About the project](#about-the-project)
    * [Issue and pull request policy](#issue-and-pull-request-policy)
    * [Acknowledgements](#acknowledgements)
    * [Starring the repository](#starring-the-repository)
    * [Copyright and citing](#copyright-and-citing)

## Introduction

### Motivation
Non-playable characters (NPCs) or artificial intelligence (AI) in video games make up a significant portion of the game experience of many games. To simulate intelligent navigation, AI generally will move with respect to a specific target, say for example towards or away from the character. To do this they implement a form of pathfinding to calculate how to efficiently get to the target. Video games are an interesting application because there are timing requirements that state when the program should produce a result. This means that we need to be able to be as computationally efficient as possible when it comes to AI pathfinding. To do this we must pick an algorithm that is as efficient as possible. 

Algorithms for pathfinding have been well-established since the dawn of computing. For example, the travelling salesman problem has many possible solutions \cite{ TravellingSalesmanProblem2022}. In computer science, we like to think of these algorithms as working on a weighted directed graph. Meaning that we have a series of nodes, edges that connect those nodes, weights on each of those edges, and a direction for each edge. For example, we could easily represent a road map as a series of intersections (nodes), roads (edges), road lengths (weights), and whether the street is one-way or two-way (direction). We would start at a certain node and have a destination ending up at another particular node on the graph. The goal is then to figure out which series of edges would be the shortest total weighting to get there.
One of the simplest, but very inefficient, ways to compute the shortest path is to brute force it. You could imagine computing every variation of every path possible from the source to the destination and picking the one with the shortest distance. The time complexity of this is horrible at a big O of $\mathcal{O}(n!)$ where n is the number of nodes in the graph \cite{ TravellingSalesmanProblem2022}. A more efficient algorithm is Dijkstra’s algorithm. In short, Dijkstra looks through paths efficiently by dynamically deciding to stop looking through branches of paths when they have already been visited as well as continuing looking down the shortest branch seen so far first. The performance of this algorithm is already significantly faster than brute force. The complexity is dependent on the data structure used but with arrays you can reach $\Theta(|V|^2)$ and with a priority queue/heap you can reach a  $\Theta(|E| + |V|log|V|)$, where $|V|$ is the number of vertices/nodes and $|E|$ is the number of edges \cite{DijkstraAlgorithm2022}.
However, one flaw of Dijkstra’s algorithm is it doesn’t necessarily consider the if each path moves toward the destination. It could start going off on a series of edges that are short but in completely the wrong direction. In this way, Dijkstra’s algorithm has no heuristic for what a good path is. For highly non-uniform and sparse graphs, such as road networks, this is okay. However, if the graph is a grid, as in the case with video game spaces, then each edge has equal weighting and it must search in all directions because each direction is equally as short until it reaches the destination. To improve this an extension of Dijkstra’s algorithm was made known as the A* pathfinding algorithm. A* uses a heuristic, such as distance to destination, to make sure that each shortest path is in the best direction. For example, with A* you could use the Pythagorean theorem distance from each subsequent node to the destination to pick the next node to follow \cite{computerphileStarSearchAlgorithm2017}. When represented as a tree the performance of the A* pathfinding algorithm is $\mathcal{O}(|E|)$ or $\mathcal{O}(b^d)$ where b is the branching factor and d is the depth of the tree.However, the actual complexity is heavily dependent on the heuristic used \cite{SearchAlgorithm2022, AlgorithmsGraphSearch}. 

NPC racer at its heart will be a project to compare the computational times of common pathfinding algorithms for video games. We will use a simplistic representation of a video game space with paths and barriers which we will call a maze. Then we will measure the time it takes for an agent to compute the shortest path from its starting location to the destination, called a trial. Each of these time trials will be performed multiple times for each of the pathfinding algorithms (brute force, Dijkstra, and A*). Then a statistical analysis, taking the mean and standard deviation, will be done for a series of trials which we will call a run. From there we can compare the runs for each of these different algorithms to see which is the fastest and by how much, which we will call a race. We can then have races on different-sized mazes and with differing amounts of barriers testing the algorithms in differing conditions. In this way, we are racing different agents to see which one is the fastest which is where the name NPC Racer comes from. 

A summary is given below with a diagram 

To summarize here:
-	A maze is a space comprised of paths, barriers, a starting point, and a destination.
-	For each maze, a race is done.
-	For each race, each of the algorithms is tested.
-	For each algorithm, a run is done.
-	For each run, a series of trials are done
-	Each trial is a timed event where the agent solves a maze for the shortest path between the start and the destination.


![See the figure here](figures/NPC_Racer_Hierarchy.png)

The design of this library has four principles. In order of importance they are *consistency*, *portability*, *organization*, and *performance*.
The key to this project will be code *consistency*. First *consistency*: the goal is not to make the fastest pathfinding algorithms but to compare them to each other. As such we have based them all on the same underlying data structures and varied them only in the computation on that structure. 
Second, *portability*: the package only utilizes the C++17 standard library, without relying on any compiler extensions or 3rd-party libraries, and is therefore compatible with any modern standards-conforming C++17 compiler on any platform. Third, *ease of use*: the package is extensively documented, and programmers of any level should be able to use it right out of the box.
Fourthly, *organization*: the library is broken up into modules for each main class and is bundled entirely in its own namespace. And finally, *performance*: while not the main goal performance of core algorithms such as those in the maze class. This provides scalability should the user want to run on even bigger mazes.





### Overview of features


### Compiling and compatibility









## Getting started

### Including the library


### Running with the sample maze
- simply run main.cpp

### Constructors


### Visual Studio Code IDE



## Running agent pathfinding tests on mazes
- the meat of making your own stuff
### Making your own mazes

### Creating agents

### Timed tests

### Putting it all together


## Making your own pathfinding algorithm

### Maze Graphs

### Extending the maze class




## Testing the package

### Automated tests


### Performance tests






## About the project

### Issue and pull request policy


### Acknowledgements


### Starring the repository


### Copyright and citing


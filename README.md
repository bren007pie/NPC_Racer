# `NPC_Racer`: A Performance Analysis of Game Agent Pathfinding Algorithms

[![License: MIT](https://img.shields.io/github/license/bren007pie/NPC_Racer)](https://github.com/bshoshany/thread-pool/blob/master/LICENSE.txt)
![Language: C++20](https://img.shields.io/badge/Language-C%2B%2B20-yellow)
![GitHub last commit](https://img.shields.io/github/last-commit/bren007pie/NPC_Racer)
![NPC Racer Repository Size](https://img.shields.io/github/repo-size/bren007pie/NPC_Racer)
[![GitHub repo stars](https://img.shields.io/github/stars/bren007pie/NPC_Racer?style=social)](bren007pie/NPC_Racer)
![GitHub repo issues](https://img.shields.io/github/issues/bren007pie/NPC_Racer)
[![Open in Visual Studio Code](https://img.shields.io/badge/-Open%20in%20Visual%20Studio%20Code-007acc)](https://vscode.dev/github/bren007pie/NPC_Racer)

```C++
McMaster University 
CSE 701: Foundations of Modern Scientific Programming
Final Project

```

By Brendan Fallon<br />
Email: [fallonbr@mcmaster.ca](mailto:fallonbr@mcmaster.ca)<br />
GitHub: [https://github.com/bren007pie](https://github.com/bren007pie)<br />

- [`NPC_Racer`: A Performance Analysis of Game Agent Pathfinding Algorithms](#npc_racer-a-performance-analysis-of-game-agent-pathfinding-algorithms)
  - [Introduction](#introduction)
    - [Motivation](#motivation)
  - [Getting started](#getting-started)
    - [Compiling and running the Project](#compiling-and-running-the-project)
    - [Mazes](#mazes)
  - [Making your own mazes](#making-your-own-mazes)
  - [Using the project as a library](#using-the-project-as-a-library)
  - [About the project](#about-the-project)
    - [Acknowledgements](#acknowledgements)

## Introduction

### Motivation

Non-playable characters (NPCs) or artificial intelligence (AI) in video games make up a significant portion of the game experience of many games. To simulate intelligent navigation, AI generally will move with respect to a specific target, say for example towards or away from the character. To do this they implement a form of pathfinding to calculate how to efficiently get to the target. Video games are an interesting application because there are non-strict timing requirements that state when the program should produce a result. This means that we need to be able to be as computationally efficient as possible when it comes to AI pathfinding. To do this we must pick an efficient algorithm.

Algorithms for pathfinding have been well-established since the dawn of computing. For example, the travelling salesman problem has many [possible solutions](https://en.wikipedia.org/w/index.php?title=Travelling_salesman_problem&oldid=1128294054#Computing_a_solution). In computer science, we like to think of these algorithms as working on a weighted directed graph. Meaning that we have a series of nodes, edges that connect those nodes, weights on each of those edges, and a direction for each edge. For example, we could easily represent a road map as a series of intersections (nodes), roads (edges), road lengths (weights), and whether the street is one-way or two-way (direction). We would start at a certain node and have a destination ending up at another particular node on the graph. The goal is then to figure out which series of edges would be the shortest total weighting to get there.

One of the simplest, but very inefficient, ways to compute the shortest path is to brute force it. You could imagine computing every variation of every path possible from the source to the destination and picking the one with the shortest distance. The time complexity of this is horrible at a big O of $\mathcal{O}(n!)$ where n is the number of nodes in the graph as stated in the [travelling salesman problem](https://en.wikipedia.org/w/index.php?title=Travelling_salesman_problem&oldid=1128294054#Computing_a_solution).

A more efficient algorithm is Dijkstra’s algorithm. In short, Dijkstra looks through paths efficiently by dynamically deciding to stop looking through branches of paths when they have already been visited as well as continuing looking down the shortest branch seen so far first. The performance of this algorithm is already significantly faster than brute force. The complexity is dependent on the data structure used but with arrays you can reach $\Theta(|V|^2)$ and with a priority queue/heap you can reach a $\Theta(|E| + |V|log|V|)$, where $|V|$ is the number of vertices/nodes and $|E|$ is the number of edges according to [this](https://en.wikipedia.org/w/index.php?title=Dijkstra%27s_algorithm&oldid=1127202995).

However, one flaw of Dijkstra’s algorithm is it doesn’t necessarily consider the if each path moves toward the destination. It could start going off on a series of edges that are short but in completely the wrong direction. In this way, Dijkstra’s algorithm has no heuristic for what a good path is. For highly non-uniform and sparse graphs, such as road networks, this is okay. However, if the graph is a grid, as in the case with video game spaces, then each edge has equal weighting and it must search in all directions because each direction is equally as short until it reaches the destination.

To improve this an extension of Dijkstra’s algorithm was made known as the *A** pathfinding algorithm. *A** uses a heuristic, such as distance to destination, to make sure that each shortest path is in the best direction. For example, with *A** you could use the Pythagorean theorem distance from each subsequent node to the destination to pick the next node to follow as shown in [this video](https://www.youtube.com/watch?v=ySN5Wnu88nE). When represented as a tree the performance of the A* pathfinding algorithm is $\mathcal{O}(|E|)$ or $\mathcal{O}(b^d)$ where b is the branching factor and d is the depth of the tree. However, the actual complexity is heavily dependent on the heuristic used according to [this](https://en.wikipedia.org/w/index.php?title=A*_search_algorithm&oldid=1127312005) and [this](https://cs.stackexchange.com/questions/56176/a-graph-search-time-complexity).

NPC racer at its heart will be a project to compare the computational times of common pathfinding algorithms for video games. We will use a simplistic representation of a video game space with paths and barriers which we will call a maze. Then we will measure the time it takes for an agent to compute the shortest path from its starting location to the destination, called a trial. Each of these time trials will be performed multiple times for each of the pathfinding algorithms (brute force, Dijkstra, and A*).

Then a statistical analysis, taking the mean and standard deviation, will be done for a series of trials which we will call a run. From there we can compare the runs for each of these different algorithms to see which is the fastest and by how much, which we will call a race. We can then have races on different-sized mazes and with differing amounts of barriers testing the algorithms in differing conditions. In this way, we are racing different agents to see which one is the fastest which is where the name NPC Racer comes from.

A summary is given below with a diagram.

To summarize here:

- A maze is a space comprised of paths, barriers, a starting point, and a destination.
- For each maze, a race is done.
- For each race, each of the algorithms is tested.
- For each algorithm, a run is done.
- For each run, a series of trials are done
- Each trial is a timed event where the agent solves a maze for the shortest path between the start and the destination.

![See the figure here](figures/NPC_Racer_Hierarchy.png)

The design of this library has four principles. In order of importance, they are *consistency*, *portability*, *organization*, and *performance*.

The key to this project will be code *consistency*. First *consistency*: the goal is not to make the fastest pathfinding algorithms but to compare them to each other. As such we have based them all on the same underlying data structures and varied them only in the computations on that structure.

Second, *portability*: the package only utilizes the C++20 standard library, without relying on any compiler extensions or 3rd-party libraries, and is therefore compatible with any modern standards-conforming C++20 compiler on any platform.

Third, *ease of use*: the package is extensively documented, and programmers of any level should be able to use it right out of the box.

Fourthly, *organization*: the project is broken up into modules for each main class and is bundled entirely in its own namespace. And finally, *performance*: while not the main goal performance of core algorithms such as those in the maze class. This provides scalability should the user want to run on even bigger mazes.

## Getting started

### Compiling and running the Project

The project was made to be portable and all files should compile independently regardless of the IDE. That being said the project was only tested and compiled on Windows 10 and with a version of the GCC compiler with C++20 with compiler arguments -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -Wpedantic -std=c++20 without warnings (with GCC v12.2.0). To compile the project in Windows PowerShell with GCC, assuming g++ is in your path, use:

```PowerShell
g++ main.cpp -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -Wpedantic -std=c++20 -o NPCRacer.exe
```

and it should compile without any errors. Assuming it compiled without errors you should now have the file `NPCRacer.exe` in your directory. To test running the project you can run with the two example mazes provided in the parent directory. Start with the [text maze](10_10_test_maze.txt):

```PowerShell
.\NPCRacer.exe 10_10_test_maze.txt
```

Expected output:

TODO: Update expected outputs when the program is done

```PowerShell
Maze `10_10_test_maze.txt` has been successfully read!
. . . . . . . . . .
. . @ . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . = =
. . . . . . . . . .
. . . . . . . + . X
```

You can also try the [CSV maze](10_10_test_maze.csv):

```PowerShell
.\NPCRacer.exe 10_10_test_maze.csv
```

Expected output:

TODO: Update when the program is done

```PowerShell
Maze `10_10_test_maze.csv` has been successfully read!
+ = = = = = = = = +
= X . . . . . . . =
= . . . . . . . . =
= . . . . . . . . =
= . . . . . . . . =
= . . . . . . . . =
= . . . . . . . . =
= . . . . . @ . . =
= . . . . . . . . =
+ = = = = = = = = +
```

### Mazes

TODO: Update for user input version of the program.

Mazes files are in the .txt or .csv format and must be located in the same directory as the executable. By default two are provided in the project directory called [10_10_test_maze.csv](10_10_test_maze.csv) and [10_10_test_maze.txt](10_10_test_maze.txt). Mazes come in a plain text format to easily read and edit small grids. The comma-separated value (CSV) format is for convenient editing in a GUI program such as Microsoft Excel. This makes editing large mazes easy by copying and pasting many elements.

Several more mazes are provided in the sample_mazes folder. Parsing any of these starts by bringing the file into the same directory as the executable. For example, let's start by parsing the [10_10_small_maze.txt](sample_mazes\\10_10_small_maze.txt) file. To parse this file include the ["mazes.hpp"](mazes.hpp) file and call the `NPC_Racer::maze`class constructor with the file name, including the extension, as a string. All classes and functions in the project will be in the `NPC_Racer` namespace.

For example

```C++
#include "mazes.hpp"

int main()
{
NPC_Racer::maze small_maze("10_10_small_maze.txt");
}

```

Expected output:

```Bash
Maze `10_10_small_maze.txt` has been successfully read!
```

If there is a problem opening the file or it is not in the same directory as the executable you may get this warning message and check where your files are.

Warning message:

```Bash
ERROR: Error opening input file `10_10_small_maze.txt`!
Does this file exist in the current working directory?
```

To print this out simply call the `.print_maze()` method or you can use the `<<` operator to output this maze to a stream such as `std::cout`.

```C++
#include "mazes.hpp"
#include <iostream>

int main()
{
NPC_Racer::maze small_maze("10_10_small_maze.txt");
std::cout << small_maze; // small_maze.print_maze() works too!
}

```

Expected output:

```Bash
Maze `10_10_small_maze.txt` has been successfully read!
+ - - - - - - - - +
| . . # . . . X . |
| . . . . # . # . |
| . # # # # # # # |
| . . . . . . . . |
| . # # # . # . # |
| . # . . . # . . |
| . # . # . # # . |
| . # @ # . . . . |
+ - - - - - - - - +
```

Congratulations, you've successfully parsed your first maze!

## Making your own mazes

To make your own maze I would suggest starting with an empty maze and filling it in. The `NPC_Racer::make_empty_maze_file` function lets you make an n x m maze filled with free spaces in a space-separated or comma-separated format.

```C++
NPC_Racer::make_empty_maze_file(35, 34, true);
```

output:

```bash
Empty maze file `35_34_empty_maze.csv` was successfully created.
```

will make a 35-row-by-34-column CSV maze file called "35_34_empty_maze.csv" in the same folder as your executable. While

```C++
NPC_Racer::make_empty_maze_file(100, 100, false);
```

```bash
Empty maze file `100_100_empty_maze.txt` was successfully created.
```

will make a 100 by 100 empty text maze file.

From there you can edit each maze in your favourite CSV or text editor.

The format of each maze is as follows:

- Mazes elements are made up of single characters
- Columns are separated by a space character ' ' (.txt files) or a comma character ',' (.csv files) and rows are separated by a newline character '\n'.
- The final character before the end of the file is a newline character '\n'.
- A period '.' denotes a free path space where agents can move.
- An 'X' or 'x' character designates the destination position.
- A '@' character designates the starting position.
- A maze must contain one and only one destination position and source position.
- Any other character is interpreted as a barrier space where the agent cannot move.

Feel free to go crazy. The project should accept mazes up to the limit of a signed 64-bit integer. Although making a maze that size may take some time and you will almost definitely run out of hard drive space unless you're Google.

## Using the project as a library

If you wish to extend the code, say with your own pathfinding algorithm, you are more than welcome to. No particular IDE is needed but I suggest an editor with a code spell checker, C++/Markdown linters, as well as rulers to make sure you don't go over 120 characters per line. Here is a brief overview of how the code is structured: #TODO Do or remove

## About the project

### Acknowledgements

A big thank you to Dr. Shoshany for his teaching on C++. Much of this project's style is based on teaching from that course. This documentation was forked off of the documentation on their [thread-pool library](https://github.com/bshoshany/thread-pool).

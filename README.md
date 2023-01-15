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
      - [Compiling Optimization](#compiling-optimization)
  - [Mazes](#mazes)
  - [Making your own mazes](#making-your-own-mazes)
    - [Using the Parser to check your maze](#using-the-parser-to-check-your-maze)
  - [Observations](#observations)
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

There are several things this library has not designed for. One *working with maze images*: as this would mean making my own image library or importing one, which would go against the *portability* principle.

Secondly, *speed of execution*: there are much faster versions of all the pathfinding algorithms used. But for consistency, they've been modified to use arrays instead of faster abstract data types such as minimum-priority queues. The rest of the program should be fairly efficient except for the parser which was designed for error checking.

Thirdly, *memory conservation*: While the project makes use of memory safe, by making use of C++ STL containers, the project has not been optimized for tracking how much memory is used. Most of the algorithms make array copies of the mazes but this has not been a problem within the testing of the example mazes. Even running the largest example maze the program did not use over 5 MB of memory according to the Windows 10 task manager.

## Getting started

### Compiling and running the Project

The project was made to be portable and all files should compile independently regardless of the IDE. That being said the project was only tested and compiled on Windows 10 and with a version of the GCC compiler with C++20 with compiler arguments -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -Wpedantic -std=c++20 without warnings (with GCC v12.2.0). To compile the project in Windows PowerShell with GCC, assuming g++ is in your path, use:

```PowerShell
g++ main.cpp -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -Wpedantic -std=c++20 -o NPCRacer.exe
```

and it should compile without any errors. Assuming it compiled without errors you should now have the file `NPCRacer.exe` in your directory. To test run the project you can run with the small example maze example mazes provided in the sample_mazes directory. Start by copying the [10x10 test maze](sample_mazes/10_10_test_maze.txt) into the same directory as the executable. Then run the program with the input:

```PowerShell
.\NPCRacer.exe 10_10_test_maze.csv
```

Expected output:

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
Running depth-first search pathfinding.
Depth-first pathfinding complete, path was successfully found!
Printing path on maze `10_10_test_maze.csv`
+ = = = = = = = = +
= X P P P P P P P =
= P P P P P P P P =
= P P P P P P P P =
= P P P P P P P P =
= P P P P P P P P =
= . . . . P P P P =
= P P P P P @ P P =
= P P P P P P P P =
+ = = = = = = = = +

Running Dijkstra's algorithm pathfinding.
Dijkstra's algorithm pathfinding complete, path was successfully found!
Printing path on maze `10_10_test_maze.csv`
+ = = = = = = = = +
= X P P P P P . . =
= . . . . . P . . =
= . . . . . P . . =
= . . . . . P . . =
= . . . . . P . . =
= . . . . . P . . =
= . . . . . @ . . =
= . . . . . . . . =
+ = = = = = = = = +


## RACE RESULTS ##.
Maze Parsing time: 0.0009089

# Depth-first pathfinding #
Average depth-first pathfinding time: 2.69364e-05 seconds
Depth-first pathfinding standard deviation: 1.1867e-05 seconds

# Dijkstra's algorithm pathfinding #
Average Dijkstra pathfinding time: 0.000146209 seconds
Dijkstra pathfinding standard deviation: 4.35045e-05 seconds

# Summary Table #
Race on maze: '10_10_test_maze.csv'
        | Depth-first   | Dijkstra's    | Algorithm
--------|---------------|---------------|
Winner  | WINNER        |               |
Average | 2.69364e-05   | 0.000146209   |
% diff. | 0      %      | 137.771711 %  |
STDDEVP.| 1.1867e-05    | 4.35045e-05   |
--------|---------------|---------------|
Value
^average and standard deviation time is in seconds

Total program time: 0.0109164 seconds
```

You feel free to try any of the mazes in the sample mazes folder or make your own by seeing the [Making your own mazes](#making-your-own-mazes) section. If you don't want to copy the maze to the current working directory each time you can also run the program with an absolute path to the maze file. For Example on Windows 10 Powershell:

```PowerShell
.\NPCRacer.exe "C:\\Path\\to\\repository\\NPC-Racer\\sample_mazes\\10_10_test_maze.csv"
```

Powershell path strings need double \\ on Windows 10 and to be surrounded by "s.

Currently, the program only runs one race at a time on one maze.

#### Compiling Optimization

When working on small mazes it's best to not have the compiler optimize the executable. This is because for small mazes the timing is already very quick so going any faster may introduce floating point error. However, when working with large mazes such as the [301 x 201 maze](sample_mazes/301_201_delorie_generated_maze.csv) I found it takes a couple of minutes to run on my computer. If you find this to be the case I would suggest adding the -O2 optimization argument to the compiler. For example on Windows 10 Powershell:

```PowerShell
g++ main.cpp -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -Wpedantic -std=c++20 -O2 -o NPCRacer.exe
```

For me, this reduced the run time of the large maze by as much as 5 times! But you will also need to be careful to only compare races done with optimized or unoptimized code.

## Mazes

Mazes files are in the .txt or .csv format and must be located in the same directory as the executable. By default, there should be 10 mazes provided in the folder called `sample_mazes`. For example [10_10_test_maze.csv](sample_mazes/10_10_test_maze.csv) and [10_10_test_maze.txt](sample_mazes/10_10_test_maze.txt). Mazes come in a plain text format to easily read and edit small grids. The comma-separated value (CSV) format is for convenient editing in a GUI program such as Microsoft Excel. This makes editing large mazes easy by copying and pasting many elements.

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

- Mazes' sizes must be at the top.
  - For `.txt` files they are separated by a space with no whitespace after
  - For `.csv` files they are separated by a comma and have an extra (columns – 2) ','s (Excel will save it this way)
- Mazes elements are made up of single characters
- Columns are separated by a space character ' ' (.txt files) or a comma character ',' (.csv files) and rows are separated by a newline character '\n'.
- The final character before the end of the file is a newline character '\n'.
- A period '.' denotes a free path space where agents can move.
- An 'X' or 'x' character designates the destination position.
- A '@' character designates the starting position.
- A maze must contain one and only one destination position and source position.
- Any other character is interpreted as a barrier space where the agent cannot move.

### Using the Parser to check your maze

It may also help to make sure your maze formatting is correct by parsing them. Parsing any of these starts by bringing the file into the same directory as the executable. For example, let's start by parsing the [10_10_small_maze.txt](sample_mazes/10_10_small_maze.txt) file. To parse this file include the ["mazes.hpp"](mazes.hpp) file and call the `NPC_Racer::maze`class constructor with the file name, including the extension, as a string. All classes and functions in the project will be in the `NPC_Racer` namespace.

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

Feel free to go crazy. The project should accept mazes up to the limit of a signed 64-bit integer. Although making a maze that size may take some time and you will almost definitely run out of hard drive space unless you're Google.

## Observations

Based on simple testing some results are already clear. A single-pass depth-first search algorithm finds a path faster but the path is significantly longer. Meanwhile, Dijkstra's algorithm is slower but is guaranteed to make the shortest path possible. If the depth-first search had to search until it found the shortest maze it would take much longer I suspect.

For Example:
Depth-first path -

```Powershell
+ = = = = = = = = +
= X P P P P P P P =
= P P P P P P P P =
= P P P P P P P P =
= P P P P P P P P =
= P P P P P P P P =
= . . . . P P P P =
= P P P P P @ P P =
= P P P P P P P P =
+ = = = = = = = = +
```

Dijkstra's algorithm path -

```Powershell
+ = = = = = = = = +
= X P P P P P . . =
= . . . . . P . . =
= . . . . . P . . =
= . . . . . P . . =
= . . . . . P . . =
= . . . . . P . . =
= . . . . . @ . . =
= . . . . . . . . =
+ = = = = = = = = +
```

The depth-first path, denoted by `P`s goes all over the place where as Dijkstra's algorithm goes straight from the source to the destination.

There is also a big difference between sparse empty spaces and perfect mazes. The perfect mazes are mazes that only have 1 valid path through them. An example of a sparse maze is the [100 x 100 sparse maze](sample_mazes/100_100_sparse_ascii_maze.csv) while an equivalent sized perfect maze is the [101 x 101 perfect maze](sample_mazes/101_101_dcode_generated_maze.csv).

Example results for the sparse maze:

```Powershell
# Summary Table #
Race on maze: '100_100_sparse_ascii_maze.csv'
        | Depth-first   | Dijkstra's    | Algorithm
--------|---------------|---------------|
Winner  | WINNER        |               |
Average | 0.00780169    | 2.10724       |
% diff. | 0      %      | 198.524534 %  |
STDDEVP.| 0.00358888    | 0.669563      |
--------|---------------|---------------|
Value
^average and standard deviation time is in seconds
```

Example results for the perfect maze:

```Powershell
# Summary Table #
Race on maze: '101_101_dcode_generated_maze.csv'
        | Depth-first   | Dijkstra's    | Algorithm
--------|---------------|---------------|
Winner  | WINNER        |               |
Average | 0.00146496    | 0.506315      |
% diff. | 0      %      | 198.845986 %  |
STDDEVP.| 0.000475519   | 0.00912798    |
--------|---------------|---------------|
Value
^average and standard deviation time is in seconds
```

Comparing the runtime of these two mazes we can see that the perfect maze takes significantly less time because the search space is much smaller, i.e. has more barriers. The depth-first single-pass algorithm is also faster and finds the correct path because there is only one. In both cases, the single-pass depth search is 200% faster but only finds the shortest path on the perfect maze. This means that for open spaces, such as those NPCs would find in a video game, a shortest path algorithm like Dijkstra's algorithm would produce a more realistic result. But for a perfect maze solver, a single-pass depth solver would be better.

## Acknowledgements

A big thank you to Dr. Shoshany for his teaching on C++. Much of this project's style is based on teaching from that course. This documentation was forked off of the documentation on their [thread-pool library](https://github.com/bshoshany/thread-pool).

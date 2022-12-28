# NPC Racer Developer Documentation

```C++
McMaster University 
CSE 701: Foundations of Modern Scientific Programming
Final Project
NPC Racer
A Performance Analysis of Game Agent Pathfinding Algorithms
```


### Purpose

This document is for developers. It acts as not only a documentation of my design and syntax decisions but as a history and work cited. I'll take you through what I initially thought, what I may have found online, and my naive reasoning for choosing one way over another.

### Code Structure


- Trying to keep the character per line down to 80 so fits on most screens but maxing out at 120 character (<https://softwareengineering.stackexchange.com/questions/604/is-the-80-character-limit-still-relevant-in-times-of-widescreen-monitors>)

Design philosophy for this project is going to be consistency over performance. The goal is to make sure each program is compared fairly so must use the same C++ technologies to get a comparable result. 

#### Naming

I decided to have all the functions outside of the `main.cpp` in their own namespace. This makes it clear what is from NPC_Racer and what is from the standard library.

Also, the name of the project is NPC Racer but for places where whitespace is not allowed (C++ names/Github repositories) and underscore is used for the whitespace.


Variable Naming rules 
1. Don't abbreviate names.
2. Don't put types in names.
3. Add units unless the type tells you.
4. Don't put types in your types (e.g. AbstractX, BaseX)
5. Refector if you find yourself naming code "Utils"/"helper"

Based on this video by [code aesthetic](https://youtu.be/-J3wNP6u5YU).


### Code Timeline

- We need maze parser and maze class ASAP (tonight)
- We need to carry laptop and work on it everywhere including drive to rocket launch. 
- Friday the 30th should be all polish (finishing touches on documentation) 
- Algorithms are going to take longer than we think. Might be good idea to do one in-depth (brute force) first with statistics just so we have at least one version working.
- Statistics shouldn't be too bad but will probably take longer than we expect.
- Write user documentation as you complete sections (refer to ThreadPool)

### Mazes
Mazes offer a great testing point, especially for games, because they are densely populated, have many branching paths, and are easy to generate. Perfect mazes are one that have only one solution. Our mazes don't need to be perfect but there should be a shortest or at least equally short path from the start node to the target node.

Our maze structure will be made up of ASCII/UTF-8 characters with `.` denoting a free space, `@` denoting the starting location of the agent, and `X` or `x` denoting the destination of the agent. Any other single symbol, besides a comma, will be treated as a wall or barrier. Therefore a valid path would travel along `.` from the `@` symbol to the `X` symbol.  For consistency only each space or barrier must only be one character to make fixed-width printing easier. 

There will be two storage types, space delimited text files and comma separated value (CSV) files. The text file format makes it easy for making small mazes in a text editor whereas the CSV files are meant to be opened in a CSV editor like Excel. There large structures can be copy pasted to make full mazes or copy generated mazes with some basic manipulation. Since they're both delimited files they are more memory expensive than fixed-width files but much more readable and editable using the existing tool set.


#### Maze Generation

Mazes are well defined and there are many algorithms to [generate mazes](https://stackoverflow.com/questions/38502/whats-a-good-algorithm-to-generate-a-maze). 

We could make it so mazes generate on the fly but that is out of the scope of this work so for now we'll rely on other tools and generate a fixed set of mazes.

Many people have made maze generators:
- https://thenerdshow.com/maze.html
- https://gist.github.com/gmalmquist/2782000bd6b378831858
- https://users.csc.calpoly.edu/~zwood/teaching/csc471/finalproj010s1/ritam/
- https://www.vidarholen.net/cgi-bin/labyrinth?w=50&h=50

But for our purposes we want it to be only ascii character with space or a . as the empty space. There are two I found:
1. https://www.dcode.fr/maze-generator
2. https://www.delorie.com/game-room/mazes/genmaze.cgi


The first only generates a 50 x 50 maze (101 x 101 matrix) while the second generates a 100 x 100 maze (301 x 201 matrix). Using these in our CSV format with Excel we could easily import these mazes to be solved.
- Split in Excel using fixed width text to column (use wizard if small, if large use formulas), could automate again using Python or C++ but we're doing this just to get a couple mazes quickly.
- Excel uses text parser with string to column convert with `=MID($A2,B$1,1)` converter to select a character.
- If empty spaces are the path use find and replace to replace empty space ` ` with `.`. 

#### Maze class
- for now maze class will read from a file, future work you can make empty maze constructor and add elements to it.
- Much like the matrix file we'll be using flattened 2D arrays but with with only need for bool version of the bitmap and char version.
- All the operations will happen on the bitmap version for speed but for any printouts the char map will be used. 
- We'll use iterators to point to the starting and ending destination.
- May be helpful to compute adjacency map that states which directions we can go for movement in some encoding (e.g. at each location store a string comprised of `L`, `R`, `U`, `D` denoting which way you can move.

- Q) Will fileIO work if you give it the full path to the file? I.e. "D:\Files\bflaw\Documents\Git\NPC-Racer\100_100_empty_maze.csv"

- Cannot do certain operations with files: https://www.mtu.edu/umc/services/websites/writing/characters-avoid/

##### Maze Constructor 

- Maze parser/constructor takes chars in one at a time, needs to see whitespace to get count of columns (to confirm), then newline or EOL to count rows. 
- Throws exceptions and says error where problems.
- Constructor saves char, Bool (1 if space, iterators if X or @, 0 if anything else in proper format).
- Once all populated and verified it saves the rows and columns count from the star.

##### Maze Member Functions
- At (with checking)
- Bracket accessor (without error checking)
- Check if can move (uses at method to check if in position)
- Extraction operator and derivative print maze to print to a stream

#### Maze Graphs

- General idea of pathfinding is to compute every possible permutation and then pick the shortest one. 
- How do we do that? 
- Well, we could find every path in an adjacency map or graph? 
- Q) Is there a graph data structure in C++, maybe algorithms?
- Might want to make a graph struct or a class that contains. Nodes have:
- i) node named the position
- ii) a vector named connected that has pointers/references to other nodes. 
- iii) optional cost/distance. For grids they are always 1
- Like a tree but not really b.c. there's no parent or child relationship. 
- Default constructor takes position and stores empty connected list. 
- For each 1 in the map we start by assigning it an empty constructor (so that all the node variables are initialized. 
- Then we go to each position and check every direction to see if it's empty to see if connected. If connected add reference to that node to the connected vector. Then we do the same thing for the next position until every node is populated. Note this has lots of redundancy of data but means we can travel from any direction and find connected nodes. 
- Web then store that graph in the mazeclass as a graph (vector containing all node objects) or a map (storing as tuple for each position) 


### Agents 
Agents are a generalized entity that for NPC Racer only work on mazes to generate paths. The way they do that differs between what kind of agent they are but they have some things in common. 

#### Prototype Agents
##### Movement
- can agents move on the diagonals? 
- Initially I said yes but the distance would be $\sqrt(2)$. This would also mean they could cut corners (i.e. could move past a maze wall w.o. having to go in the corner). 
- However, this addition not only increases the state space of possible paths but means adding irrational floats (assuming number is irrational in binary) which may lead to rounding errors with so many additions. Better to have distances only be 1. 
- As such then they can only move in the four cardinal directions and will not be able to cut corners. The datatype will then be type uint16_t, should have no floats, for the distance.
- This will generally make the paths longer and may add cases where some solutions are no longer unique (e.g. where destination is in an open diagonal). But that is okay, solutions didn't need to be unique anyways. 

#### Brute Force Agent

- We want to compute every possible path now. 
- We need a notion of branching pathways, for that we can probably use recursion! 
- What were generating here is paths (has a set of paths and a total distance (which for grids is a count of paths). This could be it's own struct. 
- So, what we need is a branch path function that takes a starting node and a path we've already taken. It then creates a path from the starting node and recursively calls itself for each connected path in the list. The base/return condition of when it finds the destination node, and then it returns the path travelled to be appended to the previous path travelled. In theory this should explore every possible path combination (due to exploring every connected node in the array). 
- Will need to test this on a small square maze (maybe 3x3 with start in the middle and destination bottom right). We should see every path including the one that goes and visits every other node before getting to the destination (spiral longest path)
- We probably should reserve memory for the path container as this will probably be very big. 
- Q) How many paths will this generate? N!?

### Testing

We will be writing incremental and testing as we go. Ideally we do formal unit tests of each function with a formal system but for now it will all be done by hand.





### Constructors

#### String Infinite Integer Input



### Member Functions
Any class member functions


### Overloaded Operators

Any overloads for any of the classes



### Class Invariants
Any invariants for any of the class.

### Class Exceptions
Any exceptions for any of the class.


### VSCode Notes

- this code was written in the Visual Studio Code (VSCode) IDE. While you to not need this IDE to follow the conventions/style it certainly helps to follow my settings. If you are familiar with VSCode these will make sense to you, if not then they will not
- Have C++ save on format on in the settings.
- Make sure to have code spell-checker on.
- ctrl+shift+0 brings up full tooltip/tooltip at cursor position. This is very useful to see the overloads and full type signatures of an operator or function.
- You can make rules to make sure you adhear to the 80-120 character limit. In the VSCode UI settings search for "rulers" and add this entry to the settings.json file:

```json
"editor.rulers": [
        80,
        120
    ],
```

- I would also suggest syncing your VSCode settings if you use multiple computers to make it easier.

### Git Notes
- if renamed remote repository can change origin url to moved url with:
>`git remote set-url origin https://github.com/bren007pie/NPC_Racer.git ` 

[https://docs.github.com/en/get-started/getting-started-with-git/managing-remote-repositories]

### Future Work


- time line, requirements, and testing plan in project proposal

- Optimizing to see how fast we can make it go

- Static target vs dynamic (randomly) moving target.
- Could display using fancier graphics 
- Testing multiple algorithms and scenarios including: 
	+ Hierarchical path finding
	+ D* Alogrithm
	+ Multi-agent pathfinding

- Write docs in Pandoc markdown and then convert to Latex to make fancy PDF/wiki/html

#### Display
-  have ASCII display AI moving along path in a terminal stop-motion or visualized in Python
-  Integrate into a text-based adventure to show speedup of agents. 
- could write in Python to compare

#### Mazes
- Constructors and setters where you can make empty maze with a constructor and add elements to it through object setters. 
- - Allow make_empty_maze_file() to define name of the empty maze (would need to be checked) and location to write to (would need to check if it exists).
- Have mazes be generated in the program or at least auto parse other mazes
- Also could include going up levels, i.e. 3D maze

##### Maze Parser
- For maze parser check to see if file exists first and throw exception if it doesn't
- For maze file header parser use getline from <strings> so that doesn't run into any problems if numbers in cells further down the header.
- Make maze parser so doesn't have to be fixed width and can have multi-character symbols (use getline from <strings> with delimiter of ',' or ' ')


### Hand-in Checklist

- [ ] Remove all TODOs
- [ ] Remove print statements, debug prints, and unused commented code
- [ ] Clean up documentation, spell/grammar check, and Pandoc to Tex/HTML
- [ ] Run through Grammarly to check
- [ ] Make sure all lines under 120 character long

Program guidelines I need to double check:

- Make sure code snippets show when they begin and end in terms of inspiration
- Avoid common errors in the big red boxes
- detect and handle errors, including: user errors (e.g. the user entered a string where they should have entered a number),
 file errors (e.g. invalid file format), and system errors (e.g. out of memory),
 and terminate the program with an informative error message, e.g. "Invalid input on line 7 of input.txt: Expected a number".

- comments explaining each function (via doxygen comments)
- Must include clear external documentation, explaining relevant equations,algorithms, and concepts.
 must include several examples of inputs, interests, and outputs (Markdown/pandoc)
- Your code and documentation should be free of typos and grammar issues.

For Project

- Comments in Doxygen format must be written for all classes, functions, and variables.
- All code must be properly formatted for maximum readability. I highly recommend using VS Code's automatic formatting feature (Shift+Alt+F) and even set it up to automatically format code on save, as explained in the lecture note
- object-oriented, which means objects and their member variables and functions should be used whenever possible, and correctly implement the object-oriented techniques we learned in class, such as encapsulation and class invariants
- may use another programming language, such as Python or Mathematica, to visualize the output of your C/C++ program if desired

### Work Checklist

Proposal
- [X] Copy all written so far into word document
- [X] Organize and pairdown that information
- [X] Write theory/background
- [X] Write main description
- [X] Make the module interface diagram
- [X] Write implementation sections
- [X] Edit pass with grammarly
- [X] Get citations in Zotero/.bib file auto generation
- [X] Copy to Latex and input citations
- [X] Send Email
- [X] Remove self notes and add all user documentation peace meal in the readme.md 
Mazes
- [X] Make empty maze generator
- [X] Fix CSV maze commas in header
- [X] Generate some mazes
- [X] Rename MazeIO and put in Mazes class
- [X] Make figure out maze data members/if you can steal from matrix class
- [ ] Make maze parser
	- [X] Sanitize filename string of trailing and leading whitespace
	- [X] Check file extension
	- [ ] Define exceptions 
	- [X] Read from file
	- [ ] Write down maze parser
	- [X] Implement maze parser
	- [ ] Read through Dr. Shoshany Threadpool 
	- [ ] Write user blurb on maze parser
- [ ] Figure out graph 




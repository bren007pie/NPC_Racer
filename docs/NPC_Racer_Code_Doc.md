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

### Testing

We will be writing incremental and testing as we go. Ideally we do formal unit tests of each function with a formal system but for now it will all be done by hand.



### Naming

I decided to have all the functions outside of the `main.cpp` in their own namespace. This makes it clear what is from NPC_Racer and what is from the standard library.

Also, the name of the project is NPC Racer but for places where whitespace is not allowed (C++ names/Github repositories) and underscore is used for the whitespace.


Variable Naming rules 
1. Don't abbreviate names.
2. Don't put types in names.
3. Add units unless the type tells you.
4. Don't put types in your types (e.g. AbstractX, BaseX)
5. Refector if you find yourself naming code "Utils"/"helper"

Based on this video by [code aesthetic](https://youtu.be/-J3wNP6u5YU).

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

### Future Work

- Optimizing to see how fast we can make it go
- Allow make_empty_maze_file() to define name of the empty maze (would need to be checked) and location to write to (would need to check if it exists).

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

- [X] Make empty maze generator
- [ ] Copy all written so far into word document
- [ ] Make maze parser
- [ ] 


### Work Log
- unknown hours working on project outline
- Sunday December 18, 2022 - 2 hr organizing project and making empty maze generator


Total Hours: 2 hr


/**
 * @file main.cpp
 * @author Brendan Fallon (fallonbr@mcmaster.ca) (https://github.com/bren007pie)
 * @brief This module reads mazes from text files and writes them
 * @version 0.1
 * @date Sunday December 18, 2022
 * @copyright Copyright (c) 2022 Brendan Fallon
 */

//// Preprocessor Directives ////
#include <iostream> // terminal output
#include <fstream>  // file output
#include <string>   // string manipulation

////// ========= //////
////// Interface //////
////// ========= //////

namespace NPC_Racer // using namespace across multiple files: https://stackoverflow.com/questions/4093407/c-namespaces-and-defining-classes-in-separate-files
{
    /**
     * @brief Writes an empty maze with `.` for every element to a text file.
     *
     * @param rows The number of rows of `.`.
     * @param columns The number of columns of `.`.
     * @param comma_separated If true will output in a comma separated file (`.csv`) format
     *      which is easier for editing in Excel. By default is false.
     * @result Writes to a file called `rows_cols_empty_maze.txt` to the current working directory.
     * @warning Overwrites any previous file with the same name. Advise @b renaming each file.
     */
    void make_empty_maze_file(const size_t &rows, const size_t &columns, const bool comma_separated = false);
}

////// ============== //////
////// Implementation //////
////// ============== //////

// default values can't be reassigned in the implementation apparently: https://stackoverflow.com/questions/2545720/error-default-argument-given-for-parameter-1
void NPC_Racer::make_empty_maze_file(const size_t &rows, const size_t &columns, const bool comma_separated /* false */)
{
    // based on https://baraksh.com/CSE701/notes.php#io-streams-and-files
    // Would let name but then would have to check for valid name types, will be future work.

    // making the file name `rows_cols_empty_maze.txt/.csv`
    std::string file_name;
    if (comma_separated)
        file_name = std::to_string(rows) + "_" + std::to_string(columns) + "_empty_maze.csv";
    else
        file_name = std::to_string(rows) + "_" + std::to_string(columns) + "_empty_maze.txt";

    std::ofstream output(file_name);
    if (!output.is_open()) // checks to make sure file opens correctly, if not exists
    {
        std::cout << "Error opening output file!";
        exit(EXIT_FAILURE);
    }

    // writing size header in first line
    std::string maze_size = std::to_string(rows) + "," + std::to_string(columns) + "\n";
    output << maze_size;

    // empty space string with or without the comma
    std::string empty_space;
    if (comma_separated)
        empty_space = ".,"; // if in csv no whitespace needed
    else
        empty_space = " ."; // standard maze space has a single whitespace for readability

    // looping through writing a 2D maze of empty spaces
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            output << empty_space;
        }
        output << "\n"; // Always need the newline character
    }
    output.close(); // Don't forget to close
    std::cout << "Empty matrix file `" << file_name << "` was successfully created.\n ";
}
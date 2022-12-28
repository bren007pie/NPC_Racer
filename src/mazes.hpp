/**
 * @file mazes.hpp
 * @author Brendan Fallon (fallonbr@mcmaster.ca) (https://github.com/bren007pie)
 * @brief This module contains the maze class and writes empty maze files.
 * @version 0.1
 * @date Sunday December 18, 2022
 * @copyright Copyright (c) 2022 Brendan Fallon
 */

//// Preprocessor Directives ////
#include <iostream> // terminal output
#include <fstream>  // file output
#include <string>   // string manipulation
#include <vector>   // vectors

////// ========= //////
////// Interface //////
////// ========= //////

namespace NPC_Racer // using namespace across multiple files: https://stackoverflow.com/questions/4093407/c-namespaces-and-defining-classes-in-separate-files
{
    //// --------- ////
    //// Functions ////
    //// --------- ////
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

    //// ---------- ////
    //// Maze Class ////
    //// ---------- ////
    /**
     * @brief Encapsulation for maze data.
     *
     * @result Maze object that contains data read from a file.
     * @cite Inspired by maze class from https://baraksh.com/CSE701/notes.php
     */
    class maze
    {
    public:
        //// Constructors ////

        /**
         * @brief Construct a maze object from a maze file.
         *
         * @param filename The maze file name of the `*.txt` or `*.csv` file.
         * @result Loads the contents of the maze file into the object.
         */
        maze(const std::string filename);

        //// Member Functions ////

        //// Class Operators ////

        //// Exceptions ////

    private:
        //// Data Members ////

        // The number of rows.
        /**
         * @param row_size The number of rows of the rectangular maze.
         */
        size_t row_size = 0;

        /**
         * @param column_size The number of columns of the rectangular maze.
         */
        size_t column_size = 0;

        /**
         * @param bit_maze A vector storing the bitmap elements of the maze in flattened (1-dimensional) form. True is a barrier, false is a free space.
         */
        std::vector<bool> bit_maze;

        /**
         * @param bit_maze A vector storing the char elements of the maze in flattened (1-dimensional) form. Used for printing.
         */
        std::vector<char> character_maze;

        /**
         * @param start_position an iterator pointing to the source position in the maze.
         * @note Operations happen on the bitmap and positions cannot be changed so is a constant iterator on the bitmap.
         */
        std::vector<bool>::const_iterator start_position;

        /**
         * @param destination_position an iterator pointing to the destination position in the maze.
         * @note Operations happen on the bitmap and positions cannot be changed so is a constant iterator on the bitmap.
         */
        std::vector<bool>::const_iterator destination_position;

        /**
         * @param file_name The file name of the maze with the extension.
         */
        std::string file_name;
    };
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
    std::string maze_size;
    if (comma_separated)
    {
        maze_size = std::to_string(rows) + "," + std::to_string(columns);
        // When Excel saves it records empty elements of first row so need to add columns - 2 commas
        maze_size.resize((maze_size.length() + columns - 2), ',');
        maze_size += "\n";
    }
    else
        maze_size = std::to_string(rows) + " " + std::to_string(columns) + "\n";

    output << maze_size;

    // empty space string with or without the comma
    std::string empty_space;
    if (comma_separated)
        empty_space = ".,"; // if in csv no whitespace needed
    else
        empty_space = ". "; // standard maze space has a single whitespace for readability

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
    std::cout << "Empty maze file `" << file_name << "` was successfully created.\n ";
}

//// ---------- ////
//// Maze Class ////
//// ---------- ////

//// Constructors ////

NPC_Racer::maze::maze(const std::string filename)
{
    // Initial variables //
    bool comma_separated;                         // true if a comma separated value file. False if space separated value file.
    char input_character;                         // the holder for each individual string to be parsed
    const std::string whitespace = " \n\t\v\r\f"; // whitespace characters
    std::string trimmed_filename;                 // filename with leading and trailing whitespace removed

    // sanitizing string to remove leading and trailing whitespace //

    // inspired by this to use substring to trim: https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
    // removing leading whitespace
    size_t start = filename.find_first_not_of(whitespace);
    trimmed_filename = (start == std::string::npos) ? "" : filename.substr(start);
    // removing trailing whitespace
    size_t end = trimmed_filename.find_last_not_of(whitespace);
    trimmed_filename = (end == std::string::npos) ? "" : trimmed_filename.substr(0, end + 1);

    // Checks for if empty invariant once whitespace/- is removed //
    if (trimmed_filename.empty())
    {
        std::cout << "ERROR: File name given `" << filename << "` is empty.\n";
        // TODO: throw empty_input() exception;
    }

    // checking what the extension of the file name is //
    // finding substring after . inspired by Brian Newman's answer: https://stackoverflow.com/questions/51949/how-to-get-file-extension-from-string-in-c
    std::string extension = trimmed_filename.substr(filename.find_last_of(".") + 1);

    if (extension == "txt")
        comma_separated = false;
    else if (extension == "csv")
        comma_separated = true;
    else
    {
        std::cout << "ERROR: Invalid extension `" << extension << "` for file name " << filename << "!\n";
        // TODO: throw exception, invalid_file_type()
    }

    // Opening the ifstream, from: https://baraksh.com/CSE701/notes.php#io-streams-and-files
    std::ifstream input(trimmed_filename);
    if (!input.is_open())
    {
        std::cout << "ERROR: Error opening input file `" << filename << "`!\n\tDoes this file exist in the current working directory?\n";
        exit(EXIT_FAILURE); // because it could be multiple errors not throwing an exception.
    }

    file_name = trimmed_filename; // now that we know it's a valid file, setting the class data member file name

    // Variables for actually parsing //

    uint64_t row = 0;                             // row position
    uint64_t column = 0;                          // column position
    std::string row_size_accumulator;             // holds row size header string
    std::string column_size_accumulator;          // holds column size header string
    char separator = comma_separated ? ',' : ' '; // comma or space separated
    bool separated = false;                       // flag for if delimited

    // Going through the header line //
    input.get(input_character);     // get first character
    while (input_character != '\n') // go until first line is done
    {
        if ((!std::isdigit(input_character)) and (input_character != separator)) // guard for non-digit/separator character
        {
            std::cout << "ERROR: Invalid character `" << input_character
                      << "` at \n\trow: " << (row + 1) << "\n\tcolumn: " << (column + 1)
                      << "\nMaze sizes must be a numeric digit.\n";
            // TODO: throw invalid_character(row,column,digit)
            return;
        }
        else if (std::isdigit(input_character) and !separated) // row size
            row_size_accumulator += input_character;           // append number
        else if (std::isdigit(input_character) and separated)  // column size
            column_size_accumulator += input_character;
        else if (input_character == separator)
            separated = true;

        input.get(input_character); // get next character
        column++;                   // increment column count
    }

    row_size = std::stoull(row_size_accumulator); // storing in data members
    column_size = std::stoull(column_size_accumulator);

    // Going through rest of fixed-width character maze //

    input.get(input_character);        // get next character
    while (input.get(input_character)) // goes until end of file
    {
        // guards for empty cell, no seperator, and setting seperator flag
        if ((input_character == separator) and !separated) // was just a character, set flag
            separated = true;
        else if (/* condition */)
        {
            /* code */
        }

        if (input_character == '.')
        {
            bit_maze[row * column_size + column] = 1;        // 1 for free space
            character_maze[row * column_size + column] = '.' // save character
        }
    }
}
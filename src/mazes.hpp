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
        /**
         * @brief Returns a string of of the maze as a fixed-width 2D array of the maze character elements.
         *
         * @return A string of fixed-width 2D array of the maze character elements.
         * @warning This string is just for display purposes, cannot be copied into a maze file.
         */
        std::string stringify() const;

        /**
         * @brief Prints characters of the maze to the terminal.
         *
         * @result Prints characters of the maze to the terminal.
         * @warning This print out is just for display purposes, cannot be copied into a maze file.
         */
        void print_maze() const;

        //// Internal Overloaded Operators ////

        //// Exceptions ////

    private:
        //// Member Functions ////

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
         * @param bit_maze An intermediate bitmap representation of the maze in flattened (1-dimensional) form. True is a free space, false is a barrier.
         * @note This intermediate representation is not intended for user access.
         */
        std::vector<bool> bit_maze;

        /**
         * @param character_maze A vector storing the char elements of the maze in flattened (1-dimensional) form. Used for printing.
         */
        std::vector<char> character_maze;

        /**
         * @param start_position index of the start position in the flattened maze vector.
         * @note Indexed from zero.
         */
        uint64_t start_position;

        /**
         * @param destination_position index of the destination position in the flattened maze vector.
         * @note Indexed from zero.
         */
        uint64_t destination_position;

        /**
         * @param file_name The file name of the maze with the extension.
         */
        std::string file_name;
    };

    //// External Overloaded Operators ////
    /**
     * @brief Overloaded binary operator << to print out a fixed-width 2D array of the maze elements as characters to a stream.
     *
     * @param out Output stream buffer that accepts strings.
     * @param print_maze The maze you want to print the characters of.
     *
     * @return A reference of out for extraction chaining.
     * @warning This print out is just for display purposes, cannot be copied into a maze file.
     */
    std::ostream &operator<<(std::ostream &out, const maze print_maze);

}

////// ============== //////
////// Implementation //////
////// ============== //////

//// Functions ////

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
        // When Excel saves CSVs it records empty elements of first row so need to add (columns - 2) commas
        maze_size.resize((maze_size.length() + columns - 2), ',');
        maze_size += "\n";
    }
    else
        maze_size = std::to_string(rows) + " " + std::to_string(columns) + "\n";

    output << maze_size;

    // looping through writing a 2D maze of free spaces
    std::string free_space = comma_separated ? ".," : ". "; // free space with space or comma

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            if (j == (columns - 1))      // Excel saves CSVs with no last separator
                output << free_space[0]; // just the "."
            else
                output << free_space;
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

    uint64_t row = 0;                             // row position.
    uint64_t column = 0;                          // column position.
    std::string row_size_accumulator;             // holds row size header string.
    std::string column_size_accumulator;          // holds column size header string.
    char separator = comma_separated ? ',' : ' '; // comma or space separated.
    bool separated = false;                       // flag for if delimited.
    bool start_position_read = false;             // flag for start position counting.
    bool destination_position_read = false;       // flag for destination position counting.

    // Going through the header line //
    input.get(input_character);     // get first character
    while (input_character != '\n') // go until first line is done
    {
        if ((!std::isdigit(input_character)) and (input_character != separator)) // guard for non-digit/separator character
        {
            std::cout << "ERROR: Invalid character `" << input_character
                      << "` in file `" << file_name
                      << "` at \n\trow   : " << (row + 1) << "\n\tcolumn: " << (column + 1)
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

    // converting and storing in data members
    row_size = std::stoull(row_size_accumulator);
    column_size = std::stoull(column_size_accumulator);

    // resizing and initializing bit_maze and character_maze with amount they will need
    bit_maze.reserve(row_size * column_size);
    character_maze.reserve(row_size * column_size);

    bit_maze.resize(row_size * column_size, false);
    character_maze.resize(row_size * column_size, '0');

    // Going through rest of fixed-width character maze //

    row++;      // next row
    column = 0; // reset column

    while (input.get(input_character)) // goes until end of file
    {
        // guards for seperator: set flag, empty cell, double value, and reset seperator flag.
        // two most common cases first for speed checks
        if ((input_character == separator) and !separated) // was just a character, set flag
        {
            separated = true;
            column++;
            continue; // can skip rest of loop because is separator.
        }
        else if ((input_character != separator) and separated) // separated properly, reset
            separated = false;
        else if ((input_character == separator) and separated) // was two separators in a row, empty value.
        {
            std::cout << "ERROR: Empty cell in file `" << file_name << "` at \n\trow   : "
                      << (row + 1) << "\n\tcolumn: " << (column + 1)
                      << "\nEvery position must be filled.\n";
            // TODO: throw empty_cell(row,column)
            return;
        }
        else if (((input_character != separator) and !separated) and (input_character != '\n')) // not separated, not a newline, double value.
        {
            std::cout << "ERROR: Double character `" << input_character
                      << "` in file `" << file_name
                      << "` at \n\trow   : " << (row + 1) << "\n\tcolumn: " << (column + 1)
                      << "\nAll positions must be only one character wide and separated by a comma or whitespace.\n";
            // TODO: throw double_character(row,column)
            return;
        }

        // Parsing rest of the characters: space, destination, source, or wall.

        if (input_character == '.') // '.' is most common besides barrier so is first
        {
            // storing free space in bit and character map in right place
            // because we are error checking can use [] over .at() for speed
            bit_maze[(row - 1) * column_size + (column / 2)] = true;
            character_maze[(row - 1) * column_size + (column / 2)] = '.';
        }
        else if (input_character == '\n') // newline (2nd most common), also checks maze sizes
        {
            if (column != ((column_size * 2) - 1)) // each column should be (column_size * 2 ) wide indexed from 1
            {
                std::cout << "ERROR: Incorrect maze size in file `" << file_name << "` at"
                          << "\n\trow   : " << (row + 1) << "\n\tcolumn: " << (column + 1)
                          << "\nExpected columns: " << (column_size * 2)
                          << "\nGiven columns: " << (column + 1)
                          << "\nMaze must be the rectangular size specified in the first row.\n";
                // TODO: throw incorrect_maze_size(row,column)
                return;
            }

            row++;            // increment row and reset column
            column = 0;       // reset column count for newline
            separated = true; // reset separated for newline

            if (row > (row_size + 1)) // check if too many rows before going on. Should be (row_size + 2) wide indexed from 1
            {
                std::cout << "ERROR: Incorrect maze size in file `" << file_name << "` at"
                          << "\n\trow   : " << (row + 1) << "\n\tcolumn: " << (column + 1)
                          << "\nExpected rows: " << (row_size + 2)
                          << "\nGiven rows: " << (row + 1)
                          << "\nMaze must be the rectangular size specified in the first row.\n";
                // TODO: throw incorrect_maze_size(row,column)
                return;
            }
            continue; // if newline don't want it to increment column so skips rest of parsing
        }
        else if (input_character == '@') // start position
        {
            if (!start_position_read) // storing index if not one in maze already
            {
                start_position = (row - 1) * column_size + (column / 2);
                start_position_read = true; // setting flag
            }
            else // start position read so duplicate
            {
                std::cout << "ERROR: Duplicate character `" << input_character
                          << "` in file `" << file_name
                          << "` at \n\trow   : " << (row + 1) << "\n\tcolumn: " << (column + 1)
                          << "\nThere can only be one start position `@` in a maze.\n";
                // TODO: throw double_character(row,column)
                return;
            }
            // storing free space in bit and character map in right place
            // because we are error checking can use [] over .at() for speed
            bit_maze[(row - 1) * column_size + (column / 2)] = true; // start is a free space
            character_maze[(row - 1) * column_size + (column / 2)] = '@';
        }
        else if ((input_character == 'X') or (input_character == 'x')) // destination position
        {
            if (!destination_position_read) // storing index if not one in maze already
            {
                destination_position = (row - 1) * column_size + (column / 2);
                destination_position_read = true; // setting flag
            }
            else // start position read so duplicate
            {
                std::cout << "ERROR: Duplicate character `" << input_character
                          << "` in file `" << file_name
                          << "` at \n\trow   : " << (row + 1) << "\n\tcolumn: " << (column + 1)
                          << "\nThere can only be one destination position `X` in a maze.\n";
                // TODO: throw double_character(row,column)
                return;
            }

            // comments same as start position
            bit_maze[(row - 1) * column_size + (column / 2)] = true;
            character_maze[(row - 1) * column_size + (column / 2)] = 'X';
        }
        else // barrier
        {
            // storing barrier space in bit and character map in right place
            // because we are error checking can use [] over .at() for speed
            bit_maze[(row - 1) * column_size + (column / 2)] = false;
            character_maze[(row - 1) * column_size + (column / 2)] = input_character;
        }
        column++; // increment column count
    }

    // final sanity checks //
    // if there's no start or destination position
    if (!start_position_read)
    {
        std::cout << "ERROR: Invalid maze file `" << file_name
                  << "`\nThere must be one start position `@` in a maze.\n";
        // TODO: throw invalid_maze
        return;
    }
    if (!destination_position_read)
    {
        std::cout << "ERROR: Invalid maze file `" << file_name
                  << "`\nThere must be one destination position `X` in a maze.\n";
        // TODO: throw invalid_maze
        return;
    }

    // // check if little rows before going on. Should be (row_size + 2) wide indexed from 1
    if (row < (row_size + 1))
    {
        std::cout << "ERROR: Incorrect maze size in file `" << file_name << "` at"
                  << "\n\trow   : " << (row + 1) << "\n\tcolumn: " << (column + 1)
                  << "\nExpected rows: " << (row_size + 2)
                  << "\nGiven rows: " << (row + 1)
                  << "\nMaze must be the rectangular size specified in the first row.\n";
        // TODO: throw incorrect_maze_size(row,column)
        return;
    }

    std::cout << "Maze `" << file_name << "` has been successfully read!\n";
    input.close(); // Always have to close the file!
}

//// Member Functions ////

std::string NPC_Racer::maze::stringify() const
{
    std::string print_accumulator;
    // print maze as characters surrounded by spaces with newlines
    for (size_t i = 0; i < row_size; i++)
    {
        for (size_t j = 0; j < column_size; j++)
        {
            print_accumulator = print_accumulator + character_maze[i * column_size + j];
            print_accumulator += " ";
        }
        print_accumulator += "\n";
    }
    return print_accumulator;
}

void NPC_Racer::maze::print_maze() const
{
    std::cout << this;
}

//// External Overloaded Operators ////

std::ostream &NPC_Racer::operator<<(std::ostream &out, const maze print_maze)
{
    out << print_maze.stringify(); // put the string in the stream
    return out;
}
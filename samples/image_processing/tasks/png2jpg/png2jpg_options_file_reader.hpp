/** \file png2jpg_options_file_reader.hpp
\brief Reader for png2jpg option files.
*/

#ifndef PNG2JPG_OPTIONS_FILE_READER_HPP
#define PNG2JPG_OPTIONS_FILE_READER_HPP

#include "simple_options_file_parser.hpp"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

/// \brief Structure holding the options controlling the
///        behaviour of the application

struct png2jpg_options
{
  string         input_file_name;        /**< Path and name of the PNG image to convert. */

  string         output_filename;        /**< Path and name of the output JPG image.     */

};

/// \brief Reader for png2jpg option files.

class png2jpg_options_file_reader
{
  public:

    /// \brief Retrieve the textual description related to a
    /// numerical error code.
    /**
     \param error_code The code of the error whose textual description
            must be retrieved. Must be in the range of error codes returned
            by method parse_file(); if not, an empty string is returned.
     \return The textual description of the requested error code
            or the empty string if such code is not in the error
            catalogue.
     */

    string& get_error_text               (int               error_code);


    /// \brief Parses the contents of a jpg2png options file,
    /// storing the results memory for easy access.
   /**
     \param options_file Name of the png2jpg options file to parse.
     \param options The structure where the options will be stored.
     \return An error code:
        - 0: Successful completion.
        - 1: Syntax errors found while parsing the png2jpg options file.
        - 2: Missing or mispelled option in in png2jpg options file: INPUT_FILENAME.
        - 3: Missing or mispelled option in png2jpg options file: OUTPUT_FILENAME.
     */

    int     parse_file                   (const string&    options_file,
                                          png2jpg_options& options);

    /// \brief Default constructor.

            png2jpg_options_file_reader  (void);

    /// \brief Destructor.

            ~png2jpg_options_file_reader (void);

  protected:

    /// \brief Builds the list of errors that may show up.
    /// so when these happen it's possible to retrieve
    /// a textual description of the problem in a per-code
    /// basis.

    void    build_error_list             (void);

  protected:

    /// \brief Map with error codes and values.

    map<int, string> error_messages_;
};

#endif // PNG2JPG_OPTIONS_FILE_READER_HPP

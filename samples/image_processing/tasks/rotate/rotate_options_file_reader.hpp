/** \file rotate_options_file_reader.hpp
\brief Reader for rotate option files.
*/

#ifndef ROTATE_OPTIONS_FILE_READER_HPP
#define ROTATE_OPTIONS_FILE_READER_HPP

#include "simple_options_file_parser.hpp"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

/// \brief Structure holding the options controlling the
///        behaviour of the application

struct rotate_options
{
  string         input_file_name;        /**< Path and name of the input image that will
                                              be rotated                                  */

  string         output_filename;        /**< Path and name of the output image,
                                              which is rotated.                           */

  double         degrees;                /**< Angle to rotate the image.                  */
};

/// \brief Reader for rotate option files.

class rotate_options_file_reader
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

    string& get_error_text             (int               error_code);

    /// \brief Parses the contents of a rotate options file,
    /// storing the results memory for easy access.
   /**
     \param options_file Name of the rotate options file to parse.
     \param options The structure where the options will be stored.
     \return An error code:
        - 0: Successful completion.
        - 1: Syntax errors found while parsing the rotate options file.
        - 2: Missing or mispelled option in in rotate options file: INPUT_FILENAME.
        - 3: Missing or mispelled option in rotate options file: OUTPUT_FILENAME.
        - 4: Missing or invalid value in rotate options file: DEGREES. It must be a real value.
     */

    int     parse_file                  (const string&   options_file,
                                         rotate_options& options);

    /// \brief Default constructor.

            rotate_options_file_reader  (void);

    /// \brief Destructor.

            ~rotate_options_file_reader (void);

  protected:

    /// \brief Builds the list of errors that may show up.
    /// so when these happen it's possible to retrieve
    /// a textual description of the problem in a per-code
    /// basis.

    void    build_error_list            (void);

  protected:

    /// \brief Map with error codes and values.

    map<int, string> error_messages_;
};

#endif // ROTATE_OPTIONS_FILE_READER_HPP

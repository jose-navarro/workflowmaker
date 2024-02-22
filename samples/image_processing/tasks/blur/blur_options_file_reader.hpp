/** \file blur_options_file_reader.hpp
\brief Reader for blur option files.
*/

#ifndef BLUR_OPTIONS_FILE_READER_HPP
#define BLUR_OPTIONS_FILE_READER_HPP

#include "simple_options_file_parser.hpp"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

/// \brief Structure holding the options controlling the
///        behaviour of the application

struct blur_options
{
  string         input_file_name;        /**< Path and name of the input image that will
                                              be blurred                                  */

  string         output_filename;        /**< Path and name of the output image,
                                              which is blurred.                           */

  double         radius;                 /**< Radius (in pixels) for the blur operation.  */

  double         sigma;                  /**< Standard deviation for the blur operation.  */
};

/// \brief Reader for blur option files.

class blur_options_file_reader
{
  public:

    /// \brief Default constructor.

            blur_options_file_reader  (void);

    /// \brief Destructor.

            ~blur_options_file_reader (void);

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

    string& get_error_text            (int               error_code);

    /// \brief Parses the contents of an blur options file,
    /// storing the results memory for easy access.
   /**
     \param options_file Name of the blur options file to parse.
     \param options The structure where the options will be stored.
     \return An error code:
        - 0: Successful completion.
        - 1: Syntax errors found while parsing the blur options file.
        - 2: Missing or mispelled option in in blur options file: INPUT_FILENAME.
        - 3: Missing or mispelled option in blur options file: OUTPUT_FILENAME.
        - 4: Missing or invalid value in blur options file: RADIUS. It must be a real value > 0.
        - 5: Missing or invalid value in blur options file: SIGMA. It must be a real value > 0.
     */

    int     parse_file                (const string& options_file,
                                       blur_options& options);

  protected:

    /// \brief Builds the list of errors that may show up.
    /// so when these happen it's possible to retrieve
    /// a textual description of the problem in a per-code
    /// basis.

    void    build_error_list          (void);

  protected:

    /// \brief Map with error codes and values.

    map<int, string> error_messages_;
};

#endif // BLUR_OPTIONS_FILE_READER_HPP

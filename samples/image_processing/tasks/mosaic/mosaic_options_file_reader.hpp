/** \file mosaic_options_file_reader.hpp
\brief Reader for mosaic option files.
*/

#ifndef MOSAIC_OPTIONS_FILE_READER_HPP
#define MOSAIC_OPTIONS_FILE_READER_HPP

#include "simple_options_file_parser.hpp"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

/// \brief Structure holding the options controlling the
///        behaviour of the application

struct mosaic_options
{
  string         left_image_file_name;        /**< Path and name of the image that will
                                                   be placed on the right side of the
                                                   mosaic.                             */

  string         right_image_filename;        /**< Path and name of the image that will
                                                   be placed on the right side of the
                                                   mosaic.                             */

  string         mosaic_file_name;            /**< Path and name of the output mosaic. */
};

/// \brief Reader for mosaic option files.

class mosaic_options_file_reader
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

    string& get_error_text              (int               error_code);

    /// \brief Default constructor.

            mosaic_options_file_reader  (void);

    /// \brief Destructor.

            ~mosaic_options_file_reader (void);


    /// \brief Parses the contents of an mosaic options file,
    /// storing the results memory for easy access.
   /**
     \param options_file Name of the mosaic options file to parse.
     \param options The structure where the options will be stored.
     \return An error code:
        - 0: Successful completion.
        - 1: Syntax errors found while parsing the mosaic options file.
        - 2: Missing or mispelled option in mosaic options file: LEFT_IMAGE_FILENAME.
        - 3: Missing or mispelled option in mosaic options file: RIGHT_IMAGE_FILENAME.
        - 4: Missing or mispelled option in mosaic options file: MOSAIC_FILENAME.
     */

    int     parse_file                  (const string&   options_file,
                                         mosaic_options& options);

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

#endif // MOSAIC_OPTIONS_FILE_READER_HPP

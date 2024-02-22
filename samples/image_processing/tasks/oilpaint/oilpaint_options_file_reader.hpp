/** \file oilpaint_options_file_reader.hpp
\brief Reader for oilpaint option files.
*/

#ifndef OILPAINT_OPTIONS_FILE_READER_HPP
#define OILPAINT_OPTIONS_FILE_READER_HPP

#include "simple_options_file_parser.hpp"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

/// \brief Structure holding the options controlling the
///        behaviour of the application

struct oilpaint_options
{
  string         input_file_name; /**< Path and name of the input image to which
                                       the oilpaint effect will be applied.            */

  string         output_filename; /**< Path and name of the output image,
                                       to which the oilpaint effect has been
                                       applied.                                        */

  double         radius;          /**< Radius (in pixels) for the oilpaint operation.  */
};

/// \brief Reader for oilpaint option files.

class oilpaint_options_file_reader
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

    string& get_error_text        (int               error_code);

    /// \brief Default constructor.

    oilpaint_options_file_reader  (void);

    /// \brief Destructor.

    ~oilpaint_options_file_reader (void);


    /// \brief Parses the contents of an blur options file,
    /// storing the results memory for easy access.
   /**
     \param options_file Name of the oilpaint options file to parse.
     \param options The structure where the options will be stored.
     \return An error code:
        - 0: Successful completion.
        - 1: Syntax errors found while parsing the oilpaint options file.
        - 2: Missing or mispelled option in in oilpaint options file: INPUT_FILENAME.
        - 3: Missing or mispelled option in oilpaint options file: OUTPUT_FILENAME.
        - 4: Missing or invalid value in oilpaint options file: RADIUS. It must be a real value > 0.
     */

    int     parse_file            (const string&     options_file,
                                   oilpaint_options& options);

  protected:

    /// \brief Builds the list of errors that may show up.
    /// so when these happen it's possible to retrieve
    /// a textual description of the problem in a per-code
    /// basis.

    void    build_error_list       (void);

  protected:

    /// \brief Map with error codes and values.

    map<int, string> error_messages_;
};

#endif // OILPAINT_OPTIONS_FILE_READER_HPP

/** \file charcoal_options_file_reader.cpp
\brief Implementation file for charcoal_options_file_reader.hpp.
*/
#include "charcoal_options_file_reader.hpp"

void
charcoal_options_file_reader::
build_error_list
(void)
{
  {
    error_messages_[ 0] = "Successful completion.";
    error_messages_[ 1] = "Syntax errors found while parsing the charcoal options file.";
    error_messages_[ 2] = "Missing or mispelled option in in charcoal options file: INPUT_FILENAME.";
    error_messages_[ 3] = "Missing or mispelled option in charcoal options file: OUTPUT_FILENAME.";
    error_messages_[ 4] = "Missing or invalid value in charcoal options file: RADIUS. It must be a real value > 0.";
    error_messages_[ 5] = "Missing or invalid value in charcoal options file: SIGMA. It must be a real value > 0.";
  }
}

string&
charcoal_options_file_reader::
get_error_text
(int error_code)
{
  string* result;

  {
    map<int, string>::iterator it;

    result = new string("");

    it = error_messages_.find(error_code);

    if (it == error_messages_.end())
      return *result; // Empty string; Error code not found.

    // We've got a correct error code. Copy the error message.

    *result = it->second;

    return *result;
  }
}

int
charcoal_options_file_reader::
parse_file
(const string&     options_file,
 charcoal_options& options)
{
  {
    int                        error_line;
    simple_options_file_parser sofp;
    int                        status;
    string                     tstring;

    // Parse the requested options file.

    status = sofp.parse(options_file, error_line);
    if (status != 0) return 1;

    //
    // Get all the options in the file, one by one. We'll try to
    // retrieve these using the proper types (int, double, etc.)
    // to check that they are correctly written in the file.
    //

    status = sofp.get_option_string ("INPUT_FILENAME",  options.input_file_name);
    if (status != 0) return 2;

    status = sofp.get_option_string ("OUTPUT_FILENAME", options.output_filename);
    if (status != 0) return 3;

    status = sofp.get_option_double ("RADIUS",          options.radius);
    if (status != 0) return 4;
    if (options.radius <= 0.0) return 4;

    status = sofp.get_option_double ("SIGMA",           options.sigma);
    if (status != 0) return 5;
    if (options.sigma <= 0.0) return 5;

    // That's all!

    return 0;
  }
}

charcoal_options_file_reader::
charcoal_options_file_reader
(void)
{
  {
    build_error_list();
  }
}

charcoal_options_file_reader::
~charcoal_options_file_reader
(void)
{
  {
    // Intentionally left blank.
  }
}

/** \file grayscale_options_file_reader.cpp
\brief Implementation file for grayscale_options_file_reader.hpp.
*/
#include "grayscale_options_file_reader.hpp"

void
grayscale_options_file_reader::
build_error_list
(void)
{
  {
    error_messages_[ 0] = "Successful completion.";
    error_messages_[ 1] = "Syntax errors found while parsing the grayscale options file.";
    error_messages_[ 2] = "Missing or mispelled option in in grayscale options file: INPUT_FILENAME.";
    error_messages_[ 3] = "Missing or mispelled option in grayscale options file: OUTPUT_FILENAME.";
  }
}

string&
grayscale_options_file_reader::
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
grayscale_options_file_reader::
parse_file
(const string&      options_file,
 grayscale_options& options)
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

    // That's all!

    return 0;
  }
}

grayscale_options_file_reader::
grayscale_options_file_reader
(void)
{
  {
    build_error_list();
  }
}

grayscale_options_file_reader::
~grayscale_options_file_reader
(void)
{
  {
    // Intentionally left blank.
  }
}

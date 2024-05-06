/** \file rotate_options_file_reader.cpp
\brief Implementation file for rotate_options_file_reader.hpp.
*/
#include "rotate_options_file_reader.hpp"

void
rotate_options_file_reader::
build_error_list
(void)
{
  {
    error_messages_[ 0] = "Successful completion.";
    error_messages_[ 1] = "Syntax errors found while parsing the rotate options file.";
    error_messages_[ 2] = "Missing or mispelled option in in rotate options file: INPUT_FILENAME.";
    error_messages_[ 3] = "Missing or mispelled option in rotate options file: OUTPUT_FILENAME.";
    error_messages_[ 4] = "Missing or invalid value in rotate options file: DEGREES. It must be a real value.";
  }
}

string&
rotate_options_file_reader::
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
rotate_options_file_reader::
parse_file
(const string& options_file,
 rotate_options& options)
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

    status = sofp.get_option_double ("DEGREES",         options.degrees);
    if (status != 0) return 4;

    // That's all!

    return 0;
  }
}

rotate_options_file_reader::
rotate_options_file_reader
(void)
{
  {
    build_error_list();
  }
}

rotate_options_file_reader::
~rotate_options_file_reader
(void)
{
  {
    // Intentionally left blank.
  }
}

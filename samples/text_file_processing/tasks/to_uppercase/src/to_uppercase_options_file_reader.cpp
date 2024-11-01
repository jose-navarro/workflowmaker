/** \file to_uppercase_options_file_reader.cpp
\brief Implementation file for to_uppercase_options_file_reader.hpp.
*/
#include "to_uppercase_options_file_reader.hpp"

to_uppercase_options_file_reader::
to_uppercase_options_file_reader
(void)
{
  {
    build_error_list();
  }
}

to_uppercase_options_file_reader::
~to_uppercase_options_file_reader
(void)
{
  {
    // Intentionally left blank.
  }
}

void
to_uppercase_options_file_reader::
build_error_list
(void)
{
  {
    error_messages_[ 0] = "Successful completion.";
    error_messages_[ 1] = "Syntax errors found while parsing the options file.";
    error_messages_[ 2] = "Missing or mispelled option in the options file: TO_UPPERCASE_INPUT_FILENAME.";
    error_messages_[ 3] = "Missing or mispelled option in the options file: TO_UPPERCASE_OUTPUT_FILENAME.";
  }
}

string&
to_uppercase_options_file_reader::
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
to_uppercase_options_file_reader::
parse_file
(const string&               options_file,
       to_uppercase_options& options)
{
  {
    int    error_line;
    int    status;
    string tstring;

    // Parse the requested options file.

    status = options_reader_.parse(options_file, error_line);
    if (status != 0) return 1;

    //
    // Get all the options in the file, one by one. We'll try to
    // retrieve these using the proper types (int, double, etc.)
    // to check that they are correctly written in the file.
    //

    status = options_reader_.get_option_string ("TO_UPPERCASE_INPUT_FILENAME",  options.input_file_name);
    if (status != 0) return 2;

    status = options_reader_.get_option_string ("TO_UPPERCASE_OUTPUT_FILENAME", options.output_filename);
    if (status != 0) return 3;

    // That's all!

    return 0;
  }
}

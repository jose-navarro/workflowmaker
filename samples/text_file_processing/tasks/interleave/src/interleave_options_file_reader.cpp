/** \file interleave_options_file_reader.cpp
\brief Implementation file for interleave_options_file_reader.hpp.
*/
#include "interleave_options_file_reader.hpp"

interleave_options_file_reader::
interleave_options_file_reader
(void)
{
  {
    build_error_list();
  }
}

interleave_options_file_reader::
~interleave_options_file_reader
(void)
{
  {
    // Intentionally left blank.
  }
}

void
interleave_options_file_reader::
build_error_list
(void)
{
  {
    error_messages_[ 0] = "Successful completion.";
    error_messages_[ 1] = "Syntax errors found while parsing the options file.";
    error_messages_[ 2] = "Missing or mispelled option in the options file: INTERLEAVE_INPUT_FILENAME_1.";
    error_messages_[ 3] = "Missing or mispelled option in the options file: INTERLEAVE_INPUT_FILENAME_2.";
    error_messages_[ 4] = "Missing or mispelled option in the options file: INTERLEAVE_OUTPUT_FILENAME.";
    error_messages_[ 5] = "Missing or invalid value in the options file: INTERLEAVE_COPY_QUEUE must be 0 or 1.";
  }
}

string&
interleave_options_file_reader::
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
interleave_options_file_reader::
parse_file
(const string&               options_file,
       interleave_options& options)
{
  {
    int    error_line;
    int    status;
    int    tmp_value;
    string tstring;

    // Parse the requested options file.

    status = options_reader_.parse(options_file, error_line);
    if (status != 0) return 1;

    //
    // Get all the options in the file, one by one. We'll try to
    // retrieve these using the proper types (int, double, etc.)
    // to check that they are correctly written in the file.
    //

    status = options_reader_.get_option_string ("INTERLEAVE_INPUT_FILENAME_1", options.input_file_name_1);
    if (status != 0) return 2;

    status = options_reader_.get_option_string ("INTERLEAVE_INPUT_FILENAME_2", options.input_file_name_2);
    if (status != 0) return 3;

    status = options_reader_.get_option_string ("INTERLEAVE_OUTPUT_FILENAME",  options.output_filename);
    if (status != 0) return 4;

    status = options_reader_.get_option_int    ("INTERLEAVE_COPY_QUEUE",       tmp_value);
    if (status != 0) return 5;

    if ((tmp_value < 0) || (tmp_value > 1))
      return 5;

    options.copy_queue = (tmp_value == 1);

    // That's all!

    return 0;
  }
}

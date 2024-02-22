/** \file mosaic_options_file_reader.cpp
\brief Implementation file for mosaic_options_file_reader.hpp.
*/
#include "mosaic_options_file_reader.hpp"

void
mosaic_options_file_reader::
build_error_list
(void)
{
  {
    error_messages_[ 0] = "Successful completion.";
    error_messages_[ 1] = "Syntax errors found while parsing the mosaic options file.";
    error_messages_[ 2] = "Missing or mispelled option in in mosaic options file: LEFT_IMAGE_FILENAME.";
    error_messages_[ 3] = "Missing or mispelled option in mosaic options file: RIGHT_IMAGE_FILENAME.";
    error_messages_[ 4] = "Missing or invalid value in mosaic options file: MOSAIC_FILENAME.";
  }
}

string&
mosaic_options_file_reader::
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

mosaic_options_file_reader::
mosaic_options_file_reader
(void)
{
  {
    build_error_list();
  }
}

mosaic_options_file_reader::
~mosaic_options_file_reader
(void)
{
  {
    // Intentionally left blank.
  }
}

int
mosaic_options_file_reader::
parse_file
(const string&   options_file,
 mosaic_options& options)
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

    status = sofp.get_option_string ("LEFT_IMAGE_FILENAME",  options.left_image_file_name);
    if (status != 0) return 2;

    status = sofp.get_option_string ("RIGHT_IMAGE_FILENAME", options.right_image_filename);
    if (status != 0) return 3;

    status = sofp.get_option_string ("MOSAIC_FILENAME",      options.mosaic_file_name);
    if (status != 0) return 4;

    // That's all!

    return 0;
  }
}

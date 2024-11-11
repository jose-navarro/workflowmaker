#include <list> 
#include <set>
#include <string>
#include <iostream>

#include <Magick++.h> 

#include "jpg2png_options_file_reader.hpp"

using namespace std;
using namespace Magick;

/// \brief Find the path plus base name and the extension of some file name.
/**
  \param filename The file whose path plus basename and extension must be found.
  \param path_and_basename The path and base name of the file.
  \param extension The extension (no dot) of the file.
*/

void
filename_split
(const string& filename,
       string& path_and_basename,
       string& extension)
{
  {
    size_t found;

    found = filename.find_last_of(".");

    if (found == string::npos)
    {
      path_and_basename = filename;
      extension = "";
    }
    else
    {
      path_and_basename = filename.substr(0, found);
      if (found == (filename.size() - 1))
      {
        extension = "";
      }
      else
      {
        extension = filename.substr(found + 1);
      }
    }
  }
}

/// \brief Converts a jpg image into a png one.

int
main
(int    argc,
 char** argv)
{
  {
    try
    {

      //
      // Initialize Magick++
      // Do this in the first place so it is possible to
      // declare objects defined by this library.
      //

      InitializeMagick(*argv);

      // Declare our variables and objects.

      string                      ext;
      set<string>                 ext_jpg;
      string                      name_options_file;
      string                      name_the_input_image;
      string                      name_the_output_image;
      jpg2png_options_file_reader op_reader;
      jpg2png_options             options;
      string                      path_and_basename;
      int                         status;
      Image                       the_image;

      //
      // Check that we've got our unique parameter (the
      // name of the options file)
      //

      if (argc < 2) return 1; // No options file name in command line.

      // The valid extensions for jpg files:

      ext_jpg.insert("jpg");
      ext_jpg.insert("JPG");
      ext_jpg.insert("jpeg");
      ext_jpg.insert("JPEG");
    
      // Read the options controlling our behaviour.

      name_options_file = argv[1];
      status = op_reader.parse_file(name_options_file, options);

      if (status != 0) return 2; // Error reading the options file.

      name_the_input_image  = options.input_file_name;
      name_the_output_image = options.output_filename;

      // Check that the input file name ends in any of the valid jpeg extensions.

      path_and_basename = ext = "";
      filename_split(name_the_input_image, path_and_basename, ext);

      set<string>::iterator finder;
      finder = ext_jpg.find(ext);
      if (finder == ext_jpg.end())
        return 3; // Invalid input file type.

      // Check that the output file name ends in any of the valid png extensions.

      path_and_basename = ext = "";
      filename_split(name_the_output_image, path_and_basename, ext);

      if ((ext != "png") && (ext != "PNG"))
        return 4; // Invalid output file type.

      // Everything is OK. Read input the image.

      the_image.read(name_the_input_image);

      // Write the image to disk.

      the_image.write(name_the_output_image);

      // That's all.

      return 0; // This error codes says "everything went right".
    }
    catch (...)
    {
       return 5; // Error when processing the image(s).
    }
  }
}

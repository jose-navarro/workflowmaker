#include <list> 
#include <string>
#include <iostream>

#include <Magick++.h> 

#include "grayscale_options_file_reader.hpp"

using namespace std;
using namespace Magick;

/// \brief Converts a color image to grayscale.

int main(int argc, char** argv)
{
  {
    try
    {

      // Initialize Magick++

      InitializeMagick(*argv);

      string                        name_options_file;
      string                        name_the_input_image;
      string                        name_the_output_image;
      grayscale_options_file_reader op_reader;
      grayscale_options             options;
      int                           status;
      Image                         the_image;
    
      // Read the options controlling our behaviour.

      name_options_file = argv[1];
      status = op_reader.parse_file(name_options_file, options);

      if (status != 0) return 1; // Error reading the options file.

      name_the_input_image  = options.input_file_name;
      name_the_output_image = options.output_filename;

      // Read the images.

      the_image.read(name_the_input_image);

      // Set its type to grayscale.

      the_image.type(Magick::GrayscaleType);

      // Write the image to disk.

      the_image.write(name_the_output_image);

      // That's all.

      return 0;
    }
    catch (...)
    {
      // Return a status code stating that an error occurred.
      return 1;
    }
  }
}
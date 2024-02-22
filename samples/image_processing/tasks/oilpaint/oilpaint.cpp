#include <list> 
#include <string>
#include <iostream>

#include <Magick++.h> 

#include "oilpaint_options_file_reader.hpp"

using namespace std;
using namespace Magick;

/// \brief Apply the oil paint to an image.

int main(int argc, char** argv)
{
  {
    try
    {

      // Initialize Magick++

      InitializeMagick(*argv);

      string                       name_options_file;
      string                       name_the_input_image;
      string                       name_the_output_image;
      oilpaint_options_file_reader op_reader;
      oilpaint_options             options;
      double                       radius;
      int                          status;
      Image                        the_image;
    
      // Read the options controlling our behaviour.

      name_options_file = argv[1];
      status = op_reader.parse_file(name_options_file, options);

      if (status != 0) return 1; // Error reading the options file.

      name_the_input_image  = options.input_file_name;
      name_the_output_image = options.output_filename;
      radius = options.radius;

      // Read the images.

      the_image.read(name_the_input_image);

      // Apply the oilpaint effect the image, according to the parameters read.

      the_image.oilPaint (radius);

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
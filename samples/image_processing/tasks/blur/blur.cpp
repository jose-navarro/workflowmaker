#include <list> 
#include <string>
#include <iostream>

#include <Magick++.h> 

#include "blur_options_file_reader.hpp"

using namespace std;
using namespace Magick;

/// \brief Apply the blur effect to an image.

int main(int argc, char** argv)
{
  {
    try
    {

      // Initialize Magick++

      InitializeMagick(*argv);

      double                   blur_radius;
      double                   blur_sigma;
      string                   name_options_file;
      string                   name_the_input_image;
      string                   name_the_output_image;
      blur_options_file_reader op_reader;
      blur_options             options;
      int                      status;
      Image                    the_image;
    
      // Read the options controlling our behaviour.

      name_options_file = argv[1];
      status = op_reader.parse_file(name_options_file, options);

      if (status != 0) return 1; // Error reading the options file.

      blur_radius           = options.radius;
      blur_sigma            = options.sigma;
      name_the_input_image  = options.input_file_name;
      name_the_output_image = options.output_filename;

      // Read the images.

      the_image.read(name_the_input_image);

      // Blur the image, according to the parameters read.

      the_image.blur(blur_radius, blur_sigma);

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
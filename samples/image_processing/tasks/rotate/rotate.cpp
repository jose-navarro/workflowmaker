#include <list> 
#include <string>
#include <iostream>

#include <Magick++.h> 

#include "rotate_options_file_reader.hpp"

using namespace std;
using namespace Magick;

/// \brief Rotate an image.

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

      double                     degrees;
      string                     name_options_file;
      string                     name_the_input_image;
      string                     name_the_output_image;
      rotate_options_file_reader op_reader;
      rotate_options             options;
      int                        status;
      Image                      the_image;

      //
      // Check that we've got our unique parameter (the
      // name of the options file)
      //

      if (argc < 2) return 1; // No options file name in command line.
    
      // Read the options controlling our behaviour.

      name_options_file = argv[1];
      status = op_reader.parse_file(name_options_file, options);

      if (status != 0) return 2; // Error reading the options file.

      degrees               = options.degrees;
      name_the_input_image  = options.input_file_name;
      name_the_output_image = options.output_filename;

      // Read the images.

      the_image.read(name_the_input_image);

      // Rotate the image, according to the parameters read.

      the_image.rotate(degrees);

      // Write the image to disk.

      the_image.write(name_the_output_image);

      // That's all.

      return 0; // This error codes says "everything went right".
    }
    catch (...)
    {
       return 3; // Error when processing the image(s).
    }
  }
}
